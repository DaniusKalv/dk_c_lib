/**
 * @file        tlv320aic3106.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TLV320AIC3106 audio CODEC driver.
 * @version     0.1
 * @date        2019-10-02
 * 
 * @copyright   Copyright (c) 2019 Danius Kalvaitis
 * 
 */

#include "tlv320aic3106.h"
#include "tlv320aic3106-internal.h"
#include "sdk_errors.h"
#include "sdk_macros.h"
#include "app_util.h"

#include "nrf_delay.h"

#define NRF_LOG_MODULE_NAME tlv320aic3106
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#define TLV320AIC3106_TWI_WRITE_BUFFER_SIZE  1  /**< Maximum amount of data that will be transfered in one write. */
#define TLV320AIC3106_TWI_READ_BUFFER_SIZE   2  /**< Maximum amount of data that will be read in one read. */

/** @brief TWI write structure. */
typedef struct
{
	uint8_t reg_address;                               /**< Register address. */
	uint8_t data[TLV320AIC3106_TWI_WRITE_BUFFER_SIZE]; /**< Data buffer. */
} tlv320aic3106_twi_write_t;

static ret_code_t twi_write(tlv320aic3106_t           * p_tlv320aic3106,
                            tlv320aic3106_twi_write_t * p_twi_write,
                            uint8_t                     write_size);

static ret_code_t tlv320aic3106_page_select_set(tlv320aic3106_t           * p_tlv320aic3106, 
                                                tlv320aic3106_active_page_t page);

/**
 * @brief       Function to be called by twi manager upon twi transaction result.
 * 
 * @param[in]   result        Transaction result.
 * @param[in]   evt           Event.
 * @param[in]   p_transfer    Pointer to transfer data.
 * @param[in]   p_user_data   Pointer to user data.
 */
static void twi_mngr_callback(ret_code_t result, uint8_t evt, dk_twi_mngr_transfer_t * p_transfer, void * p_user_data)
{
	if(result != NRF_SUCCESS)
	{
		tlv320aic3106_t * p_tlv320aic3106 = (tlv320aic3106_t *)p_user_data;

		NRF_LOG_ERROR("Error: 0x%x", result);
		if(p_tlv320aic3106->error_handler)
		{
			p_tlv320aic3106->error_handler(result, p_tlv320aic3106);
		}
	}
}

static inline tlv320aic3106_active_page_t extract_page_from_reg(uint8_t * p_reg_address)
{
	if(*p_reg_address & TLV320AIC3106_PAGE1_BIT)
	{
		*p_reg_address &= ~TLV320AIC3106_PAGE1_BIT;
		return TLV320AIC3106_ACTIVE_PAGE_1;
	}

	return TLV320AIC3106_ACTIVE_PAGE_0;
}

static inline ret_code_t set_reg_page(tlv320aic3106_t * p_tlv320aic3106,
                                      uint8_t         * p_reg_address)
{
	if(*p_reg_address != TLV320AIC3106_PAGE_SELECT)
	{
		tlv320aic3106_active_page_t page = extract_page_from_reg(p_reg_address);
		return tlv320aic3106_page_select_set(p_tlv320aic3106, page);
	}

	return NRF_SUCCESS;
}

/**
 * @brief       Function for scheduling a twi TX transfer using dk_twi_mngr.
 * 
 * @param[in]   p_tlv320aic3106 Pointer to tlv320aic3106 instance.
 * @param[in]   p_twi_write     Pointer to write buffer.
 * @param[in]   write_size      Write buffer size.
 * 
 * @return      NRF_SUCCESS     Upon successful twi transaction scheduling.
 * @return      Other           Error codes returned by @dk_twi_mngr_schedule.
 */
static ret_code_t twi_write(tlv320aic3106_t           * p_tlv320aic3106,
                            tlv320aic3106_twi_write_t * p_twi_write,
                            uint8_t                     write_size)
{
	ret_code_t err_code;

	err_code = set_reg_page(p_tlv320aic3106, &p_twi_write->reg_address);
	VERIFY_SUCCESS(err_code);

	dk_twi_mngr_transaction_t twi_transaction =
	{
		.callback           = twi_mngr_callback,
		.p_user_data        = (void *)p_tlv320aic3106,
		.transfer           = DK_TWI_MNGR_TX(p_tlv320aic3106->i2c_address, (uint8_t *)p_twi_write, write_size, 0)
	};

	return dk_twi_mngr_schedule(p_tlv320aic3106->p_dk_twi_mngr_instance,
	                            &twi_transaction);
}

/**
 * @brief   Function to be called by dk_twi_mngr when waiting for a tansfer to finish.
 *          Just waits for one millisecond.
 */
static void wait_for_transfer_complete() // TODO: Look for ways to utilize this wait time for something else
{
	nrf_delay_ms(1);
}

/**
 * @brief       Perform a blocking twi read.
 * 
 * @note        Use this function when data must be read imediately (ie during initialization).
 * 
 * @param[in]   p_tlv320aic3106 Pointer to tlv320aic3106 instance.
 * @param[in]   reg             Register address to read from.
 * @param[in]   p_buffer        Pointer to read buffer.
 * @param[in]   buffer_size     Read buffer size.
 * 
 * @return      NRF_SUCESS      Upon successful twi transaction.
 * @return      Other           Error codes returned by @dk_twi_mngr_perform function.
 */
static ret_code_t twi_read_blocking(tlv320aic3106_t const * p_tlv320aic3106,
                                    uint8_t                 reg,
                                    uint8_t               * p_buffer,
                                    uint8_t                 buffer_size)
{
	dk_twi_mngr_transfer_t twi_transfer = DK_TWI_MNGR_TX_RX(p_tlv320aic3106->i2c_address,
	                                                        &reg,
	                                                        sizeof(reg),
	                                                        p_buffer,
	                                                        buffer_size,
	                                                        NRFX_TWI_FLAG_TX_NO_STOP);

	return dk_twi_mngr_perform(p_tlv320aic3106->p_dk_twi_mngr_instance, &twi_transfer, wait_for_transfer_complete);
}

static ret_code_t tlv320aic3106_page_select_get(tlv320aic3106_t             * p_tlv320aic3106, 
                                                tlv320aic3106_active_page_t * p_page)
{
	return twi_read_blocking(p_tlv320aic3106,
	                         TLV320AIC3106_PAGE_SELECT,
	                         (uint8_t *) p_page,
	                         sizeof(tlv320aic3106_active_page_t));
}

static ret_code_t tlv320aic3106_page_select_set(tlv320aic3106_t           * p_tlv320aic3106, 
                                                tlv320aic3106_active_page_t page)
{
	ret_code_t err_code;
	tlv320aic3106_active_page_t page_check;

	if(p_tlv320aic3106->active_page == page)
	{
		return NRF_SUCCESS;
	}

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_page_data, sizeof(page));
	p_page_data->reg_address = TLV320AIC3106_PAGE_SELECT;
	p_page_data->data[0]     = page;

	err_code = twi_write(p_tlv320aic3106,
	                     p_page_data,
	                     p_page_data_size);
	VERIFY_SUCCESS(err_code);

	err_code = tlv320aic3106_page_select_get(p_tlv320aic3106, &page_check);
	VERIFY_SUCCESS(err_code);

	if(page_check == page)
	{
		p_tlv320aic3106->active_page = page;
		return NRF_SUCCESS;
	}
	else
	{
		return NRF_ERROR_INTERNAL;
	}
}

ret_code_t tlv320aic3106_init(tlv320aic3106_t * p_tlv320aic3106,
                              tlv320aic3106_error_handler_t error_handler)
{
	NRF_LOG_INFO("Initializing TLV320AIC3106");

	p_tlv320aic3106->error_handler = error_handler;

	return tlv320aic3106_soft_rst(p_tlv320aic3106);
}

ret_code_t tlv320aic3106_soft_rst(tlv320aic3106_t * p_tlv320aic3106)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_soft_rst, sizeof(uint8_t));
	p_soft_rst->reg_address = TLV320AIC3106_SOFT_RST;
	p_soft_rst->data[0]     = TLV320AIC3106_SOFT_RST_VAL;

	return twi_write(p_tlv320aic3106,
	                 p_soft_rst,
	                 p_soft_rst_size);
	// p_tlv320aic3106->active_page = TLV320AIC3106_ACTIVE_PAGE_0; // TODO: Set active page to 0 after successful soft reset
}

ret_code_t tlv320aic3106_set_passive_ana_sig_bypass_sel_pd(tlv320aic3106_t * p_tlv320aic3106,
                                                           tlv320aic3106_passive_ana_sig_bypass_sel_pd_t * p_bypass)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_bypass_cmd, sizeof(tlv320aic3106_passive_ana_sig_bypass_sel_pd_t));
	p_bypass_cmd->reg_address = TLV320AIC3106_PASSIVE_ANA_SIG_BYPASS_SEL_PD;
	memcpy(p_bypass_cmd->data, p_bypass, sizeof(tlv320aic3106_passive_ana_sig_bypass_sel_pd_t));

	return twi_write(p_tlv320aic3106, p_bypass_cmd, p_bypass_cmd_size);
}

ret_code_t tlv320aic3106_set_pll_prog_reg_a(tlv320aic3106_t * p_tlv320aic3106,
                                            tlv320aic3106_pll_prog_reg_a_t * p_pll_prog_reg_a)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_pll_prog_reg_a_t));
	p_cmd->reg_address = TLV320AIC3106_PLL_PROG_REG_A;
	memcpy(p_cmd->data, p_pll_prog_reg_a, sizeof(tlv320aic3106_pll_prog_reg_a_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_pll_prog_reg_b(tlv320aic3106_t * p_tlv320aic3106,
                                            tlv320aic3106_pll_prog_reg_b_t * p_pll_prog_reg_b)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_pll_prog_reg_b_t));
	p_cmd->reg_address = TLV320AIC3106_PLL_PROG_REG_B;
	memcpy(p_cmd->data, p_pll_prog_reg_b, sizeof(tlv320aic3106_pll_prog_reg_b_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_pll_d(tlv320aic3106_t * p_tlv320aic3106, uint16_t d_value)
{
	if(d_value > TLV320AIC3106_MAX_PLL_D_VAL)
	{
		return NRF_ERROR_INVALID_PARAM;
	}

	uint16_t encoded_d = uint16_big_decode(&d_value);
	encoded_d <<= 2; // Shift by two bits

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(d_value));
	p_cmd->reg_address = TLV320AIC3106_PLL_PROG_REG_C;
	memcpy(p_cmd->data, &encoded_d, sizeof(encoded_d));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_datapath(tlv320aic3106_t * p_tlv320aic3106,
                                      tlv320aic3106_datapath_setup_t * p_datapath_setup)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_datapath, sizeof(tlv320aic3106_datapath_setup_t));
	p_datapath->reg_address = TLV320AIC3106_DATAPATH_SETUP;
	memcpy(p_datapath->data, p_datapath_setup, sizeof(tlv320aic3106_datapath_setup_t));

	return twi_write(p_tlv320aic3106, p_datapath, p_datapath_size);
}

ret_code_t tlv320aic3106_set_audio_ser_data_interface_ctrl_a(tlv320aic3106_t * p_tlv320aic3106,
                                                             tlv320aic3106_audio_ser_data_interface_ctrl_a_t * p_audio_ser_data_interface_ctrl_a)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_audio_ser_data_interface_ctrl_a_t));
	p_cmd->reg_address = TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_CTRL_A;
	memcpy(p_cmd->data, p_audio_ser_data_interface_ctrl_a, sizeof(tlv320aic3106_audio_ser_data_interface_ctrl_a_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_audio_ser_data_interface_ctrl_b(tlv320aic3106_t * p_tlv320aic3106,
                                                             tlv320aic3106_audio_ser_data_interface_ctrl_b_t * p_audio_ser_data_interface_ctrl_b)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_audio_ser_data_interface_ctrl_b_t));
	p_cmd->reg_address = TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_CTRL_B;
	memcpy(p_cmd->data, p_audio_ser_data_interface_ctrl_b, sizeof(tlv320aic3106_audio_ser_data_interface_ctrl_b_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_pll_r(tlv320aic3106_t * p_tlv320aic3106,
                                   tlv320aic3106_pll_r_t pll_r)
{
	tlv320aic3106_audio_codec_overflow_flag_t overflow_reg =
	{
		.pll_r= pll_r
	};

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(overflow_reg));
	p_cmd->reg_address = TLV320AIC3106_AUDIO_CODEC_OVERFLOW_FLAG;
	memcpy(p_cmd->data, &overflow_reg, sizeof(overflow_reg));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_ac_pwr_and_out_drv_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                     tlv320aic3106_ac_pwr_and_out_drv_ctrl_t * p_ac_pwr_and_out_drv_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_ac_pwr_and_out_drv_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_AC_PWR_AND_OUT_DRV_CTRL;
	memcpy(p_cmd->data, p_ac_pwr_and_out_drv_ctrl, sizeof(tlv320aic3106_ac_pwr_and_out_drv_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_dac_out_switch_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                 tlv320aic3106_dac_out_switch_ctrl_t * p_dac_out_switch_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_dac_out_switch_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_DAC_OUT_SWITCH_CTRL;
	memcpy(p_cmd->data, p_dac_out_switch_ctrl, sizeof(tlv320aic3106_dac_out_switch_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_left_dac_dig_volume_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                      tlv320aic3106_dac_dig_volume_ctrl_t * p_dac_dig_volume_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_dac_dig_volume_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_LEFT_DAC_DIG_VOLUME_CTRL;
	memcpy(p_cmd->data, p_dac_dig_volume_ctrl, sizeof(tlv320aic3106_dac_dig_volume_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_right_dac_dig_volume_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                      tlv320aic3106_dac_dig_volume_ctrl_t * p_dac_dig_volume_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_dac_dig_volume_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_RIGHT_DAC_DIG_VOLUME_CTRL;
	memcpy(p_cmd->data, p_dac_dig_volume_ctrl, sizeof(tlv320aic3106_dac_dig_volume_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_left_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                     tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_LEFT_LOP_M_OUT_LVL_CTRL;
	memcpy(p_cmd->data, p_out_lvl_ctrl, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_right_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                      tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_RIGHT_LOP_M_OUT_LVL_CTRL;
	memcpy(p_cmd->data, p_out_lvl_ctrl, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_gpio_ctrl_b(tlv320aic3106_t * p_tlv320aic3106,
                                         tlv320aic3106_gpio_ctrl_b_t * p_gpio_ctrl_b)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_gpio_ctrl_b_t));
	p_cmd->reg_address = TLV320AIC3106_GPIO_CTRL_B;
	memcpy(p_cmd->data, p_gpio_ctrl_b, sizeof(tlv320aic3106_gpio_ctrl_b_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_clk_gen_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                          tlv320aic3106_clk_gen_ctrl_t * p_clk_gen_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_clk_gen_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_CLK_GEN_CTRL;
	memcpy(p_cmd->data, p_clk_gen_ctrl, sizeof(tlv320aic3106_clk_gen_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

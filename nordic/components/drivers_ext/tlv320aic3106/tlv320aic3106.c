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
#include "nrf_log_ctrl.h"
NRF_LOG_MODULE_REGISTER();

#define TLV320AIC3106_TWI_WRITE_BUFFER_SIZE  4  /**< Maximum amount of data that will be transfered in one write. */
#define TLV320AIC3106_TWI_READ_BUFFER_SIZE   2  /**< Maximum amount of data that will be read in one read. */

/** @brief TWI write structure. */
typedef struct
{
	uint8_t reg_address;                               /**< Register address. */
	uint8_t data[TLV320AIC3106_TWI_WRITE_BUFFER_SIZE]; /**< Data buffer. */
} tlv320aic3106_twi_write_t;

/** @brief TWI read structure. */
typedef struct
{
	uint8_t reg_address;                              /**< Register address. */
	uint8_t data[TLV320AIC3106_TWI_READ_BUFFER_SIZE]; /**< Data buffer. */
} tlv320aic3106_twi_read_t;

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
	tlv320aic3106_t * p_tlv320aic3106 = (tlv320aic3106_t *)p_user_data;

	tlv320aic3106_evt_t event =
	{
		.p_tlv320aic3106 = p_tlv320aic3106
	};

	if(result != NRF_SUCCESS)
	{
		NRF_LOG_ERROR("Error: 0x%x", result);

		event.type = TLV320AIC3106_EVT_TYPE_ERROR;
		event.params.err_code = result;

		if(p_tlv320aic3106->evt_handler != NULL)
		{
			p_tlv320aic3106->evt_handler(&event);
		}
	}
	else
	{
		if(p_tlv320aic3106->evt_handler == NULL)
		{
			return;
		}

		switch (evt)
		{
			case TLV320AIC3106_EVT_TYPE_RX_MODULE_PWR_STATUS:
				event.type = TLV320AIC3106_EVT_TYPE_RX_MODULE_PWR_STATUS;
				event.params.p_module_pwr_status = (tlv320aic3106_module_pwr_status_t *)p_transfer->transfer_description.p_secondary_buf;
				break;
			default:
				return;
		}

		p_tlv320aic3106->evt_handler(&event);
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
 * @return      Other           Error codes returned by @dk_twi_mngr_scheduler.
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
 * @brief       Perform a TWI read operation (non-blocking).
 * 
 * @param[in]   p_tlv320aic3106     Pointer to TLV320AIC3106 instance.
 * @param[in]   p_twi_read          Pointer twi_read struct.
 * @param[in]   read_size           Amount of bytes to read.
 * @param[in]   evt_type            Event type.
 * 
 * @retval      NRF_SUCCESS         On success.
 * @retval      Other               Error codes returned by dk_twi_mngr functions.
 */
static ret_code_t twi_read(tlv320aic3106_t    const * p_tlv320aic3106,
                           tlv320aic3106_twi_read_t * p_twi_read,
                           uint8_t                    read_size,
                           tlv320aic3106_evt_type_t   evt_type)
{
	dk_twi_mngr_transaction_t twi_transaction =
	{
		.callback           = twi_mngr_callback,
		.p_user_data        = (void *)p_tlv320aic3106,
		.event_type         = evt_type,
		.transfer           = DK_TWI_MNGR_TX_RX(p_tlv320aic3106->i2c_address,
		                                        &p_twi_read->reg_address,
		                                        sizeof(p_twi_read->reg_address),
		                                        p_twi_read->data,
		                                        read_size-1,
		                                        0)
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

static ret_code_t twi_write_blocking(tlv320aic3106_t     const * p_tlv320aic3106,
                                     tlv320aic3106_twi_write_t * p_twi_write,
                                     uint8_t                     write_size)
{
	dk_twi_mngr_transfer_t twi_transfer = DK_TWI_MNGR_TX(p_tlv320aic3106->i2c_address,
	                                                     p_twi_write,
	                                                     write_size,
	                                                     0);

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

static uint16_t pll_d_encode(uint16_t d_value)
{
	uint16_t encoded_d = uint16_big_decode((uint8_t *)&d_value);
	encoded_d <<= 2; // Shift by two bits

	return encoded_d;
}

ret_code_t tlv320aic3106_init(tlv320aic3106_t * p_tlv320aic3106,
                              tlv320aic3106_evt_handler_t evt_handler)
{
	NRF_LOG_INFO("Initializing TLV320AIC3106");

	VERIFY_PARAM_NOT_NULL(evt_handler);
	p_tlv320aic3106->evt_handler = evt_handler;

	return tlv320aic3106_soft_rst(p_tlv320aic3106);
}

ret_code_t tlv320aic3106_soft_rst(tlv320aic3106_t * p_tlv320aic3106)
{
	ret_code_t err_code;

	tlv320aic3106_twi_write_t twi_write =
	{
		.reg_address = TLV320AIC3106_SOFT_RST,
		.data[0]     = TLV320AIC3106_SOFT_RST_VAL
	};

	err_code = twi_write_blocking(p_tlv320aic3106,
	                              &twi_write,
	                              2);

	VERIFY_SUCCESS(err_code);

	memset(&p_tlv320aic3106->config.pll_prog_reg_a,
	        TLV320AIC3106_RST_VAL_PLL_PROG_REG_A, 
	        sizeof(p_tlv320aic3106->config.pll_prog_reg_a));

	memset(&p_tlv320aic3106->config.gpio_ctrl_b,
	        TLV320AIC3106_RST_VAL_GPIO_CTRL_B,
	        sizeof(p_tlv320aic3106->config.gpio_ctrl_b));

	memset(&p_tlv320aic3106->config.passive_ana_sig_bypass_sel_pd,
	        TLV320AIC3106_RST_VAL_PASSIVE_ANA_SIG_BYPASS_SEL_PD,
	        sizeof(p_tlv320aic3106->config.passive_ana_sig_bypass_sel_pd));

	memset(&p_tlv320aic3106->config.ac_pwr_and_out_drv_ctrl,
	        TLV320AIC3106_RST_VAL_AC_PWR_AND_OUT_DRV_CTRL,
	        sizeof(p_tlv320aic3106->config.ac_pwr_and_out_drv_ctrl));

	memset(&p_tlv320aic3106->config.dac_dig_volume_ctrl,
	        TLV320AIC3106_RST_VAL_DAC_DIG_VOLUME_CTRL,
	        sizeof(p_tlv320aic3106->config.dac_dig_volume_ctrl));

	memset(&p_tlv320aic3106->config.dac_r1_to_right_lop,
	        TLV320AIC3106_RST_VAL_DAC_R1_TO_RIGHT_LOP,
	        sizeof(p_tlv320aic3106->config.dac_r1_to_right_lop));

	memset(&p_tlv320aic3106->config.dac_l1_to_left_lop,
	        TLV320AIC3106_RST_VAL_DAC_L1_TO_LEFT_LOP,
	        sizeof(p_tlv320aic3106->config.dac_l1_to_left_lop));

	memset(&p_tlv320aic3106->config.right_lop_m_out_lvl_ctrl,
	        TLV320AIC3106_RST_VAL_RIGHT_LOP_M_OUT_LVL_CTRL,
	        sizeof(p_tlv320aic3106->config.right_lop_m_out_lvl_ctrl));

	memset(&p_tlv320aic3106->config.left_lop_m_out_lvl_ctrl,
	        TLV320AIC3106_RST_VAL_LEFT_LOP_M_OUT_LVL_CTRL,
	        sizeof(p_tlv320aic3106->config.left_lop_m_out_lvl_ctrl));

	p_tlv320aic3106->active_page = TLV320AIC3106_ACTIVE_PAGE_0;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_passive_ana_sig_bypass_sel_pd(tlv320aic3106_t * p_tlv320aic3106,
                                                           tlv320aic3106_passive_ana_sig_bypass_sel_pd_t * p_bypass)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_bypass_cmd, sizeof(tlv320aic3106_passive_ana_sig_bypass_sel_pd_t));
	p_bypass_cmd->reg_address = TLV320AIC3106_PASSIVE_ANA_SIG_BYPASS_SEL_PD;
	memcpy(p_bypass_cmd->data, p_bypass, sizeof(tlv320aic3106_passive_ana_sig_bypass_sel_pd_t));

	err_code = twi_write(p_tlv320aic3106, p_bypass_cmd, p_bypass_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.passive_ana_sig_bypass_sel_pd = *p_bypass;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_line1_bypass(tlv320aic3106_t * p_tlv320aic3106,
                                          bool enable)
{
	tlv320aic3106_passive_ana_sig_bypass_sel_pd_t ana_sig_bypass = p_tlv320aic3106->config.passive_ana_sig_bypass_sel_pd;

	ana_sig_bypass.line1lp_routed_to_left_lop  = enable;
	ana_sig_bypass.line1lm_routed_to_left_lom  = enable;
	ana_sig_bypass.line1rp_routed_to_right_lop = enable;
	ana_sig_bypass.line1rm_routed_to_right_lom = enable;

	return tlv320aic3106_set_passive_ana_sig_bypass_sel_pd(p_tlv320aic3106, &ana_sig_bypass);
}

ret_code_t tlv320aic3106_pll_init(tlv320aic3106_t * p_tlv320aic3106,
                                  tlv320aic3106_pll_config_t * p_pll_config)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_pll_prog_t));
	p_cmd->reg_address = TLV320AIC3106_PLL_PROG_REG_A;
	tlv320aic3106_pll_prog_t * p_pll_prog = (tlv320aic3106_pll_prog_t *)p_cmd->data;
	memset(p_pll_prog, 0, sizeof(tlv320aic3106_pll_prog_t));

	p_pll_prog->reg_a.pll_enabled = p_pll_config->enable;
	p_pll_prog->reg_a.pll_p       = p_pll_config->p;
	p_pll_prog->reg_a.pll_q       = p_pll_config->q;

	p_pll_prog->reg_b.pll_j       = p_pll_config->j;

	p_pll_prog->val_d             = pll_d_encode(p_pll_config->d);

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.pll_prog_reg_a = p_pll_prog->reg_a;

	return tlv320aic3106_set_pll_r(p_tlv320aic3106, p_pll_config->r);
}

ret_code_t tlv320aic3106_pll_enable(tlv320aic3106_t * p_tlv320aic3106,
                                    bool enable)
{
	tlv320aic3106_pll_prog_reg_a_t pll_prog_reg_a = p_tlv320aic3106->config.pll_prog_reg_a;
	pll_prog_reg_a.pll_enabled = enable;

	return tlv320aic3106_set_pll_prog_reg_a(p_tlv320aic3106, &pll_prog_reg_a);
}

ret_code_t tlv320aic3106_set_pll_prog_reg_a(tlv320aic3106_t * p_tlv320aic3106,
                                            tlv320aic3106_pll_prog_reg_a_t * p_pll_prog_reg_a)
{
	ret_code_t err_code;
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_pll_prog_reg_a_t));
	p_cmd->reg_address = TLV320AIC3106_PLL_PROG_REG_A;
	memcpy(p_cmd->data, p_pll_prog_reg_a, sizeof(tlv320aic3106_pll_prog_reg_a_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.pll_prog_reg_a = *p_pll_prog_reg_a;

	return NRF_SUCCESS;
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

	uint16_t encoded_d = pll_d_encode(d_value);

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

ret_code_t tlv320aic3106_set_audio_ser_data_interface_ctrl_c(tlv320aic3106_t * p_tlv320aic3106,
                                                             uint8_t offset)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(offset));
	p_cmd->reg_address = TLV320AIC3106_AUDIO_SER_DATA_INTERFACE_CTRL_C;
	memcpy(p_cmd->data, &offset, sizeof(offset));

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

ret_code_t tlv320aic3106_set_digital_filter_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                 tlv320aic3106_audio_codec_digital_filter_ctrl_t * p_dig_filter_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_audio_codec_digital_filter_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_AUDIO_CODEC_DIGITAL_FILTER_CTRL;
	memcpy(p_cmd->data, p_dig_filter_ctrl, sizeof(tlv320aic3106_audio_codec_digital_filter_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_headset_btn_press_detect_b(tlv320aic3106_t * p_tlv320aic3106,
                                                        tlv320aic3106_headset_btn_press_detect_b_t * p_headset_btn_press_detect_b)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_headset_btn_press_detect_b_t));
	p_cmd->reg_address = TLV320AIC3106_HEADSET_BTN_PRESS_DETECT_B;
	memcpy(p_cmd->data, p_headset_btn_press_detect_b, sizeof(tlv320aic3106_headset_btn_press_detect_b_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_ac_pwr_and_out_drv_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                     tlv320aic3106_ac_pwr_and_out_drv_ctrl_t * p_ac_pwr_and_out_drv_ctrl)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_ac_pwr_and_out_drv_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_AC_PWR_AND_OUT_DRV_CTRL;
	memcpy(p_cmd->data, p_ac_pwr_and_out_drv_ctrl, sizeof(tlv320aic3106_ac_pwr_and_out_drv_ctrl_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.ac_pwr_and_out_drv_ctrl = *p_ac_pwr_and_out_drv_ctrl;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_dac_pwr(tlv320aic3106_t * p_tlv320aic3106,
                                     bool right_dac_en,
                                     bool left_dac_en)
{
	tlv320aic3106_ac_pwr_and_out_drv_ctrl_t ac_pwr_and_out_drv_ctrl = p_tlv320aic3106->config.ac_pwr_and_out_drv_ctrl;

	ac_pwr_and_out_drv_ctrl.right_dac_powered_up = right_dac_en;
	ac_pwr_and_out_drv_ctrl.left_dac_powered_up = left_dac_en;

	return tlv320aic3106_set_ac_pwr_and_out_drv_ctrl(p_tlv320aic3106, &ac_pwr_and_out_drv_ctrl);
}

ret_code_t tlv320aic3106_set_hi_pwr_out_stage_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                     tlv320aic3106_hi_pwr_out_stage_ctrl_t * p_hi_pwr_out_stage_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_hi_pwr_out_stage_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_HI_PWR_OUT_STAGE_CTRL;
	memcpy(p_cmd->data, p_hi_pwr_out_stage_ctrl, sizeof(tlv320aic3106_hi_pwr_out_stage_ctrl_t));

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

ret_code_t tlv320aic3106_set_out_drv_pop_reduction(tlv320aic3106_t * p_tlv320aic3106,
                                                   tlv320aic3106_out_drv_pop_reduction_t * p_out_drv_pop_reduction)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_out_drv_pop_reduction_t));
	p_cmd->reg_address = TLV320AIC3106_OUT_DRV_POP_REDUCTION;
	memcpy(p_cmd->data, p_out_drv_pop_reduction, sizeof(tlv320aic3106_out_drv_pop_reduction_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_dac_dig_volume_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                      tlv320aic3106_dac_dig_volume_ctrl_t * p_dac_dig_volume_ctrl)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_dac_dig_volume_ctrl_t) * 2);
	p_cmd->reg_address = TLV320AIC3106_LEFT_DAC_DIG_VOLUME_CTRL;
	memcpy(&p_cmd->data[0], p_dac_dig_volume_ctrl, sizeof(tlv320aic3106_dac_dig_volume_ctrl_t));
	memcpy(&p_cmd->data[1], p_dac_dig_volume_ctrl, sizeof(tlv320aic3106_dac_dig_volume_ctrl_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.dac_dig_volume_ctrl = *p_dac_dig_volume_ctrl;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_dac_mute(tlv320aic3106_t * p_tlv320aic3106,
                                      bool mute)
{
	tlv320aic3106_dac_dig_volume_ctrl_t dac_dig_volume_ctrl = p_tlv320aic3106->config.dac_dig_volume_ctrl;

	dac_dig_volume_ctrl.dac_muted = mute;

	return tlv320aic3106_set_dac_dig_volume_ctrl(p_tlv320aic3106, &dac_dig_volume_ctrl);
}

ret_code_t tlv320aic3106_set_left_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                     tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_LEFT_LOP_M_OUT_LVL_CTRL;
	memcpy(p_cmd->data, p_out_lvl_ctrl, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.left_lop_m_out_lvl_ctrl = *p_out_lvl_ctrl;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl)
{
	ret_code_t err_code;

	err_code = tlv320aic3106_set_left_lop_m_out_lvl_ctrl(p_tlv320aic3106, p_out_lvl_ctrl);
	VERIFY_SUCCESS(err_code);

	err_code = tlv320aic3106_set_right_lop_m_out_lvl_ctrl(p_tlv320aic3106, p_out_lvl_ctrl);
	VERIFY_SUCCESS(err_code);

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_left_lop_m_out_mute(tlv320aic3106_t * p_tlv320aic3106,
                                                  bool mute)
{
	tlv320aic3106_x_out_lvl_ctrl_t left_lop_out_lvl_ctrl = p_tlv320aic3106->config.left_lop_m_out_lvl_ctrl;

	left_lop_out_lvl_ctrl.not_muted = !mute;

	return tlv320aic3106_set_left_lop_m_out_lvl_ctrl(p_tlv320aic3106, &left_lop_out_lvl_ctrl);
}

ret_code_t tlv320aic3106_set_left_lop_m_out_pwr_en(tlv320aic3106_t * p_tlv320aic3106,
                                                  bool pwr_en)
{
	tlv320aic3106_x_out_lvl_ctrl_t left_lop_out_lvl_ctrl = p_tlv320aic3106->config.left_lop_m_out_lvl_ctrl;

	left_lop_out_lvl_ctrl.power_en = pwr_en;

	return tlv320aic3106_set_left_lop_m_out_lvl_ctrl(p_tlv320aic3106, &left_lop_out_lvl_ctrl);
}

ret_code_t tlv320aic3106_set_dac_r1_to_right_lop(tlv320aic3106_t * p_tlv320aic3106,
                                                 tlv320aic3106_x_to_y_volume_ctrl_t * p_dac_r1_to_right_lop)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_x_to_y_volume_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_DAC_R1_TO_RIGHT_LOP_M_VOLUME_CTRL;
	memcpy(p_cmd->data, p_dac_r1_to_right_lop, sizeof(tlv320aic3106_x_to_y_volume_ctrl_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.dac_r1_to_right_lop = *p_dac_r1_to_right_lop;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_dac_l1_to_left_lop(tlv320aic3106_t * p_tlv320aic3106,
                                                tlv320aic3106_x_to_y_volume_ctrl_t * p_dac_l1_to_left_lop)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_x_to_y_volume_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_DAC_L1_TO_LEFT_LOP_M_VOLUME_CTRL;
	memcpy(p_cmd->data, p_dac_l1_to_left_lop, sizeof(tlv320aic3106_x_to_y_volume_ctrl_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.dac_l1_to_left_lop = *p_dac_l1_to_left_lop;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_dac_r1_to_right_lop_volume(tlv320aic3106_t * p_tlv320aic3106,
                                                        uint8_t volume)
{
	tlv320aic3106_x_to_y_volume_ctrl_t dac_r1_to_right_lop = p_tlv320aic3106->config.dac_r1_to_right_lop;

	dac_r1_to_right_lop.x_to_y_ana_volume_ctrl = volume;

	return tlv320aic3106_set_dac_r1_to_right_lop(p_tlv320aic3106, &dac_r1_to_right_lop);
}

ret_code_t tlv320aic3106_set_dac_l1_to_left_lop_volume(tlv320aic3106_t * p_tlv320aic3106,
                                                       uint8_t volume)
{
	tlv320aic3106_x_to_y_volume_ctrl_t dac_l1_to_left_lop = p_tlv320aic3106->config.dac_l1_to_left_lop;

	dac_l1_to_left_lop.x_to_y_ana_volume_ctrl = volume;

	return tlv320aic3106_set_dac_l1_to_left_lop(p_tlv320aic3106, &dac_l1_to_left_lop);
}

ret_code_t tlv320aic3106_set_dac_x1_to_lop(tlv320aic3106_t * p_tlv320aic3106,
                                        tlv320aic3106_x_to_y_volume_ctrl_t * p_dac_to_lop)
{
	ret_code_t err_code;

	err_code = tlv320aic3106_set_dac_r1_to_right_lop(p_tlv320aic3106, p_dac_to_lop);
	VERIFY_SUCCESS(err_code);

	err_code = tlv320aic3106_set_dac_l1_to_left_lop(p_tlv320aic3106, p_dac_to_lop);
	VERIFY_SUCCESS(err_code);

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_right_lop_m_out_lvl_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                                      tlv320aic3106_x_out_lvl_ctrl_t * p_out_lvl_ctrl)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_RIGHT_LOP_M_OUT_LVL_CTRL;
	memcpy(p_cmd->data, p_out_lvl_ctrl, sizeof(tlv320aic3106_x_out_lvl_ctrl_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.right_lop_m_out_lvl_ctrl = *p_out_lvl_ctrl;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_right_lop_m_out_mute(tlv320aic3106_t * p_tlv320aic3106,
                                                  bool mute)
{
	tlv320aic3106_x_out_lvl_ctrl_t right_lop_out_lvl_ctrl = p_tlv320aic3106->config.right_lop_m_out_lvl_ctrl;

	right_lop_out_lvl_ctrl.not_muted = !mute;

	return tlv320aic3106_set_right_lop_m_out_lvl_ctrl(p_tlv320aic3106, &right_lop_out_lvl_ctrl);
}

ret_code_t tlv320aic3106_set_right_lop_m_out_pwr_en(tlv320aic3106_t * p_tlv320aic3106,
                                                    bool pwr_en)
{
	tlv320aic3106_x_out_lvl_ctrl_t right_lop_out_lvl_ctrl = p_tlv320aic3106->config.right_lop_m_out_lvl_ctrl;

	right_lop_out_lvl_ctrl.power_en = pwr_en;

	return tlv320aic3106_set_right_lop_m_out_lvl_ctrl(p_tlv320aic3106, &right_lop_out_lvl_ctrl);
}

ret_code_t tlv320aic3106_set_gpio_ctrl_b(tlv320aic3106_t * p_tlv320aic3106,
                                         tlv320aic3106_gpio_ctrl_b_t * p_gpio_ctrl_b)
{
	ret_code_t err_code;

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_gpio_ctrl_b_t));
	p_cmd->reg_address = TLV320AIC3106_GPIO_CTRL_B;
	memcpy(p_cmd->data, p_gpio_ctrl_b, sizeof(tlv320aic3106_gpio_ctrl_b_t));

	err_code = twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
	VERIFY_SUCCESS(err_code);

	p_tlv320aic3106->config.gpio_ctrl_b = *p_gpio_ctrl_b;

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_set_clkin_src(tlv320aic3106_t * p_tlv320aic3106,
                                       tlv320aic3106_codec_clkin_src_t clkin_src)
{
	tlv320aic3106_gpio_ctrl_b_t gpio_ctrl_b = p_tlv320aic3106->config.gpio_ctrl_b;

	gpio_ctrl_b.codec_clkin_src = clkin_src;

	return tlv320aic3106_set_gpio_ctrl_b(p_tlv320aic3106, &gpio_ctrl_b);
}

ret_code_t tlv320aic3106_set_clk_gen_ctrl(tlv320aic3106_t * p_tlv320aic3106,
                                          tlv320aic3106_clk_gen_ctrl_t * p_clk_gen_ctrl)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_clk_gen_ctrl_t));
	p_cmd->reg_address = TLV320AIC3106_CLK_GEN_CTRL;
	memcpy(p_cmd->data, p_clk_gen_ctrl, sizeof(tlv320aic3106_clk_gen_ctrl_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_set_dac_quiescient_current(tlv320aic3106_t * p_tlv320aic3106,
                                                    tlv320aic3106_dac_quiescent_current_adj_t * p_dac_quiscient)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_cmd, sizeof(tlv320aic3106_dac_quiescent_current_adj_t));
	p_cmd->reg_address = TLV320AIC3106_DAC_QUIESCENT_CURRENT_ADJ;
	memcpy(p_cmd->data, p_dac_quiscient, sizeof(tlv320aic3106_dac_quiescent_current_adj_t));

	return twi_write(p_tlv320aic3106, p_cmd, p_cmd_size);
}

ret_code_t tlv320aic3106_debug(tlv320aic3106_t * p_tlv320aic3106)
{
	ret_code_t err_code;
	uint8_t data[107];

	err_code = twi_read_blocking(p_tlv320aic3106, TLV320AIC3106_PLL_PROG_REG_A, data, sizeof(data));
	VERIFY_SUCCESS(err_code);

	for(uint8_t i = 0; i < sizeof(data); i++)
	{
		NRF_LOG_INFO("%u 0x%x", i+3, data[i]);
		NRF_LOG_PROCESS();
	}

	return NRF_SUCCESS;
}

ret_code_t tlv320aic3106_get_module_power_status(tlv320aic3106_t * p_tlv320aic3106)
{
	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_read_t, p_obj_read, sizeof(tlv320aic3106_module_pwr_status_t));

	p_obj_read->reg_address = TLV320AIC3106_MODULE_PWR_STATUS;

	return twi_read(p_tlv320aic3106, p_obj_read, p_obj_read_size, TLV320AIC3106_EVT_TYPE_RX_MODULE_PWR_STATUS);
}
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

#include "nrf_delay.h"

#define NRF_LOG_MODULE_NAME TLV320AIC3106
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
	ret_code_t err_code;
	tlv320aic3106_passive_ana_sig_bypass_sel_pd_t bypass =
	{
		.line1lp_routed_to_left_lop  = true,
		.line1lm_routed_to_left_lom  = true,
		.line2lp_routed_to_left_lop  = false,
		.line2lm_routed_to_left_lom  = false,
		.line1rp_routed_to_right_lop = true,
		.line1rm_routed_to_right_lom = true,
		.line2rp_routed_to_right_lop = false,
		.line2rm_routed_to_right_lom = false
	};

	NRF_LOG_INFO("Initializing TLV320AIC3106");

	p_tlv320aic3106->error_handler = error_handler;

	err_code = tlv320aic3106_soft_rst(p_tlv320aic3106);
	VERIFY_SUCCESS(err_code);

	DK_TWI_MNGR_BUFF_ALLOC(tlv320aic3106_twi_write_t, p_bypass, sizeof(bypass));
	p_bypass->reg_address = TLV320AIC3106_PASSIVE_ANA_SIG_BYPASS_SEL_PD;
	memcpy(p_bypass->data, &bypass, sizeof(p_bypass->data));

	return twi_write(p_tlv320aic3106,
	                 p_bypass,
	                 p_bypass_size);
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

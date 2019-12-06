/**
 * @file        dk_twi_mngr.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TWI manager that implements a FIFO buffer on top on nrf_twi_mngr.
 * @version     0.1
 * @date        2019-12-05
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_twi_mngr.h"

#include "dk_lib_common.h"
#if DK_MODULE_ENABLED(DK_TWI_MNGR)

#define NRF_LOG_MODULE_NAME DK_TWI_MNGR
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
NRF_LOG_MODULE_REGISTER();

#if !NRF_MODULE_ENABLED(NRF_TWI_MNGR)
#error "Enable NRF_TWI_MNGR module in sdk_config!"
#endif

static void twi_mngr_cb_handler(ret_code_t result, void * p_user_data)
{
	NRF_LOG_INFO("CB");
	NRF_LOG_FLUSH();
}

ret_code_t dk_twi_mngr_init(dk_twi_mngr_t   const * p_dk_twi_mngr,
                            uint32_t        scl_pin,
                            uint32_t        sda_pin)
{
	VERIFY_PARAM_NOT_NULL(p_dk_twi_mngr);
	VERIFY_PARAM_NOT_NULL(p_dk_twi_mngr->p_rw_buf_queue);
	VERIFY_PARAM_NOT_NULL(p_dk_twi_mngr->p_transfer_queue);

	if(!p_dk_twi_mngr->p_rw_buf_queue->size ||
	   !p_dk_twi_mngr->p_transfer_queue->size)
	{
		return NRF_ERROR_NOT_SUPPORTED;
	}

	nrf_drv_twi_config_t default_twi_config = NRF_DRV_TWI_DEFAULT_CONFIG;
	default_twi_config.scl = scl_pin;
	default_twi_config.sda = sda_pin;

	return nrf_twi_mngr_init(p_dk_twi_mngr->p_nrf_twi_mngr,
	                         &default_twi_config);
}

void dk_twi_mngr_uninit(dk_twi_mngr_t const * p_dk_twi_mngr)
{
	if(p_dk_twi_mngr == NULL) return;
	
	nrf_twi_mngr_uninit(p_dk_twi_mngr->p_nrf_twi_mngr);
}

ret_code_t dk_twi_mngr_schedule(dk_twi_mngr_t             const * p_dk_twi_mngr,
                                dk_twi_mngr_transaction_t const * p_transaction)
{
	VERIFY_PARAM_NOT_NULL(p_dk_twi_mngr);
	VERIFY_PARAM_NOT_NULL(p_transaction);

	ret_code_t  err_code;
	size_t      write_pos;

	write_pos = p_dk_twi_mngr->p_rw_buf_queue->p_cb->back;

	err_code = nrf_queue_push(p_dk_twi_mngr->p_rw_buf_queue, p_transaction->p_data);
	VERIFY_SUCCESS(err_code);

	nrf_twi_mngr_transfer_t twi_transfer =
	    NRF_TWI_MNGR_WRITE
	    (
	        p_transaction->i2c_address,
	        &((uint8_t *)p_dk_twi_mngr->p_rw_buf_queue->p_buffer)[write_pos],
	        p_transaction->data_length,
	        0
	    );
	
	write_pos = p_dk_twi_mngr->p_transfer_queue->p_cb->back;

	err_code = nrf_queue_push(p_dk_twi_mngr->p_transfer_queue, &twi_transfer);
	VERIFY_SUCCESS(err_code);

	nrf_twi_mngr_transaction_t twi_transaction =
	{
		.callback            = twi_mngr_cb_handler,
		.p_user_data         = p_transaction->p_user_data,
		.p_transfers         = (nrf_twi_mngr_transfer_t *)&((uint8_t *)p_dk_twi_mngr->p_transfer_queue->p_buffer)[write_pos],
		.number_of_transfers = 1,
		.p_required_twi_cfg  = NULL
	};
	
	write_pos = p_dk_twi_mngr->p_transaction_queue->p_cb->back;

	err_code = nrf_queue_push(p_dk_twi_mngr->p_transaction_queue, &twi_transaction);
	VERIFY_SUCCESS(err_code);

	return nrf_twi_mngr_schedule(p_dk_twi_mngr->p_nrf_twi_mngr, (nrf_twi_mngr_transaction_t *)&((uint8_t *)p_dk_twi_mngr->p_transaction_queue->p_buffer)[write_pos]);
}

#endif

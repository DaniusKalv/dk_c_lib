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

#include "dk_lib_common.h"
#if DK_MODULE_ENABLED(DK_TWI_MNGR)

#include "dk_twi_mngr.h"

#define NRF_LOG_MODULE_NAME DK_TWI_MNGR
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
NRF_LOG_MODULE_REGISTER();

#if !NRF_MODULE_ENABLED(NRF_TWI_MNGR)
#error "Enable NRF_TWI_MNGR module in sdk_config!"
#endif

typedef volatile struct
{
	bool    transaction_in_progress;
	uint8_t transaction_result;
} dk_twi_mngr_cb_data_t;

static ret_code_t start_transfer(dk_twi_mngr_t const * p_dk_twi_mngr)
{
	ASSERT(p_dk_twi_mngr != NULL);

	// Pointer for cleaner code.
	dk_twi_mngr_cb_t * p_cb = p_dk_twi_mngr->p_dk_twi_mngr_cb;

	// [use a local variable to avoid using two volatile variables in one
	//  expression]
	dk_twi_mngr_transfer_t transfer = p_cb->current_transaction.transfer;

	return nrfx_twi_xfer(&p_dk_twi_mngr->twi, &transfer.transfer_description, transfer.flags);
}

static void transaction_end_signal(dk_twi_mngr_t const * p_dk_twi_mngr,
                                   ret_code_t            result)
{
	ASSERT(p_dk_twi_mngr != NULL);

	if (p_dk_twi_mngr->p_dk_twi_mngr_cb->current_transaction.callback)
	{
		// [use a local variable to avoid using two volatile variables in one
		//  expression]
		void * p_user_data = p_dk_twi_mngr->p_dk_twi_mngr_cb->current_transaction.p_user_data;
		p_dk_twi_mngr->p_dk_twi_mngr_cb->current_transaction.callback(result, p_user_data);
	}

	nrf_free(p_dk_twi_mngr->p_dk_twi_mngr_cb->current_transaction.transfer.transfer_description.p_primary_buf);

	if(p_dk_twi_mngr->p_dk_twi_mngr_cb->current_transaction.transfer.transfer_description.p_secondary_buf != NULL)
	{
		nrf_free(p_dk_twi_mngr->p_dk_twi_mngr_cb->current_transaction.transfer.transfer_description.p_secondary_buf);
	}
}

static void start_pending_transaction(dk_twi_mngr_t const * p_dk_twi_mngr,
                                      bool                  switch_transaction)
{
	ASSERT(p_dk_twi_mngr != NULL);

	// Pointer for cleaner code.
	dk_twi_mngr_cb_t * p_cb = p_dk_twi_mngr->p_dk_twi_mngr_cb;

	for (;;)
	{
		bool start_transaction = false;

		CRITICAL_REGION_ENTER();
		if (switch_transaction || dk_twi_mngr_is_idle(p_dk_twi_mngr))
		{
			if (nrf_queue_pop(p_dk_twi_mngr->p_queue, (void *)(&p_cb->current_transaction))
			    == NRF_SUCCESS)
			{
				start_transaction = true;
				p_cb->transaction_in_progress = true;
			}
			else
			{
				p_cb->transaction_in_progress = false;
			}
		}
		CRITICAL_REGION_EXIT();

		if (!start_transaction)
		{
			return;
		}
		else
		{
			ret_code_t result;

			// Try to start first transfer for this new transaction.
			result = start_transfer(p_dk_twi_mngr);

			// If transaction started successfully there is nothing more to do here now.
			if (result == NRF_SUCCESS)
			{
				return;
			}

			NRF_LOG_ERROR("Failed to start transaction 0x%x", result);

			// Transfer failed to start - notify user that this transaction
			// cannot be started and try with next one (in next iteration of
			// the loop).
			transaction_end_signal(p_dk_twi_mngr, result);

			switch_transaction = true;
		}
	}
}

static void twi_event_handler(nrfx_twi_evt_t const * p_event,
                              void *                 p_context)
{
	ASSERT(p_event != NULL);

	dk_twi_mngr_t * p_dk_twi_mngr = (dk_twi_mngr_t *)p_context;
	ret_code_t result;

	// This callback should be called only during transaction.
	ASSERT(p_dk_twi_mngr->p_dk_twi_mngr_cb->transaction_in_progress);

	if (p_event->type == NRFX_TWI_EVT_DONE)
	{
		result = NRF_SUCCESS;
	}
	else
	{
		NRF_LOG_ERROR("0x%x", p_event->type);
		result = NRF_ERROR_INTERNAL;
	}

	// The current transaction has been completed or interrupted by some error.
	// Notify the user and start next one (if there is any).
	transaction_end_signal(p_dk_twi_mngr, result);
	start_pending_transaction(p_dk_twi_mngr, true);
}

ret_code_t dk_twi_mngr_init(dk_twi_mngr_t       const * p_dk_twi_mngr,
                            nrfx_twi_config_t   const * p_default_twi_config)
{
	ASSERT(p_dk_twi_mngr != NULL);
	ASSERT(p_dk_twi_mngr->p_queue != NULL);
	ASSERT(p_dk_twi_mngr->p_queue->size > 0);
	ASSERT(p_default_twi_config != NULL);

	ret_code_t err_code;

	err_code = nrf_mem_init();
	VERIFY_SUCCESS(err_code);

	err_code = nrfx_twi_init(&p_dk_twi_mngr->twi,
	                         p_default_twi_config,
	                         twi_event_handler,
	                         (void *)p_dk_twi_mngr);
	VERIFY_SUCCESS(err_code);

	nrfx_twi_enable(&p_dk_twi_mngr->twi);

	p_dk_twi_mngr->p_dk_twi_mngr_cb->transaction_in_progress = false;

	return NRF_SUCCESS;
}

void dk_twi_mngr_uninit(dk_twi_mngr_t const * p_dk_twi_mngr)
{
	ASSERT(p_dk_twi_mngr != NULL);

	nrfx_twi_uninit(&p_dk_twi_mngr->twi);

	p_dk_twi_mngr->p_dk_twi_mngr_cb->transaction_in_progress = false;
}

ret_code_t dk_twi_mngr_schedule(dk_twi_mngr_t             const * p_dk_twi_mngr,
                                dk_twi_mngr_transaction_t const * p_transaction)
{
	ASSERT(p_dk_twi_mngr != NULL);
	ASSERT(p_transaction != NULL);

	ret_code_t result = NRF_SUCCESS;

	result = nrf_queue_push(p_dk_twi_mngr->p_queue, (void *)p_transaction);
	if (result == NRF_SUCCESS)
	{
		// New transaction has been successfully added to queue,
		// so if we are currently idle it's time to start the job.
		start_pending_transaction(p_dk_twi_mngr, false);
	}

	return result;
}

static void internal_transaction_cb(ret_code_t result, void * p_user_data)
{
	dk_twi_mngr_cb_data_t *p_cb_data = (dk_twi_mngr_cb_data_t *)p_user_data;

	p_cb_data->transaction_result      = result;
	p_cb_data->transaction_in_progress = false;
}

ret_code_t dk_twi_mngr_perform(dk_twi_mngr_t          const * p_dk_twi_mngr,
                               dk_twi_mngr_transfer_t const * p_transfer,
                               void                           (* user_function)(void))
{
	ASSERT(p_dk_twi_mngr != NULL);
	ASSERT(p_transfer != NULL);

	dk_twi_mngr_cb_data_t cb_data =
	{
		.transaction_in_progress = true
	};

	dk_twi_mngr_transaction_t internal_transaction =
	{
		.callback            = internal_transaction_cb,
		.p_user_data         = (void *)&cb_data,
		.transfer            = *p_transfer
	};

	ret_code_t result = dk_twi_mngr_schedule(p_dk_twi_mngr, &internal_transaction);
	VERIFY_SUCCESS(result);

	while (cb_data.transaction_in_progress)
	{
		if (user_function)
		{
			user_function();
		}
	}

	return cb_data.transaction_result;
}

#endif // DK_MODULE_ENABLED(DK_TWI_MNGR)

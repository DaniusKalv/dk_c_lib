/**
 * @file        dk_twi_mngr.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TWI manager that implements a queue buffer on top of nrf_twi_mngr.
 * @version     0.1
 * @date        2019-12-05
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_TWI_MNGR_H
#define DK_TWI_MNGR_H

#include "nrf_twi_mngr.h"
#include "nrf_queue.h"

typedef void (* dk_twi_mngr_callback_t)(ret_code_t result, void * p_user_data);

typedef struct
{
	nrf_twi_mngr_transfer_t transfer;
	nrf_twi_mngr_callback_t callback;
} dk_twi_mngr_transfer_t;

typedef struct
{
	uint8_t                 i2c_address;
	uint8_t               * p_data;
	uint8_t                 data_length;
	nrf_twi_mngr_callback_t callback;
	void                  * p_user_data;
} dk_twi_mngr_transaction_t;

typedef struct
{
	nrf_twi_mngr_t  const * p_nrf_twi_mngr;
	nrf_queue_t     const * p_rw_buf_queue;
	nrf_queue_t     const * p_transfer_queue;
	nrf_queue_t     const * p_transaction_queue;
} dk_twi_mngr_t;

#define DK_TWI_MNGR_DEF(_dk_twi_mngr_name, _rw_buf_type, _queue_size, _twi_idx) \
	NRF_TWI_MNGR_DEF(_dk_twi_mngr_name##_nrf,                                   \
	                 (_queue_size),                                             \
	                 _twi_idx);                                                 \
	NRF_QUEUE_DEF(_rw_buf_type,                                                 \
	              _dk_twi_mngr_name##_rw_buf_queue,                             \
	              (_queue_size),                                                \
	              NRF_QUEUE_MODE_NO_OVERFLOW);                                  \
	NRF_QUEUE_DEF(dk_twi_mngr_transfer_t,                                       \
	              _dk_twi_mngr_name##_transfer_queue,                           \
	              (_queue_size),                                                \
	              NRF_QUEUE_MODE_NO_OVERFLOW);                                  \
	NRF_QUEUE_DEF(dk_twi_mngr_transaction_t,                                    \
	              _dk_twi_mngr_name##_transaction_queue,                        \
	              (_queue_size),                                                \
	              NRF_QUEUE_MODE_NO_OVERFLOW);                                  \
	static const dk_twi_mngr_t _dk_twi_mngr_name =                              \
	{                                                                           \
		.p_nrf_twi_mngr         = &_dk_twi_mngr_name##_nrf,                     \
		.p_rw_buf_queue         = &_dk_twi_mngr_name##_rw_buf_queue,            \
		.p_transfer_queue       = &_dk_twi_mngr_name##_transfer_queue,          \
		.p_transaction_queue    = &_dk_twi_mngr_name##_transaction_queue        \
	}

ret_code_t dk_twi_mngr_init(dk_twi_mngr_t   const * p_dk_twi_mngr,
                            uint32_t        scl_pin,
                            uint32_t        sda_pin);

void dk_twi_mngr_uninit(dk_twi_mngr_t const * p_dk_twi_mngr);

ret_code_t dk_twi_mngr_schedule(dk_twi_mngr_t             const * p_dk_twi_mngr,
                                dk_twi_mngr_transaction_t const * p_transaction);

#endif

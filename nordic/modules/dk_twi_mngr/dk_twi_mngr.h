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

#include "nrfx_twi.h"
#include "nrf_queue.h"
#include "mem_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

//If TWIM is present buffers can only be in RAM
/*lint -save -e491*/

/**
 * @brief Macro checking if buffers should be stored in RAM.
 */
#ifndef DK_TWI_MNGR_BUFFERS_IN_RAM
	#define DK_TWI_MNGR_BUFFERS_IN_RAM defined(TWIM_PRESENT)
#endif

#if DK_TWI_MNGR_BUFFERS_IN_RAM
	#define DK_TWI_MNGR_BUFFER_LOC_IND
#else
	#define DK_TWI_MNGR_BUFFER_LOC_IND const
#endif

#define DK_TWI_MNGR_BUFF_CHECK(_buff_name)                                      \
	if(_buff_name == NULL) return NRF_ERROR_NO_MEM

#define DK_TWI_MNGR_BUFF_ALLOC(_type, _name, _data_size)                            \
	const uint32_t CONCAT_2(_name, _size) = _data_size + 1;                         \
	_type * _name = (_type *)dk_twi_mngr_data_buffer_alloc(CONCAT_2(_name, _size)); \
	DK_TWI_MNGR_BUFF_CHECK(_name)

#define DK_TWI_MNGR_TX(address, p_data, length, _flags)                         \
{                                                                               \
	.transfer_description = NRFX_TWI_XFER_DESC_TX(address, (uint8_t *)p_data, length),     \
	.flags                = (_flags)                                            \
}

#define DK_TWI_MNGR_RX(address, p_data, length, _flags)                         \
{                                                                               \
	.transfer_description = NRFX_TWI_XFER_DESC_RX(address, p_data, length),     \
	.flags                = (_flags)                                            \
}

#define DK_TWI_MNGR_TX_RX(address, p_tx, tx_len, p_rx, rx_len, _flags)                      \
{                                                                                           \
	.transfer_description = NRFX_TWI_XFER_DESC_TXRX(address, p_tx, tx_len, p_rx, rx_len),   \
	.flags                = (_flags)                                                        \
}

#define DK_TWI_MNGR_TX_TX(address, p_tx, tx_len, p_tx2, tx_len2, _flags)                    \
{                                                                                           \
	.transfer_description = NRFX_TWI_XFER_DESC_TXTX(address, p_tx, tx_len, p_tx2, tx_len2), \
	.flags                = (_flags)                                                        \
}

typedef struct
{
	nrfx_twi_xfer_desc_t    transfer_description;   ///< Transfer description.
	uint8_t                 flags;                  ///< Transfer flags (see @ref NRFX_TWI_FLAG_TX_NO_STOP).
} dk_twi_mngr_transfer_t;

typedef void (* dk_twi_mngr_callback_t)(ret_code_t result, uint8_t evt, dk_twi_mngr_transfer_t * p_transfer, void * p_user_data);

typedef struct
{
	dk_twi_mngr_callback_t          callback;
	///< User-specified function to be called after the transaction is finished.

	void                          * p_user_data;
	///< Pointer to user data to be passed to the callback.

	uint8_t                         event_type;
	///< This variable can be used to pass event type to the callback.

	dk_twi_mngr_transfer_t          transfer;
	///< Transfer that is performed in this transaction.
} dk_twi_mngr_transaction_t;

typedef struct
{
	volatile dk_twi_mngr_transaction_t current_transaction;
	///< Currently realized transaction.
	volatile bool transaction_in_progress;
} dk_twi_mngr_cb_t;

typedef struct
{
	dk_twi_mngr_cb_t * p_dk_twi_mngr_cb;
	///< Control block of instance.

	nrf_queue_t const * p_queue;
	///< Transaction queue.

	nrfx_twi_t twi;
	///< Pointer to TWI master driver instance.
} dk_twi_mngr_t;

#define DK_TWI_MNGR_DEF(_dk_twi_mngr_name, _queue_size, _twi_idx)           \
	NRF_QUEUE_DEF(dk_twi_mngr_transaction_t,                                \
	              _dk_twi_mngr_name##_queue,                                \
	              (_queue_size),                                            \
	              NRF_QUEUE_MODE_NO_OVERFLOW);                              \
	static dk_twi_mngr_cb_t CONCAT_2(_dk_twi_mngr_name, _cb);               \
	static const dk_twi_mngr_t _dk_twi_mngr_name =                          \
	{                                                                       \
	    .p_dk_twi_mngr_cb           = &CONCAT_2(_dk_twi_mngr_name, _cb),    \
	    .p_queue                    = &_dk_twi_mngr_name##_queue,           \
	    .twi                        = NRFX_TWI_INSTANCE(_twi_idx)           \
	}

ret_code_t dk_twi_mngr_init(dk_twi_mngr_t     const * p_dk_twi_mngr,
                            nrfx_twi_config_t const * p_default_twi_config);

void dk_twi_mngr_uninit(dk_twi_mngr_t const * p_dk_twi_mngr);

inline void * dk_twi_mngr_data_buffer_alloc(uint32_t size)
{
	return nrf_malloc(size);
}

ret_code_t dk_twi_mngr_schedule(dk_twi_mngr_t             const * p_dk_twi_mngr,
                                dk_twi_mngr_transaction_t const * p_transaction);

ret_code_t dk_twi_mngr_perform(dk_twi_mngr_t          const * p_dk_twi_mngr,
                               dk_twi_mngr_transfer_t const * p_transfer,
                               void                           (* user_function)(void));

__STATIC_INLINE bool dk_twi_mngr_is_idle(dk_twi_mngr_t const * p_dk_twi_mngr);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION
__STATIC_INLINE bool dk_twi_mngr_is_idle(dk_twi_mngr_t const * p_dk_twi_mngr)
{
	return (!p_dk_twi_mngr->p_dk_twi_mngr_cb->transaction_in_progress);
}
#endif //SUPPRESS_INLINE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif

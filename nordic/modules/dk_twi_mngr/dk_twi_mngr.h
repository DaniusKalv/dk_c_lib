/**
 * @file        dk_twi_mngr.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TWI manager that implements a FIFO buffer on top on nrf_twi_mngr.
 * @version     0.1
 * @date        2019-12-05
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_TWI_MNGR_H
#define DK_TWI_MNGR_H

#include "nrf_twi_mngr.h"

#define DK_TWI_MNGR_WRITE_SIZE          10
#define IS31FL3206_DESIRED_FIFO_SIZE    16

typedef struct
{
	uint8_t reg_address;                    /**< Register address. */
	uint8_t data[DK_TWI_MNGR_WRITE_SIZE];   /**< Data buffer. */
} dk_twi_mngr_write_t;

typedef struct
{
	dk_twi_mngr_write_t         twi_write;
	nrf_twi_mngr_transfer_t     twi_transfer;
	nrf_twi_mngr_transaction_t  twi_transaction;
} dk_twi_mngr_fifo_element_t;

#if IS_POWER_OF_TWO(sizeof(dk_twi_mngr_fifo_element_t) * IS31FL3206_DESIRED_FIFO_SIZE)
	#define IS31FL3206_FIFO_SIZE IS31FL3206_DESIRED_FIFO_SIZE
#else
	#define IS31FL3206_FIFO_SIZE (((sizeof(dk_twi_mngr_fifo_element_t) * IS31FL3206_DESIRED_FIFO_SIZE) << 1) &    \
	                             (!((sizeof(dk_twi_mngr_fifo_element_t) * IS31FL3206_DESIRED_FIFO_SIZE))))
#endif

WORD_ALIGNED_MEM_BUFF(m_fifo_buffer, IS31FL3206_FIFO_SIZE);

typedef struct
{
	
} dk_twi_mngr_t;

#endif

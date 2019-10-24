/**
 * @file dk_twi.h
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief Helper functions for working with TWI peripheral.
 * @details These TWI functions implement ERRATA 89 workaround for nRF52832.
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_TWI_H
#define DK_TWI_H

#include "nrfx_twi.h"
#include "sdk_errors.h"

/**
 * @brief       Function for initializing and enabling twi instance
 * 
 * @param[in]   p_twi_instance              Pointer to the driver instance structure.
 * @param[in]   scl_pin                     SCL pin number
 * @param[in]   sda_pin                     SDA pin number
 * @param[in]   nrfx_twi_evt_handler        Event handler provided by the user. If NULL, blocking mode is enabled.
 * @param[in]   p_context                   Context passed to event handler.
 * 
 * @retval      NRFX_SUCCESS                If initialization was successful.
 * @retval      NRFX_ERROR_INVALID_STATE    If the driver is in invalid state.
 * @retval      NRFX_ERROR_BUSY             If some other peripheral with the same
 *                                          instance ID is already in use. This is
 *                                          possible only if @ref nrfx_prs module
 *                                          is enabled.
 */
ret_code_t dk_twi_enable(nrfx_twi_t * p_twi_instance, uint32_t scl_pin, uint32_t sda_pin, nrfx_twi_evt_handler_t nrfx_twi_evt_handler, void * p_context);

/**
 * @brief       Function for disabling TWI instance.
 * @details     Implements ERRATA 89 workaround for nRF52832.
 * 
 * @param[in]   p_twi_instance Pointer to the driver instance structure.
 */
void dk_twi_disable(nrfx_twi_t * p_twi_instance);

#endif // DK_TWI_H

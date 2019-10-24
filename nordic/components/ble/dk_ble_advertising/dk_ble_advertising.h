/**
 * @file dk_ble_advertising.h
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief Custom DK BLE advertising functions.
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_BLE_ADVERTISING_H
#define DK_BLE_ADVERTISING_H

#include "ble_advertising.h"

/**
 * @brief       Initialize DK BLE advertising module.
 * 
 * @param[in]   p_ble_advertising   Pointer to BLE advertising structure.
 * @param[in]   evt_handler         Advertising module event handler.
 * 
 * @retval      NRF_SUCCESS         on success,
 * @retval      Other               error codes returned by ble_advertising_init function. 
 */
ret_code_t dk_ble_advertising_init(ble_advertising_t * p_ble_advertising, ble_adv_evt_handler_t evt_handler);

/**
 * @brief       Start BLE advertising.
 * 
 * @param[in]   p_ble_advertising   Pointer to BLE advertising structure.
 * 
 * @retval      NRF_SUCCESS         on success,
 * @retval      Other               error codes returned by ble_advertising_start function.
 */
static __INLINE ret_code_t dk_ble_advertising_start(ble_advertising_t * p_ble_advertising)
{
	return ble_advertising_start(p_ble_advertising, BLE_ADV_MODE_FAST);
}

/**
 * @brief       Stop BLE advertising.
 * 
 * @param[in]   p_ble_advertising   Pointer to BLE advertising structure. 
 * 
 * @retval      NRF_SUCCESS         on success,
 * @retval      Other               error codes returned by ble_advertising_start function.
 */
static __INLINE ret_code_t dk_ble_advertising_stop(ble_advertising_t * p_ble_advertising)
{
	return ble_advertising_start(p_ble_advertising, BLE_ADV_MODE_IDLE);
}

/**
 * @brief       Get default advertising configuration.
 * 
 * @param[out]  p_config        Pointer to ble advertising configuration structure.
 * 
 * @retval      NRF_SUCCESS     on success,
 * @retval      NRF_ERROR_NULL  if p_config is NULL.
 */
ret_code_t dk_ble_advertising_config_get(ble_adv_modes_config_t * p_config);

#endif // DK_BLE_ADVERTISING_H

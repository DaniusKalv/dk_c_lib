/**
 * @file        dk_ble_dis.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef DK_BLE_DIS_H
#define DK_BLE_DIS_H

#include "ble_dis.h"
#include "sdk_errors.h"

/**
 * @brief   Initialize device information service with DK advertising data.
 *
 * @retval  NRF_SUCCESS on success,
 * @retval  Other       error codes returned by internal functions.
 */
ret_code_t dk_ble_dis_init(void);

#endif // DK_BLE_DIS_H

/**
 * @file dk_ble_dis.h
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief Custom DK BLE device information service wraper.
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
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

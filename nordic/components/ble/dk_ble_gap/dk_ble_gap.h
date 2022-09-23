/**
 * @file        dk_ble_gap.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       BLE GAP initialization functions according to DK standard.
 * @version     0.1
 * @date        2020-02-27
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef DK_BLE_GAP_H
#define DK_BLE_GAP_H

#include "sdk_errors.h"

/**
 * @brief   Function for the GAP initialization according to DK standard.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
ret_code_t dk_ble_gap_init(void);

#endif // DK_BLE_GAP_H

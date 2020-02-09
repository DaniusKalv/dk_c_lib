/**
 * @file        dk_ble_uuids.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       DK BLE services UUID defines.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef DK_BLE_UUIDS_H
#define DK_BLE_UUIDS_H

#define DK_BLE_UUID_BASE    {{0x8F, 0x02, 0xED, 0x3E, 0x20, 0xE9, 0x4F, 0x60, 0xB1, 0x7A, 0xA4, 0x78, 0x1B, 0x33, 0x43, 0x01}} // 128-bit base UUID

// Accelerometer service
#define DK_BLE_UUID_ACC_SERVICE                     0x4B24  /**< Accelerometer service UUID. */
#define DK_BLE_UUID_ACC_RAW_CHARACTERISTIC          0x4B25  /**< Accelerometer raw data characteristic UUID. */
#define DK_BLE_UUID_ACC_ALERT_CHARACTERISTIC        0x4B26  /**< Accelerometer alert characteristic UUID. */
#define DK_BLE_UUID_ACC_CONFIG_CHARACTERISTIC       0x4B27  /**< Accelerometer configuration characteristic UUID. */

// Gyro service
#define DK_BLE_UUID_GYRO_SERVICE                    0x3044  /**< Gyro service UUID. */
#define DK_BLE_UUID_GYRO_RAW_CHARACTERISTIC         0x3045  /**< Gyro raw data characteristic UUID. */
#define DK_BLE_UUID_GYRO_ALERT_CHARACTERISTIC       0x3046  /**< Gyro alert characteristic UUID. */
#define DK_BLE_UUID_GYRO_CONFIG_CHARACTERISTIC      0x3047  /**< Gyro configuration characteristic UUID. */

#endif // DK_BLE_UUIDS_H

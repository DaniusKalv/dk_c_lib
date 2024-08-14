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
#define DK_BLE_UUID_ACC_SERVICE                             0x4B24  /**< Accelerometer service UUID. */
#define DK_BLE_UUID_ACC_RAW_CHARACTERISTIC                  0x4B25  /**< Accelerometer raw data characteristic UUID. */
#define DK_BLE_UUID_ACC_ALERT_CHARACTERISTIC                0x4B26  /**< Accelerometer alert characteristic UUID. */
#define DK_BLE_UUID_ACC_CONFIG_CHARACTERISTIC               0x4B27  /**< Accelerometer configuration characteristic UUID. */

// Gyro service
#define DK_BLE_UUID_GYRO_SERVICE                            0x3044  /**< Gyro service UUID. */
#define DK_BLE_UUID_GYRO_RAW_CHARACTERISTIC                 0x3045  /**< Gyro raw data characteristic UUID. */
#define DK_BLE_UUID_GYRO_ALERT_CHARACTERISTIC               0x3046  /**< Gyro alert characteristic UUID. */
#define DK_BLE_UUID_GYRO_CONFIG_CHARACTERISTIC              0x3047  /**< Gyro configuration characteristic UUID. */

// Magnetometer service
#define DK_BLE_UUID_MAG_SERVICE                             0xCB26  /**< Mag service UUID. */
#define DK_BLE_UUID_MAG_RAW_CHARACTERISTIC                  0xCB27  /**< Mag raw data characteristic UUID. */
#define DK_BLE_UUID_MAG_ALERT_CHARACTERISTIC                0xCB28  /**< Mag alert characteristic UUID. */
#define DK_BLE_UUID_MAG_CONFIG_CHARACTERISTIC               0xCB29  /**< Mag configuration characteristic UUID. */

// Mr Pickle service
#define DK_BLE_UUID_MR_PICKLE_SERVICE                       0x8746  /**< Mr Pickle service UUID. */
#define DK_BLE_UUID_MR_PICKLE_MODE_CHARACTERISTIC           0x8747  /**< Mr Pickle mode characteristic UUID. */

// Phil It Up service
#define DK_BLE_UUID_PHIL_IT_UP_SERVICE                      0xFD98  /**< Phil It Up service UUID. */
#define DK_BLE_UUID_PHIL_IT_UP_AMB_TEMP_CHARACTERISTIC      0xFD99  /**< Phil It Up ambient temperature characteristic UUID. */
#define DK_BLE_UUID_PHIL_IT_UP_MUG_TEMP_CHARACTERISTIC      0xFD9A  /**< Phil It Up mug temperature characteristic UUID. */
#define DK_BLE_UUID_PHIL_IT_UP_MUG_UP_CHARACTERISTIC        0xFD9B  /**< Phil It Up mug up characteristic UUID. */
#define DK_BLE_UUID_PHIL_IT_UP_TARGET_TEMP_CHARACTERISTIC   0xFD9C  /**< Phil It Up target temperature characteristic UUID. */

#endif // DK_BLE_UUIDS_H

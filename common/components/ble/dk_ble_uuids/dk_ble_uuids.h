/**
 * @file dk_ble_uuids.h
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief DK BLE services UUID defines.
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_BLE_UUIDS_H
#define DK_BLE_UUIDS_H

#define DK_BLE_UUID_BASE    {{0x8F, 0x02, 0xED, 0x3E, 0x20, 0xE9, 0x4F, 0x60, 0xB1, 0x7A, 0xA4, 0x78, 0x1B, 0x33, 0x43, 0x01}} // 128-bit base UUID

#define DK_BLE_UUID_TEST_SERVICE            0xABCD  /**< Test service UUID. */
#define DK_BLE_UUID_TEST_CHARACTERISTIC     0xAAAA  /**< Test service log characteristic UUID. */

#endif // DK_BLE_UUIDS_H

/**
 * @file        dk_ble_mag.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       BLE service for interacting with magnetometer
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef DK_BLE_MAG_H
#define DK_BLE_MAG_H

#include <stdbool.h>
#include <stdint.h>

#include "ble.h"

#define DK_MAG_RAW_CHARACTERISTIC_VALUE_SIZE                                                                           \
    6 // Magnetometer raw characteristic value size in bytes. (3 axis) * 2 bytes
#define DK_MAG_CONFIG_CHARACTERISTIC_VALUE_SIZE 3 // 3 bytes
#define DK_MAG_ALERT_CHARACTERISTIC_VALUE_SIZE  0

typedef enum
{
    DK_BLE_MAG_EVT_RAW_NOTIFICATIONS_ENABLED,
    DK_BLE_MAG_EVT_RAW_NOTIFICATIONS_DISABLED,
    DK_BLE_MAG_EVT_ALERT_NOTIFICATIONS_ENABLED,
    DK_BLE_MAG_EVT_ALERT_NOTIFICATIONS_DISABLED,
    DK_BLE_MAG_EVT_CONFIGURATION_CHANGED
} dk_ble_mag_evt_type_t;

typedef struct
{
    dk_ble_mag_evt_type_t event_type;
    uint8_t               data[DK_MAG_CONFIG_CHARACTERISTIC_VALUE_SIZE];
    uint8_t               data_length;
} dk_ble_mag_evt_t;

typedef struct dk_ble_mag_service_s dk_ble_mag_service_t;

typedef void (*dk_ble_mag_evt_handler_t)(dk_ble_mag_evt_t *p_dk_ble_mag_evt);

struct dk_ble_mag_service_s
{
    uint8_t                  uuid_type;      /**< UUID type for dk magnetometer service. */
    uint16_t                 conn_handle;    /**< Handle of the current connection (as provided by the BLE stack, is
                                                BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t                 service_handle; /**< Handle of Our Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t mag_raw_char_handles;    /**< Raw magnetometer characteristic handles. */
    ble_gatts_char_handles_t mag_alert_char_handles;
    ble_gatts_char_handles_t mag_config_char_handles; /**< Magnetometer config characteristic handles. */
    bool                     notifications_enabled;   /**< A value indicating when notifications are enabled. */
    dk_ble_mag_evt_handler_t dk_ble_mag_evt_handler;
    uint8_t                 *p_mag_config;            /**< Magnetometer configuration bytes. */
};

#define DK_BLE_MAG_DEF(_name)                                                                                          \
    static dk_ble_mag_service_t _name;                                                                                 \
    NRF_SDH_BLE_OBSERVER(_name##_obs, DK_BLE_MAG_OBSERVER_PRIO, dk_ble_mag_on_ble_evt, &_name)

void dk_ble_mag_service_init(dk_ble_mag_service_t *p_mag_service);

void dk_ble_mag_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context);

uint32_t dk_ble_mag_raw_char_notify(dk_ble_mag_service_t *p_mag_service, uint8_t *data);

uint32_t dk_ble_mag_alert_char_notify(dk_ble_mag_service_t *p_mag_service);

#endif // DK_BLE_MAG_H

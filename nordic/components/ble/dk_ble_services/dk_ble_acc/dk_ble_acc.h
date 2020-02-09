/**
 * @file        dk_ble_acc.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       BLE service for interacting with accelerometer.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef DK_BLE_ACC_H
#define DK_BLE_ACC_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"

#define DK_ACC_RAW_CHARACTERISTIC_VALUE_SIZE    6 // Accelerometer raw characteristic value size in bytes. (3 axis) * 2 bytes
#define DK_ACC_CONFIG_CHARACTERISTIC_VALUE_SIZE 3 // 3 bytes, one for mode config and two for threshold
#define DK_ACC_ALERT_CHARACTERISTIC_VALUE_SIZE  0 // 1 bytes, using just notification

typedef enum
{
	DK_BLE_ACC_EVT_RAW_NOTIFICATIONS_ENABLED,
	DK_BLE_ACC_EVT_RAW_NOTIFICATIONS_DISABLED,
	DK_BLE_ACC_EVT_ALERT_NOTIFICATIONS_ENABLED,
	DK_BLE_ACC_EVT_ALERT_NOTIFICATIONS_DISABLED,
	DK_BLE_ACC_EVT_CONFIGURATION_CHANGED
} dk_ble_acc_evt_type_t;

typedef struct
{
	dk_ble_acc_evt_type_t event_type;
	uint8_t data[DK_ACC_CONFIG_CHARACTERISTIC_VALUE_SIZE]; // Maximum amount of bytes to be received from central
	uint8_t data_length;
} dk_ble_acc_evt_t;

typedef struct dk_ble_acc_service_s dk_ble_acc_service_t;

typedef void (*dk_ble_acc_evt_handler_t) (dk_ble_acc_evt_t * p_dk_ble_acc_evt);

struct dk_ble_acc_service_s
{
	uint8_t                     uuid_type;                  /**< UUID type for dk accelerometer service. */
	uint16_t                    conn_handle;                /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
	uint16_t                    service_handle;             /**< Handle of Our Service (as provided by the BLE stack). */
	ble_gatts_char_handles_t    acc_raw_char_handles;       /**< Raw accelerometer characteristic handles. */
	ble_gatts_char_handles_t    acc_alert_char_handles;     /**< Accelerometer alert characteristic handles. */
	ble_gatts_char_handles_t    acc_config_char_handles;    /**< Accelerometer config characteristic handles. */
	bool                        notifications_enabled;      /**< A value indicating when notifications are enabled. */
	dk_ble_acc_evt_handler_t    dk_ble_acc_evt_handler;     /**< Handler function for dk acc service events. */
	uint8_t                   * p_accelerometer_config;     /**< Accelerometer configuration bytes. */
};

#define DK_BLE_ACC_DEF(_name)                   \
static dk_ble_acc_service_t _name;              \
NRF_SDH_BLE_OBSERVER(_name ## _obs,             \
                     DK_BLE_ACC_OBSERVER_PRIO,  \
                     dk_ble_acc_on_ble_evt,     \
                     &_name)

void dk_ble_acc_service_init(dk_ble_acc_service_t * p_dk_acc_service);

void dk_ble_acc_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

uint32_t dk_ble_acc_raw_char_notify(dk_ble_acc_service_t * p_dk_ble_acc_service, uint8_t * p_data);

uint32_t dk_ble_acc_alert_char_notify(dk_ble_acc_service_t * p_dk_ble_acc_service);

uint32_t dk_ble_acc_config_write(dk_ble_acc_service_t * p_dk_ble_acc_service, uint8_t * p_data);

#endif // DK_BLE_ACC_H

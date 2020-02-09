/**
 * @file        dk_ble_mr_pickle.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       BLE service for setting mr pickle mode.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef DK_BLE_MR_PICKLE_H
#define DK_BLE_MR_PICKLE_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"

#define DK_MR_PICKLE_MODE_CHARACTERISTIC_VALUE_SIZE 1 // Mr Pickle mode characteristic value size in bytes.

typedef enum
{
	DK_BLE_MR_PICKLE_EVT_CONFIGURATION_CHANGED
} dk_mr_pickle_evt_type_t;

typedef struct
{
	dk_mr_pickle_evt_type_t event_type;
	uint8_t mode;
} dk_mr_pickle_evt_t;

typedef struct dk_mr_pickle_service_s dk_mr_pickle_service_t;

typedef void (*dk_mr_pickle_evt_handler_t) (dk_mr_pickle_evt_t * p_dk_mr_pickle_evt);

struct dk_mr_pickle_service_s
{
	uint8_t                     uuid_type;                  /**< UUID type for device service. */
	uint16_t                    conn_handle;                /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
	uint16_t                    service_handle;             /**< Handle of device service (as provided by the BLE stack). */
	ble_gatts_char_handles_t    device_mode_char_handles;   /**< Device mode characteristic handles. */
	dk_mr_pickle_evt_handler_t  dk_mr_pickle_evt_handler;   /**< Device service event handler. */ 
	uint8_t                     initial_device_mode;        /**< Initial device mode when initialising the service. */
};

#define DK_BLE_MR_PICKLE_DEF(_name)                     \
static dk_mr_pickle_service_t _name;                    \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                     \
                     DK_BLE_MR_PICKLE_OBSERVER_PRIO,    \
                     dk_mr_pickle_on_ble_evt,           \
                     &_name)

void dk_mr_pickle_service_init(dk_mr_pickle_service_t * p_dk_mr_pickle_service);

void dk_mr_pickle_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

uint32_t dk_mr_pickle_mode_set(dk_mr_pickle_service_t * p_dk_mr_pickle_service, uint8_t mode);

#endif // DK_BLE_MR_PICKLE_H

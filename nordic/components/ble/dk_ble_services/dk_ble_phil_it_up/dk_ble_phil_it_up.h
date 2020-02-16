/**
 * @file        dk_ble_phil_it_up.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Phil It Up service.
 * @details     Supports multiple peripheral links.
 * @version     0.1
 * @date        2020-02-16
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef DK_BLE_PHIL_IT_UP_H
#define DK_BLE_PHIL_IT_UP_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_config.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
#include "ble_config.h"

#define DK_BLE_PHIL_IT_UP_AMB_CHAR_SIZE    sizeof(float)   /**< Ambient temperature characteristic size (float). */
#define DK_BLE_PHIL_IT_UP_OBJ_CHAR_SIZE    sizeof(float)   /**< Object temperature characteristic size (float). */
#define DK_BLE_PHIL_IT_UP_MUG_UP_CHAR_SIZE sizeof(uint8_t) /**< Mug up characteristic size. */

#define ORG_BLUETOOTH_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELCIUS 0x272F  /**< Bluetooth unit celcius code as provided by Bluetooth SIG.*/

/* Forward declaration of the dk_ble_phil_it_up_t type. */
typedef struct dk_ble_phil_it_up_s dk_ble_phil_it_up_t;

/** @brief Phil It Up service event types. */
typedef enum
{
	DK_BLE_PHIL_IT_UP_EVT_AMB_NOTIFICATIONS_ENABLED,       /**< Ambient temperature characteristic notifications enabled. */
	DK_BLE_PHIL_IT_UP_EVT_AMB_NOTIFICATIONS_DISABLED,      /**< Ambient temperature characteristic notifications disabled. */
	DK_BLE_PHIL_IT_UP_EVT_OBJ_NOTIFICATIONS_ENABLED,       /**< Object temperature characteristic notifications enabled. */
	DK_BLE_PHIL_IT_UP_EVT_OBJ_NOTIFICATIONS_DISABLED,      /**< Object temperature characteristic notifications disabled. */
	DK_BLE_PHIL_IT_UP_EVT_MUG_UP_NOTIFICATIONS_ENABLED,    /**< Mug Up characteristic notifications enabled. */
	DK_BLE_PHIL_IT_UP_EVT_MUG_UP_NOTIFICATIONS_DISABLED    /**< Mug Up characteristic notifications disabled. */
} dk_ble_phil_it_up_event_type_t;

/** @brief Phil it Up service event structure. */
typedef struct
{
	dk_ble_phil_it_up_event_type_t evt_type;       /**< Event type. */
	uint16_t                       conn_handle;    /**< Connection handle. */
} dk_ble_phil_it_up_evt_t;

/** @brief Phil it Up service characteristic handles. */
typedef struct
{
	ble_gatts_char_handles_t amb;       /**< Ambient temperature characteristic handle. */
	ble_gatts_char_handles_t obj;       /**< Object temperature characteristic handle. */
	ble_gatts_char_handles_t mug_up;    /**< Mug up characteristic handle. */
} dk_ble_phil_it_up_char_handles_t;

/** @brief DK Phil it Up Service event handler type. */
typedef void (*dk_ble_phil_it_up_evt_handler_t)(dk_ble_phil_it_up_evt_t * p_dk_ble_phil_it_up_evt);

/** @brief Phil it Up service configuration structure. */
typedef struct
{
	dk_ble_phil_it_up_evt_handler_t    evt_handler;        /**< Event handler. */
	bool                               initial_mug_up_val; /**< Initial mug up characteristic value. */
} dk_ble_phil_it_up_config_t;

/** @brief Phil it Up service structure. */
struct dk_ble_phil_it_up_s
{
	uint8_t                             uuid_type;      /**< UUID type for DK Phil it Up service. */
	uint16_t                            service_handle; /**< Handle of Phil it Up Service (as provided by the BLE stack). */
	dk_ble_phil_it_up_char_handles_t    char_handles;   /**< Phil it Up service characteristic handles. */
	dk_ble_phil_it_up_evt_handler_t     evt_handler;    /**< Event handler. */
};

/**@brief Macro for defining a DK Phil it Up service instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define DK_BLE_PHIL_IT_UP_DEF(_name)                   \
static dk_ble_phil_it_up_t _name;                      \
NRF_SDH_BLE_OBSERVER(_name##_obs,                      \
                     DK_BLE_PHIL_IT_UP_OBSERVER_PRIO,  \
                     dk_ble_phil_it_up_on_ble_evt,     \
                     &_name)

/**@brief       Function for handling the DK Phil it Up Service's BLE events.
 *
 * @details     The Phil it Up Service expects the application to call this function each time an
 *              event is received from the SoftDevice. This function processes the event if it
 *              is relevant and calls the Phil it Up Service event handler of the
 *              application if necessary.
 *
 * @param[in]   p_ble_evt   Event received from the SoftDevice.
 * @param[in]   p_context   Phil it Up Service structure.
 */
void dk_ble_phil_it_up_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

/**
 * @brief       Initialize Phil it Up service.
 * 
 * @param[in]   p_dk_ble_phil_it_up     Pointer to Phil it Up service structure.
 * @param[in]   p_phil_it_up_config     Pointer to initial Phil it Up service configuration structure.
 * 
 * @retval      NRF_SUCCESS             If the service was successfully initialized. Otherwise, an error code is returned.
 * @retval      NRF_ERROR_NULL          If either of the pointers p_nus or p_nus_init is NULL.
 */
ret_code_t dk_ble_phil_it_up_init(dk_ble_phil_it_up_t * p_dk_ble_phil_it_up, dk_ble_phil_it_up_config_t * p_phil_it_up_config);

/**
 * @brief       Send ambient temperature characteristic notification.
 * 
 * @param[in]   conn_handle             Connection handle.
 * @param[in]   p_dk_ble_phil_it_up     Pointer to Phil it Up service instance.
 * @param[in]   temperature             Temperature value.
 * 
 * @retval      NRF_SUCCESS on success,
 * @retval      Other       error codes returned by internal functions.
 */
ret_code_t dk_ble_phil_it_up_amb_tmp_notify(uint16_t conn_handle, dk_ble_phil_it_up_t * p_dk_ble_phil_it_up, float temperature);

/**
 * @brief       Send object (mug) temperature characteristic notification.
 * 
 * @param[in]   conn_handle             Connection handle.
 * @param[in]   p_dk_ble_phil_it_up     Pointer to Phil it Up service instance.
 * @param[in]   temperature             Temperature value. 
 * 
 * @retval      NRF_SUCCESS on success,
 * @retval      Other       error codes returned by internal functions.
 */
ret_code_t dk_ble_phil_it_up_mug_tmp_notify(uint16_t conn_handle, dk_ble_phil_it_up_t * p_dk_ble_phil_it_up, float temperature);

/**
 * @brief       Send mug up characteristic notification.
 * 
 * @param[in]   conn_handle             Connection handle.
 * @param[in]   p_dk_ble_phil_it_up     Pointer to Phil it Up service instance.
 * @param[in]   mug_up                  Mug Up value (true- when mug is up, false- when mug is down).
 * 
 * @retval      NRF_SUCCESS on success,
 * @retval      Other       error codes returned by internal functions.
 */
ret_code_t dk_ble_phil_it_up_mug_up_notify(uint16_t conn_handle, dk_ble_phil_it_up_t * p_dk_ble_phil_it_up, bool mug_up);

/**
 * @brief       Set mug up characteristic value.
 * 
 * @param[in]   conn_handle             Connection handle.
 * @param[in]   p_dk_ble_phil_it_up     Pointer to Phil it Up service instance.
 * @param[in]   mug_up                  Mug Up value (true- when mug is up, false- when mug is down).
 * 
 * @retval      NRF_SUCCESS on success,
 * @retval      Other       error codes returned by internal functions.
 */
ret_code_t dk_ble_phil_it_up_mug_up_value_set(uint16_t conn_handle, dk_ble_phil_it_up_t * p_dk_ble_phil_it_up, bool mug_up);

#endif // DK_BLE_PHIL_IT_UP_H

/**
 * @file        dk_ble_phil_it_up.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Phil It Up service.
 * @details     Supports multiple peripheral links.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#include "dk_ble_phil_it_up.h"

#include "app_error.h"

#define NRF_LOG_MODULE_NAME BLE_PHIL_IT_UP
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

/**
 * @brief       Function for handling the @ref BLE_GATTS_EVT_WRITE event from the SoftDevice.
 *
 * @param[in]   p_dk_ble_phil_it_up     Pointer to Phil it Up service instance.
 * @param[in]   p_ble_evt               Pointer to the event received from BLE stack.
 */
static void on_write(dk_ble_phil_it_up_t *p_dk_ble_phil_it_up, ble_evt_t const *p_ble_evt)
{
    ble_gatts_evt_write_t const *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    dk_ble_phil_it_up_evt_t      dk_ble_phil_it_up_evt;

    dk_ble_phil_it_up_evt.conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;

    if (p_evt_write->handle == p_dk_ble_phil_it_up->char_handles.amb_temp.cccd_handle)
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            dk_ble_phil_it_up_evt.evt_type = DK_BLE_PHIL_IT_UP_EVT_AMB_NOTIFICATIONS_ENABLED;
        } else
        {
            dk_ble_phil_it_up_evt.evt_type = DK_BLE_PHIL_IT_UP_EVT_AMB_NOTIFICATIONS_DISABLED;
        }
    } else if (p_evt_write->handle == p_dk_ble_phil_it_up->char_handles.mug_temp.cccd_handle)
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            dk_ble_phil_it_up_evt.evt_type = DK_BLE_PHIL_IT_UP_EVT_MUG_NOTIFICATIONS_ENABLED;
        } else
        {
            dk_ble_phil_it_up_evt.evt_type = DK_BLE_PHIL_IT_UP_EVT_MUG_NOTIFICATIONS_DISABLED;
        }
    } else if (p_evt_write->handle == p_dk_ble_phil_it_up->char_handles.mug_up.cccd_handle)
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            dk_ble_phil_it_up_evt.evt_type = DK_BLE_PHIL_IT_UP_EVT_MUG_UP_NOTIFICATIONS_ENABLED;
        } else
        {
            dk_ble_phil_it_up_evt.evt_type = DK_BLE_PHIL_IT_UP_EVT_MUG_UP_NOTIFICATIONS_DISABLED;
        }
    } else if ((p_evt_write->handle == p_dk_ble_phil_it_up->char_handles.target_temp.value_handle) &&
               (p_evt_write->len == DK_BLE_PHIL_IT_UP_TARGET_TEMP_CHAR_SIZE))
    {
        dk_ble_phil_it_up_evt.evt_type = DK_BLE_PHIL_IT_UP_EVT_TARGET_TEMP_RX;
        memcpy(&dk_ble_phil_it_up_evt.params.target_temp,
               p_evt_write->data,
               sizeof(dk_ble_phil_it_up_evt.params.target_temp));
    } else
    {
        NRF_LOG_WARNING("Unhandled BLE event");
        return;
    }

    p_dk_ble_phil_it_up->evt_handler(&dk_ble_phil_it_up_evt);
}

/**
 * @brief       Add ambient temperature characteristic.
 *
 * @param[in]   p_dk_ble_phil_it_up    Pointer to Phil it Up service instance.
 *
 * @retval      NRF_SUCCESS If the characteristic was successfully added. Otherwise, an error code from
 * characteristic_add is returned.
 */
static uint32_t dk_ble_phil_it_up_amb_tmp_characteristic_add(dk_ble_phil_it_up_t *p_dk_ble_phil_it_up)
{
    uint8_t             initial_value[DK_BLE_PHIL_IT_UP_AMB_TEMP_CHAR_SIZE] = {0};
    ble_gatts_char_pf_t amb_temp_pf;
    memset(&amb_temp_pf, 0, sizeof(amb_temp_pf));

    amb_temp_pf.format = BLE_GATT_CPF_FORMAT_FLOAT32;
    amb_temp_pf.unit   = ORG_BLUETOOTH_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS;

    ble_add_char_params_t amb_temp_char_params;
    memset(&amb_temp_char_params, 0, sizeof(amb_temp_char_params));

    amb_temp_char_params.uuid                  = DK_BLE_UUID_PHIL_IT_UP_AMB_TEMP_CHARACTERISTIC;
    amb_temp_char_params.uuid_type             = p_dk_ble_phil_it_up->uuid_type;
    amb_temp_char_params.max_len               = DK_BLE_PHIL_IT_UP_AMB_TEMP_CHAR_SIZE;
    amb_temp_char_params.init_len              = DK_BLE_PHIL_IT_UP_AMB_TEMP_CHAR_SIZE;
    amb_temp_char_params.p_init_value          = initial_value;
    amb_temp_char_params.char_props.read       = 1;
    amb_temp_char_params.char_props.notify     = 1;
    amb_temp_char_params.read_access           = SEC_OPEN;
    amb_temp_char_params.cccd_write_access     = SEC_OPEN;
    amb_temp_char_params.p_user_descr          = NULL;
    amb_temp_char_params.p_presentation_format = &amb_temp_pf;

    return characteristic_add(p_dk_ble_phil_it_up->service_handle,
                              &amb_temp_char_params,
                              &p_dk_ble_phil_it_up->char_handles.amb_temp);
}

/**
 * @brief       Add object (mug) temperature characteristic.
 *
 * @param[in]   p_dk_ble_phil_it_up    Pointer to Phil it Up service instance.
 *
 * @retval      NRF_SUCCESS If the characteristic was successfully added. Otherwise, an error code from
 * characteristic_add is returned.
 */
static uint32_t dk_ble_phil_it_up_mug_tmp_characteristic_add(dk_ble_phil_it_up_t *p_dk_ble_phil_it_up)
{
    uint8_t             initial_value[DK_BLE_PHIL_IT_UP_MUG_TEMP_CHAR_SIZE] = {0};
    ble_gatts_char_pf_t mug_temp_pf;
    memset(&mug_temp_pf, 0, sizeof(mug_temp_pf));

    mug_temp_pf.format = BLE_GATT_CPF_FORMAT_FLOAT32;
    mug_temp_pf.unit   = ORG_BLUETOOTH_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS;

    ble_add_char_params_t obj_temp_char_params;
    memset(&obj_temp_char_params, 0, sizeof(obj_temp_char_params));

    obj_temp_char_params.uuid                  = DK_BLE_UUID_PHIL_IT_UP_MUG_TEMP_CHARACTERISTIC;
    obj_temp_char_params.uuid_type             = p_dk_ble_phil_it_up->uuid_type;
    obj_temp_char_params.max_len               = DK_BLE_PHIL_IT_UP_MUG_TEMP_CHAR_SIZE;
    obj_temp_char_params.init_len              = DK_BLE_PHIL_IT_UP_MUG_TEMP_CHAR_SIZE;
    obj_temp_char_params.p_init_value          = initial_value;
    obj_temp_char_params.char_props.read       = 1;
    obj_temp_char_params.char_props.notify     = 1;
    obj_temp_char_params.read_access           = SEC_OPEN;
    obj_temp_char_params.cccd_write_access     = SEC_OPEN;
    obj_temp_char_params.p_user_descr          = NULL;
    obj_temp_char_params.p_presentation_format = &mug_temp_pf;

    return characteristic_add(p_dk_ble_phil_it_up->service_handle,
                              &obj_temp_char_params,
                              &p_dk_ble_phil_it_up->char_handles.mug_temp);
}

/**
 * @brief       Add mug up characteristic to Phil it Up service.
 *
 * @param[in]   p_dk_ble_phil_it_up     Pointer to Phil it Up service instance.
 * @param[in]   initial_value           Initial mug up value.
 *
 * @retval      NRF_SUCCESS If the characteristic was successfully added. Otherwise, an error code from
 * characteristic_add is returned.
 */
static uint32_t dk_ble_phil_it_up_mug_up_characteristic_add(dk_ble_phil_it_up_t *p_dk_ble_phil_it_up,
                                                            uint8_t              initial_value)
{
    ble_gatts_char_pf_t mug_up_pf;
    memset(&mug_up_pf, 0, sizeof(mug_up_pf));

    mug_up_pf.format = BLE_GATT_CPF_FORMAT_BOOLEAN;

    ble_add_char_params_t mug_up_char_params;
    memset(&mug_up_char_params, 0, sizeof(mug_up_char_params));

    mug_up_char_params.uuid                  = DK_BLE_UUID_PHIL_IT_UP_MUG_UP_CHARACTERISTIC;
    mug_up_char_params.uuid_type             = p_dk_ble_phil_it_up->uuid_type;
    mug_up_char_params.max_len               = DK_BLE_PHIL_IT_UP_MUG_UP_CHAR_SIZE;
    mug_up_char_params.init_len              = DK_BLE_PHIL_IT_UP_MUG_UP_CHAR_SIZE;
    mug_up_char_params.p_init_value          = &initial_value;
    mug_up_char_params.char_props.read       = 1;
    mug_up_char_params.char_props.notify     = 1;
    mug_up_char_params.read_access           = SEC_OPEN;
    mug_up_char_params.cccd_write_access     = SEC_OPEN;
    mug_up_char_params.p_user_descr          = NULL;
    mug_up_char_params.p_presentation_format = &mug_up_pf;

    return characteristic_add(p_dk_ble_phil_it_up->service_handle,
                              &mug_up_char_params,
                              &p_dk_ble_phil_it_up->char_handles.mug_up);
}

/**
 * @brief       Add target temperature characteristic to Phil it Up service.
 *
 * @param[in]   p_dk_ble_phil_it_up    Pointer to Phil it Up service instance.
 * @param[in]   initial_value          Initial target temperature value.
 *
 * @retval      NRF_SUCCESS If the characteristic was successfully added. Otherwise, an error code from
 * characteristic_add is returned.
 */
static uint32_t dk_ble_phil_it_up_target_temp_characteristic_add(dk_ble_phil_it_up_t *p_dk_ble_phil_it_up,
                                                                 float                initial_value)
{
    ble_gatts_char_pf_t target_temp_pf;
    memset(&target_temp_pf, 0, sizeof(target_temp_pf));

    target_temp_pf.format = BLE_GATT_CPF_FORMAT_FLOAT32;
    target_temp_pf.unit   = ORG_BLUETOOTH_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS;

    ble_add_char_params_t target_temp_char_params;
    memset(&target_temp_char_params, 0, sizeof(target_temp_char_params));

    target_temp_char_params.uuid                  = DK_BLE_UUID_PHIL_IT_UP_MUG_TEMP_CHARACTERISTIC;
    target_temp_char_params.uuid_type             = p_dk_ble_phil_it_up->uuid_type;
    target_temp_char_params.max_len               = DK_BLE_PHIL_IT_UP_MUG_TEMP_CHAR_SIZE;
    target_temp_char_params.init_len              = DK_BLE_PHIL_IT_UP_MUG_TEMP_CHAR_SIZE;
    target_temp_char_params.p_init_value          = (uint8_t *)&initial_value;
    target_temp_char_params.char_props.read       = 1;
    target_temp_char_params.char_props.write      = 1;
    target_temp_char_params.read_access           = SEC_OPEN;
    target_temp_char_params.write_access          = SEC_OPEN;
    target_temp_char_params.p_user_descr          = NULL;
    target_temp_char_params.p_presentation_format = &target_temp_pf;

    return characteristic_add(p_dk_ble_phil_it_up->service_handle,
                              &target_temp_char_params,
                              &p_dk_ble_phil_it_up->char_handles.target_temp);
}

void dk_ble_phil_it_up_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context)
{
    if ((p_context == NULL) || (p_ble_evt == NULL))
        return;

    dk_ble_phil_it_up_t *p_dk_ble_phil_it_up = (dk_ble_phil_it_up_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTS_EVT_WRITE:
            on_write(p_dk_ble_phil_it_up, p_ble_evt);
            break;
        default:
            break;
    }
}

ret_code_t dk_ble_phil_it_up_init(dk_ble_phil_it_up_t        *p_dk_ble_phil_it_up,
                                  dk_ble_phil_it_up_config_t *p_phil_it_up_config)
{
    VERIFY_PARAM_NOT_NULL(p_dk_ble_phil_it_up);
    VERIFY_PARAM_NOT_NULL(p_phil_it_up_config);
    VERIFY_PARAM_NOT_NULL(p_phil_it_up_config->evt_handler);

    NRF_LOG_INFO("Initializing BLE Phil It Up service.");

    ret_code_t    err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t dk_ble_phil_it_up_base_uuid = DK_BLE_UUID_BASE;

    p_dk_ble_phil_it_up->evt_handler = p_phil_it_up_config->evt_handler;

    err_code = sd_ble_uuid_vs_add(&dk_ble_phil_it_up_base_uuid, &p_dk_ble_phil_it_up->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_dk_ble_phil_it_up->uuid_type;
    ble_uuid.uuid = DK_BLE_UUID_PHIL_IT_UP_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_dk_ble_phil_it_up->service_handle);
    VERIFY_SUCCESS(err_code);

    err_code = dk_ble_phil_it_up_amb_tmp_characteristic_add(p_dk_ble_phil_it_up);
    VERIFY_SUCCESS(err_code);

    err_code = dk_ble_phil_it_up_mug_tmp_characteristic_add(p_dk_ble_phil_it_up);
    VERIFY_SUCCESS(err_code);

    err_code = dk_ble_phil_it_up_mug_up_characteristic_add(p_dk_ble_phil_it_up,
                                                           (uint8_t)p_phil_it_up_config->initial_mug_up_val);
    VERIFY_SUCCESS(err_code);

    err_code =
      dk_ble_phil_it_up_target_temp_characteristic_add(p_dk_ble_phil_it_up, p_phil_it_up_config->initial_target_temp);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

ret_code_t dk_ble_phil_it_up_amb_tmp_notify(uint16_t             conn_handle,
                                            dk_ble_phil_it_up_t *p_dk_ble_phil_it_up,
                                            float                temperature)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(temperature);

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_dk_ble_phil_it_up->char_handles.amb_temp.value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.p_len  = &length;
    hvx_params.p_data = (uint8_t *)&temperature;

    return sd_ble_gatts_hvx(conn_handle, &hvx_params);
}

ret_code_t dk_ble_phil_it_up_mug_tmp_notify(uint16_t             conn_handle,
                                            dk_ble_phil_it_up_t *p_dk_ble_phil_it_up,
                                            float                temperature)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(temperature);

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_dk_ble_phil_it_up->char_handles.mug_temp.value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.p_len  = &length;
    hvx_params.p_data = (uint8_t *)&temperature;

    return sd_ble_gatts_hvx(conn_handle, &hvx_params);
}

ret_code_t dk_ble_phil_it_up_mug_up_notify(uint16_t conn_handle, dk_ble_phil_it_up_t *p_dk_ble_phil_it_up, bool mug_up)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length     = DK_BLE_PHIL_IT_UP_MUG_UP_CHAR_SIZE;
    uint8_t                mug_up_val = (uint8_t)mug_up;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_dk_ble_phil_it_up->char_handles.mug_up.value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.p_len  = &length;
    hvx_params.p_data = &mug_up_val;

    return sd_ble_gatts_hvx(conn_handle, &hvx_params);
}

ret_code_t dk_ble_phil_it_up_mug_up_value_set(uint16_t             conn_handle,
                                              dk_ble_phil_it_up_t *p_dk_ble_phil_it_up,
                                              bool                 mug_up)
{
    ble_gatts_value_t ble_gatts_value;
    uint8_t           mug_up_val = (uint8_t)mug_up;

    ble_gatts_value.len     = DK_BLE_PHIL_IT_UP_MUG_UP_CHAR_SIZE;
    ble_gatts_value.offset  = 0;
    ble_gatts_value.p_value = &mug_up_val;

    return sd_ble_gatts_value_set(conn_handle, p_dk_ble_phil_it_up->char_handles.mug_up.value_handle, &ble_gatts_value);
}

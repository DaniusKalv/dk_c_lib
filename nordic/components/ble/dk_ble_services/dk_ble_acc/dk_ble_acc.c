/**
 * @file        dk_ble_acc.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       BLE service for interacting with accelerometer.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#include "dk_ble_acc.h"
#include "app_error.h"
#include "nrf_log.h"
#include "ble_config.h"

static uint32_t dk_ble_acc_raw_characteristic_add(dk_ble_acc_service_t * p_dk_acc_service){
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;
	uint8_t initial_value[DK_ACC_RAW_CHARACTERISTIC_VALUE_SIZE] = { 0 };

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = &cccd_md;
	char_md.p_sccd_md         = NULL;

	ble_uuid.type = p_dk_acc_service->uuid_type;
	ble_uuid.uuid = DK_BLE_UUID_ACC_RAW_CHARACTERISTIC;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc    = BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth = 0;
	attr_md.wr_auth = 0;
	attr_md.vlen    = 1;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid    = &ble_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = DK_ACC_RAW_CHARACTERISTIC_VALUE_SIZE;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = DK_ACC_RAW_CHARACTERISTIC_VALUE_SIZE;
	attr_char_value.p_value	  = initial_value;

	return sd_ble_gatts_characteristic_add(p_dk_acc_service->service_handle,
	                                       &char_md,
	                                       &attr_char_value,
	                                       &p_dk_acc_service->acc_raw_char_handles);
}

static uint32_t dk_ble_acc_alert_characteristic_add(dk_ble_acc_service_t * p_dk_acc_service){
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;
	// uint8_t             initial_value[DK_ACC_ALERT_CHARACTERISTIC_VALUE_SIZE] = { 0 };

	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = &cccd_md;
	char_md.p_sccd_md         = NULL;

	ble_uuid.type = p_dk_acc_service->uuid_type;
	ble_uuid.uuid = DK_BLE_UUID_ACC_ALERT_CHARACTERISTIC;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc    = BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth = 0;
	attr_md.wr_auth = 0;
	attr_md.vlen    = 1;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid    = &ble_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = DK_ACC_ALERT_CHARACTERISTIC_VALUE_SIZE;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = DK_ACC_ALERT_CHARACTERISTIC_VALUE_SIZE;

	return sd_ble_gatts_characteristic_add(p_dk_acc_service->service_handle,
	                                       &char_md,
	                                       &attr_char_value,
	                                       &p_dk_acc_service->acc_alert_char_handles);
}

static uint32_t dk_ble_acc_config_characteristic_add(dk_ble_acc_service_t * p_dk_acc_service){
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read	  = 1;
	char_md.char_props.write  = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = NULL;
	char_md.p_sccd_md         = NULL;

	ble_uuid.type = p_dk_acc_service->uuid_type;
	ble_uuid.uuid = DK_BLE_UUID_ACC_CONFIG_CHARACTERISTIC;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc    = BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth = 0;
	attr_md.wr_auth = 0;
	attr_md.vlen    = 1;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid    = &ble_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = DK_ACC_CONFIG_CHARACTERISTIC_VALUE_SIZE;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = DK_ACC_CONFIG_CHARACTERISTIC_VALUE_SIZE;
	attr_char_value.p_value   = p_dk_acc_service->p_accelerometer_config;

	return sd_ble_gatts_characteristic_add(p_dk_acc_service->service_handle,
	                                       &char_md,
	                                       &attr_char_value,
	                                       &p_dk_acc_service->acc_config_char_handles);
}

void dk_ble_acc_service_init(dk_ble_acc_service_t * p_dk_acc_service)
{
	NRF_LOG_INFO("Initializing dk accelerometer service.");

	ret_code_t      err_code;
	ble_uuid_t      ble_uuid;
	ble_uuid128_t   dk_acc_base_uuid = DK_BLE_UUID_BASE;

	p_dk_acc_service->conn_handle = BLE_CONN_HANDLE_INVALID;
	p_dk_acc_service->notifications_enabled = false;

	err_code = sd_ble_uuid_vs_add(&dk_acc_base_uuid, &p_dk_acc_service->uuid_type);
	VERIFY_SUCCESS_VOID(err_code);

	ble_uuid.type = p_dk_acc_service->uuid_type;
	ble_uuid.uuid = DK_BLE_UUID_ACC_SERVICE;

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
	                                    &ble_uuid,
	                                    &p_dk_acc_service->service_handle);
	VERIFY_SUCCESS_VOID(err_code);
	
	err_code = dk_ble_acc_raw_characteristic_add(p_dk_acc_service);
	VERIFY_SUCCESS_VOID(err_code);
	
	err_code = dk_ble_acc_config_characteristic_add(p_dk_acc_service);
	VERIFY_SUCCESS_VOID(err_code);

	err_code = dk_ble_acc_alert_characteristic_add(p_dk_acc_service);
	VERIFY_SUCCESS_VOID(err_code);
}

static void on_ble_write(dk_ble_acc_service_t * p_dk_ble_acc_service, ble_evt_t const * p_ble_evt)
{
	if(p_ble_evt->evt.gatts_evt.params.write.handle == p_dk_ble_acc_service->acc_raw_char_handles.cccd_handle) {
		ret_code_t          err_code;
		dk_ble_acc_evt_t    dk_ble_acc_evt; 
		ble_gatts_value_t   rx_data;

		dk_ble_acc_evt.data_length  = sizeof(uint8_t);
		rx_data.len                 = sizeof(uint8_t);
		rx_data.offset              = 0;
		rx_data.p_value             = dk_ble_acc_evt.data;

		err_code = sd_ble_gatts_value_get(p_dk_ble_acc_service->conn_handle, p_dk_ble_acc_service->acc_raw_char_handles.cccd_handle, &rx_data);
		APP_ERROR_CHECK(err_code);
		if(dk_ble_acc_evt.data[0] == 0){
			dk_ble_acc_evt.event_type = DK_BLE_ACC_EVT_RAW_NOTIFICATIONS_DISABLED;
			p_dk_ble_acc_service->dk_ble_acc_evt_handler(&dk_ble_acc_evt);
		}
		else if (dk_ble_acc_evt.data[0] == 1){
			dk_ble_acc_evt.event_type = DK_BLE_ACC_EVT_RAW_NOTIFICATIONS_ENABLED;
			p_dk_ble_acc_service->dk_ble_acc_evt_handler(&dk_ble_acc_evt);
		}
	}
	else if(p_ble_evt->evt.gatts_evt.params.write.handle == p_dk_ble_acc_service->acc_alert_char_handles.cccd_handle) {
		ret_code_t          err_code;
		dk_ble_acc_evt_t    dk_ble_acc_evt; 
		ble_gatts_value_t   rx_data;

		dk_ble_acc_evt.data_length  = sizeof(uint8_t);
		rx_data.len                 = sizeof(uint8_t);
		rx_data.offset              = 0;
		rx_data.p_value             = dk_ble_acc_evt.data;

		err_code = sd_ble_gatts_value_get(p_dk_ble_acc_service->conn_handle, p_dk_ble_acc_service->acc_alert_char_handles.cccd_handle, &rx_data);
		APP_ERROR_CHECK(err_code);
		if(dk_ble_acc_evt.data[0] == 0){
			dk_ble_acc_evt.event_type = DK_BLE_ACC_EVT_ALERT_NOTIFICATIONS_DISABLED;
			p_dk_ble_acc_service->dk_ble_acc_evt_handler(&dk_ble_acc_evt);
		}
		else if (dk_ble_acc_evt.data[0] == 1){
			dk_ble_acc_evt.event_type = DK_BLE_ACC_EVT_ALERT_NOTIFICATIONS_ENABLED;
			p_dk_ble_acc_service->dk_ble_acc_evt_handler(&dk_ble_acc_evt);
		}
	}
	else if(p_ble_evt->evt.gatts_evt.params.write.handle == p_dk_ble_acc_service->acc_config_char_handles.value_handle){
		ret_code_t          err_code;
		dk_ble_acc_evt_t    dk_ble_acc_evt;
		ble_gatts_value_t   rx_data;

		dk_ble_acc_evt.data_length  = DK_ACC_CONFIG_CHARACTERISTIC_VALUE_SIZE;
		rx_data.len                 = DK_ACC_CONFIG_CHARACTERISTIC_VALUE_SIZE;
		rx_data.offset              = 0;
		rx_data.p_value             = dk_ble_acc_evt.data;

		err_code = sd_ble_gatts_value_get(p_dk_ble_acc_service->conn_handle, p_dk_ble_acc_service->acc_config_char_handles.value_handle, &rx_data);
		APP_ERROR_CHECK(err_code);
		dk_ble_acc_evt.event_type = DK_BLE_ACC_EVT_CONFIGURATION_CHANGED;
		p_dk_ble_acc_service->dk_ble_acc_evt_handler(&dk_ble_acc_evt);
	}
}

void dk_ble_acc_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context){
	if((p_context == NULL) || (p_ble_evt == NULL)) return;

	dk_ble_acc_service_t * p_dk_ble_acc_service = (dk_ble_acc_service_t *)p_context;
	
	switch (p_ble_evt->header.evt_id)
	{
		case BLE_GATTS_EVT_WRITE:
			on_ble_write(p_dk_ble_acc_service, p_ble_evt);
			break;
		case BLE_GAP_EVT_CONNECTED:
			p_dk_ble_acc_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
			break;
		case BLE_GAP_EVT_DISCONNECTED:
			p_dk_ble_acc_service->conn_handle = BLE_CONN_HANDLE_INVALID;
			break;
		default:
			break;
	}
}

uint32_t dk_ble_acc_raw_char_notify(dk_ble_acc_service_t * p_dk_ble_acc_service, uint8_t * p_data){
	ble_gatts_hvx_params_t hvx_params;
	uint16_t data_length = DK_ACC_RAW_CHARACTERISTIC_VALUE_SIZE;

	if(p_dk_ble_acc_service->conn_handle == BLE_CONN_HANDLE_INVALID){
		return NRF_ERROR_NOT_FOUND;
	}

	memset(&hvx_params, 0, sizeof(hvx_params));

	hvx_params.handle   = p_dk_ble_acc_service->acc_raw_char_handles.value_handle;
	hvx_params.type     = BLE_GATT_HVX_NOTIFICATION;
	hvx_params.p_len    = &data_length;
	hvx_params.p_data   = p_data;

	return sd_ble_gatts_hvx(p_dk_ble_acc_service->conn_handle, &hvx_params); 
}

uint32_t dk_ble_acc_alert_char_notify(dk_ble_acc_service_t * p_dk_ble_acc_service){
	ble_gatts_hvx_params_t hvx_params;
	uint16_t data_length = DK_ACC_ALERT_CHARACTERISTIC_VALUE_SIZE;

	if(p_dk_ble_acc_service->conn_handle == BLE_CONN_HANDLE_INVALID){
		return NRF_ERROR_NOT_FOUND;
	}

	memset(&hvx_params, 0, sizeof(hvx_params));

	hvx_params.handle = p_dk_ble_acc_service->acc_alert_char_handles.value_handle;
	hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
	hvx_params.p_len = &data_length;

	return sd_ble_gatts_hvx(p_dk_ble_acc_service->conn_handle, &hvx_params); 
}

uint32_t dk_ble_acc_config_write(dk_ble_acc_service_t * p_dk_ble_acc_service, uint8_t * p_data)
{
    if(p_dk_ble_acc_service->conn_handle == BLE_CONN_HANDLE_INVALID){
        return NRF_ERROR_NOT_FOUND;
    }

	ble_gatts_value_t ble_gatts_value;

	ble_gatts_value.len = DK_ACC_CONFIG_CHARACTERISTIC_VALUE_SIZE;
	ble_gatts_value.offset = 0;
	ble_gatts_value.p_value = p_data;

	return sd_ble_gatts_value_set(p_dk_ble_acc_service->conn_handle, p_dk_ble_acc_service->acc_config_char_handles.value_handle, &ble_gatts_value);
}
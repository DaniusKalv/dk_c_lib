/**
 * @file        dk_ble_mr_pickle.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       BLE service for setting mr pickle mode.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#include "dk_ble_mr_pickle.h"
#include "app_error.h"
#include "nrf_log.h"
#include "ble_config.h"

static uint32_t dk_mr_pickle_mode_characteristic_add(dk_mr_pickle_service_t * p_dk_mr_pickle_service){
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;
	uint8_t initial_value[DK_MR_PICKLE_MODE_CHARACTERISTIC_VALUE_SIZE] = { p_dk_mr_pickle_service->initial_device_mode };

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read	  = 1;
	char_md.char_props.write  = 1;
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = NULL;
	char_md.p_sccd_md         = NULL;

	ble_uuid.type = p_dk_mr_pickle_service->uuid_type;
	ble_uuid.uuid = DK_BLE_UUID_MR_PICKLE_MODE_CHARACTERISTIC;

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
	attr_char_value.init_len  = DK_MR_PICKLE_MODE_CHARACTERISTIC_VALUE_SIZE;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = DK_MR_PICKLE_MODE_CHARACTERISTIC_VALUE_SIZE;
	attr_char_value.p_value	  = initial_value;

	return sd_ble_gatts_characteristic_add(p_dk_mr_pickle_service->service_handle,
	                                       &char_md,
	                                       &attr_char_value,
	                                       &p_dk_mr_pickle_service->device_mode_char_handles);
}

void dk_mr_pickle_service_init(dk_mr_pickle_service_t * p_dk_mr_pickle_service)
{
	NRF_LOG_INFO("Initializing dk mr pickle service.");

	ret_code_t      err_code;
	ble_uuid_t      ble_uuid;
	ble_uuid128_t   dk_mr_pickle_base_uuid = DK_BLE_UUID_BASE;

	p_dk_mr_pickle_service->conn_handle = BLE_CONN_HANDLE_INVALID;

	err_code = sd_ble_uuid_vs_add(&dk_mr_pickle_base_uuid, &p_dk_mr_pickle_service->uuid_type);
	VERIFY_SUCCESS_VOID(err_code);

	ble_uuid.type = p_dk_mr_pickle_service->uuid_type;
	ble_uuid.uuid = DK_BLE_UUID_MR_PICKLE_SERVICE;

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
	                                    &ble_uuid,
	                                    &p_dk_mr_pickle_service->service_handle);
	VERIFY_SUCCESS_VOID(err_code);
	
	err_code = dk_mr_pickle_mode_characteristic_add(p_dk_mr_pickle_service);
	VERIFY_SUCCESS_VOID(err_code);
}

static void on_ble_write(dk_mr_pickle_service_t * p_dk_mr_pickle_service, ble_evt_t const * p_ble_evt)
{
	if(p_ble_evt->evt.gatts_evt.params.write.handle == p_dk_mr_pickle_service->device_mode_char_handles.value_handle){
		ret_code_t err_code;
		dk_mr_pickle_evt_t dk_mr_pickle_evt;
		ble_gatts_value_t rx_data;
		rx_data.len = sizeof(dk_mr_pickle_evt.mode);
		rx_data.offset = 0;
		rx_data.p_value = &dk_mr_pickle_evt.mode;
		err_code = sd_ble_gatts_value_get(p_dk_mr_pickle_service->conn_handle, p_dk_mr_pickle_service->device_mode_char_handles.value_handle, &rx_data);
		APP_ERROR_CHECK(err_code);
		dk_mr_pickle_evt.event_type = DK_BLE_MR_PICKLE_EVT_CONFIGURATION_CHANGED;
		p_dk_mr_pickle_service->dk_mr_pickle_evt_handler(&dk_mr_pickle_evt);
	}
}

void dk_mr_pickle_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context){
	if((p_context == NULL) || (p_ble_evt == NULL)) return;

	dk_mr_pickle_service_t * p_dk_mr_pickle_service = (dk_mr_pickle_service_t *)p_context;
	
	switch (p_ble_evt->header.evt_id)
	{
		case BLE_GATTS_EVT_WRITE:
			on_ble_write(p_dk_mr_pickle_service, p_ble_evt);
			break;
		case BLE_GAP_EVT_CONNECTED:
			p_dk_mr_pickle_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
			break;
		case BLE_GAP_EVT_DISCONNECTED:
			p_dk_mr_pickle_service->conn_handle = BLE_CONN_HANDLE_INVALID;
			break;
		default:
			break;
	}
}

uint32_t dk_mr_pickle_mode_set(dk_mr_pickle_service_t * p_dk_mr_pickle_service, uint8_t mode)
{
	if(p_dk_mr_pickle_service->conn_handle == BLE_CONN_HANDLE_INVALID){
		return NRF_ERROR_NOT_FOUND;
	}

	ble_gatts_value_t ble_gatts_value;

	ble_gatts_value.len = sizeof(mode);
	ble_gatts_value.offset = 0;
	ble_gatts_value.p_value = &mode;

	return sd_ble_gatts_value_set(p_dk_mr_pickle_service->conn_handle, p_dk_mr_pickle_service->device_mode_char_handles.value_handle, &ble_gatts_value);
}

/**
 * @file dk_ble_advertising.c
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_ble_advertising.h"
#include "dk_bootloader_data.h"
#include "ble_config.h"

#define NRF_LOG_MODULE_NAME dk_ble_advertising
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

ret_code_t dk_ble_advertising_init(ble_advertising_t * p_ble_advertising, ble_adv_evt_handler_t evt_handler)
{
	ret_code_t err_code;
	ble_advertising_init_t advertising_init;
	ble_advdata_manuf_data_t manuf_data;

#ifdef DEBUG
	uint8_t advertised_manuf_data[] = { DEVICE_APP_ID, (uint8_t)DEVICE_APP_V };
#else
	dk_bootloader_data_t * p_bootloader_data = DK_BOOTLOADER_DATA_SECTION_ITEM_GET;
	uint8_t advertised_manuf_data[] = { DEVICE_APP_ID, (uint8_t)p_bootloader_data->application_version };
#endif

	manuf_data.company_identifier   = MANUFACTURER_ID;
	manuf_data.data.p_data          = advertised_manuf_data;
	manuf_data.data.size            = sizeof(advertised_manuf_data);

	memset(&advertising_init, 0, sizeof(advertising_init));

	advertising_init.advdata.name_type              = BLE_ADVDATA_NO_NAME;
	advertising_init.advdata.include_appearance     = false;
	advertising_init.advdata.flags                  = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
	advertising_init.advdata.p_manuf_specific_data  = &manuf_data;

	advertising_init.srdata.name_type = BLE_ADVDATA_FULL_NAME;

	advertising_init.config.ble_adv_fast_enabled    = true;
	advertising_init.config.ble_adv_fast_interval   = APP_ADV_INTERVAL;
	advertising_init.config.ble_adv_fast_timeout    = APP_ADV_DURATION;

	advertising_init.evt_handler = evt_handler;

	err_code = ble_advertising_init(p_ble_advertising, &advertising_init);
	VERIFY_SUCCESS(err_code);

	ble_advertising_conn_cfg_tag_set(p_ble_advertising, APP_BLE_CONN_CFG_TAG);

	return NRF_SUCCESS;
}

ret_code_t dk_ble_advertising_config_get(ble_adv_modes_config_t * p_config)
{
	VERIFY_PARAM_NOT_NULL(p_config);

	memset(p_config, 0, sizeof(ble_adv_modes_config_t));

	p_config->ble_adv_fast_enabled  = true;
	p_config->ble_adv_fast_interval = APP_ADV_INTERVAL;
	p_config->ble_adv_fast_timeout  = APP_ADV_DURATION;

	return NRF_SUCCESS;
}

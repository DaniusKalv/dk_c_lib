/**
 * @file        dk_ble_gap.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       BLE GAP initialization functions according to DK standard.
 * @version     0.1
 * @date        2020-02-27
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#include <stdio.h>
#include <string.h>
#include "dk_ble_gap.h"
#include "ble_gap.h"
#include "ble_config.h"
#include "dk_uicr.h"
#include "sdk_macros.h"

ret_code_t dk_ble_gap_init(void)
{
	ret_code_t              err_code;
	ble_gap_conn_params_t   gap_conn_params;
	ble_gap_conn_sec_mode_t sec_mode;
	uint32_t                short_serial_number;

	dk_uicr_short_serial_number_get(&short_serial_number);

	char device_name[sizeof(DEVICE_NAME)+(sizeof(short_serial_number)*2)];

	sprintf(device_name, "%s%lX", DEVICE_NAME, short_serial_number);

	BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&sec_mode);

	err_code = sd_ble_gap_device_name_set(&sec_mode,
	                                      (const uint8_t *)device_name,
	                                      strlen(device_name));
	VERIFY_SUCCESS(err_code);

	err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN);
	VERIFY_SUCCESS(err_code);

	memset(&gap_conn_params, 0, sizeof(gap_conn_params));

	gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
	gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
	gap_conn_params.slave_latency     = SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

	return sd_ble_gap_ppcp_set(&gap_conn_params);
}

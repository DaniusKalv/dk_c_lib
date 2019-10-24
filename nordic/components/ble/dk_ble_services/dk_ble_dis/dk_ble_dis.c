/**
 * @file dk_ble_dis.c
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_ble_dis.h"
#include "dk_bootloader_data.h"
#include "ble_config.h"
#include "ble.h"
#include "sdk_macros.h"

#include <stdio.h>
#include <string.h>

ret_code_t dk_ble_dis_init(void)
{
	ret_code_t              err_code;
	ble_dis_init_t          dis_init;
	ble_version_t           ble_version;
	dk_bootloader_data_t  * p_bootloader_data = DK_BOOTLOADER_DATA_SECTION_ITEM_GET;

	char device_ser_number[((sizeof(DEVICE_HIGH_SERIAL_NUMBER) + sizeof(DEVICE_SHORT_SERIAL_NUMBER)) * 2) + 1]; //Sizeof device serial number (1 byte = 2 hex chars) and string terminator
	char device_model_number[8];                                                                                //Sizeof device model number (1 byte = 2 hex chars) and string terminator
	char device_firmware_revision[18];                                                                          //Sizeof versions (1 byte = 2 hex chars), two dots and string terminator

	// Get softdevice version number
	err_code = sd_ble_version_get(&ble_version);
	VERIFY_SUCCESS(err_code);

#ifdef DEBUG
	sprintf(device_firmware_revision, "%u.%u.%u.%u", DEVICE_APP_ID, DEVICE_APP_V, ble_version.subversion_number, (uint8_t)p_bootloader_data->bootloader_version);
#else
	sprintf(device_firmware_revision, "%u.%u.%u.%u", DEVICE_APP_ID, (uint8_t)p_bootloader_data->application_version, ble_version.subversion_number, (uint8_t)p_bootloader_data->bootloader_version);
#endif

	sprintf(device_ser_number, "%X%X", DEVICE_HIGH_SERIAL_NUMBER, (unsigned int)DEVICE_SHORT_SERIAL_NUMBER);
	sprintf(device_model_number, "%u.%u", DEVICE_MODEL_NUMBER, DEVICE_HW_REV);

	memset(&dis_init, 0, sizeof(dis_init));

	ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)MANUFACTURER_NAME);
	ble_srv_ascii_to_utf8(&dis_init.model_num_str, device_model_number);
	ble_srv_ascii_to_utf8(&dis_init.serial_num_str, device_ser_number);
	ble_srv_ascii_to_utf8(&dis_init.fw_rev_str, device_firmware_revision);

	dis_init.dis_char_rd_sec = SEC_OPEN;

	return ble_dis_init(&dis_init);
}

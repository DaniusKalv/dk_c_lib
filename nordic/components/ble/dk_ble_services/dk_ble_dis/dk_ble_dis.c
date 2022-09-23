/**
 * @file        dk_ble_dis.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @version     0.1
 * @date        2019-10-20
 * 
 * @copyright   (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_ble_dis.h"
#include "dk_bootloader_data.h"
#include "ble_config.h"
#include "ble.h"
#include "sdk_macros.h"
#include "dk_uicr.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief Expands to an integer constant expression evaluating to a close upper
 * bound on the number the number of decimal digits in a value expressible in
 * the integer type given by the argument (if it is a type name) or the the integer
 * type of the argument (if it is an expression). The meaning of the resulting
 * expression is unspecified for other arguments.
 */
#define DECIMAL_DIGITS_MAX(t) (241 * sizeof(t) / 100 + 1)

ret_code_t dk_ble_dis_init(void)
{
	ret_code_t              err_code;
	ble_dis_init_t          dis_init;
	ble_version_t           ble_version;
	dk_bootloader_data_t  * p_bootloader_data = DK_BOOTLOADER_DATA_SECTION_ITEM_GET;
	uint64_t                serial_number;
	dk_uicr_hw_info_t       hw_info;

	char device_ser_number[((sizeof(serial_number)) * 2) + 1];                                          // Sizeof device serial number (1 byte = 2 hex chars) and string terminator
	char device_model_number[DECIMAL_DIGITS_MAX(hw_info.hw_id) + DECIMAL_DIGITS_MAX(hw_info.hw_v) + 2]; // Sizeof device model number and string terminator
	char device_firmware_revision[DECIMAL_DIGITS_MAX(uint8_t) +                                         // Sizeof versions two dots and string terminator
	                              DECIMAL_DIGITS_MAX(uint8_t) +
	                              DECIMAL_DIGITS_MAX(ble_version.subversion_number) +
	                              DECIMAL_DIGITS_MAX(uint8_t) + 4];

	// Get softdevice version number
	err_code = sd_ble_version_get(&ble_version);
	VERIFY_SUCCESS(err_code);

	dk_uicr_full_serial_number_get(&serial_number);
	dk_uicr_hw_info_get(&hw_info);

#ifdef DEBUG
	sprintf(device_firmware_revision, "%u.%u.%u.%u", DEVICE_APP_ID, DEVICE_APP_V, ble_version.subversion_number, (uint8_t)p_bootloader_data->bootloader_version);
#else
	sprintf(device_firmware_revision, "%u.%u.%u.%u", DEVICE_APP_ID, (uint8_t)p_bootloader_data->application_version, ble_version.subversion_number, (uint8_t)p_bootloader_data->bootloader_version);
#endif

	sprintf(device_ser_number, "%lX%lX", (uint32_t)(serial_number >> 32),(uint32_t)serial_number);
	sprintf(device_model_number, "%u.%u", hw_info.hw_id, hw_info.hw_v);

	memset(&dis_init, 0, sizeof(dis_init));

	ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)MANUFACTURER_NAME);
	ble_srv_ascii_to_utf8(&dis_init.model_num_str, device_model_number);
	ble_srv_ascii_to_utf8(&dis_init.serial_num_str, device_ser_number);
	ble_srv_ascii_to_utf8(&dis_init.fw_rev_str, device_firmware_revision);

	dis_init.dis_char_rd_sec = SEC_OPEN;

	return ble_dis_init(&dis_init);
}

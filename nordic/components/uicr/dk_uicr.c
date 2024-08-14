/**
 * @file        dk_uicr.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       DK UICR register data.
 * @version     0.2
 * @date        2019-11-23
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_uicr.h"
#include "app_util.h"

#include <string.h>

#ifdef NRF52840_XXAA
#include "dk_config.h"
#endif

/* Write HW ID and HW version values to UICR. */
NRF_SECTION_ITEM_REGISTER(dk_uicr_hw_info, dk_uicr_hw_info_t dk_uicr_hw_info) =
{
	.hw_id  = HW_ID,
	.hw_v   = HW_VERSION
};

#if defined(NRF52840_XXAA) && defined(DK_UICR_REGOUT0)

/** @brief Voltage regulator output setting. */
typedef struct
{
	uint32_t vout       :3;     /**< Vout value. */
	uint32_t _padding0  :29;
} dk_uicr_regout0_t;

/* Write DK_UICR_REGOUT0 value to vout UICR register. */
NRF_SECTION_ITEM_REGISTER(dk_uicr_regout0, dk_uicr_regout0_t dk_uicr_regout0) =
{
	.vout = DK_UICR_REGOUT0
};

#endif // defined(NRF52840_XXAA) && defined(DK_UICR_REGOUT0)

/**
 * @brief Retrieve 32 least significant bits of device serial number from FICR.
 * 
 * @param p_short_serial_number Pointer to where return short serial number.
 */
void dk_uicr_short_serial_number_get(uint32_t * p_short_serial_number)
{
	*p_short_serial_number = NRF_FICR->DEVICEADDR[0];
}

/**
 * @brief Retrieve full 64 device serial number from FICR.
 * 
 * @param p_full_serial_number Pointer to where return full serial number.
 */
void dk_uicr_full_serial_number_get(uint64_t * p_full_serial_number)
{
	memcpy((uint8_t *)p_full_serial_number, (uint8_t *)NRF_FICR->DEVICEADDR, sizeof(uint64_t));
}

/**
 * @brief Retrieve hw info from UICR.
 * 
 * @param p_hw_info Pointer to where return hw info.
 */
void dk_uicr_hw_info_get(dk_uicr_hw_info_t * p_hw_info)
{
	p_hw_info->hw_id = dk_uicr_hw_info.hw_id;
	p_hw_info->hw_v = dk_uicr_hw_info.hw_v;
}

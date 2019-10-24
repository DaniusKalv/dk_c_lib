/**
 * @file dk_uicr.h
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief DK UICR register data.
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_UICR_H
#define DK_UICR_H

#include <stdint.h>
#include "nrf_section.h"
#include "nrf.h"

/** @brief DK UICR HW info structure. */
typedef struct
{
	uint8_t     hw_v;       /**< Hardware version. */
	uint8_t     hw_id;      /**< Hardware ID. */
	uint16_t    _padding0;
} hug_uicr_hw_info_t;

#ifdef NRF52840_XXAA

/** @brief Voltage regulator output setting. */
typedef struct
{
	uint32_t vout       :3;     /**< Vout value. */
	uint32_t _padding0  :29;
} hug_uicr_regout0_t;

#endif

/** @brief Macro to retrieve dk_uicr_hw_info section. */
#define DK_UICR_HW_INFO_SECTION_ITEM_GET   NRF_SECTION_ITEM_GET(dk_uicr_hw_info, dk_uicr_hw_info_t, 0)


#define DK_UICR_LOW_DEVICE_ID      NRF_FICR->DEVICEADDR[0]                         /**< Device ID low value. */
#define DK_UICR_HIGH_DEVICE_ID     (uint16_t)(NRF_FICR->DEVICEADDR[1] & 0xFFFF)    /**< Device ID high value. */

#define DK_UICR_DEVICE_MODEL_NO    (uint8_t) (NRF_UICR->CUSTOMER[0] >> 8) & 0xFF   /**< Using the 1 byte of the register to retrieve model number. */
#define DK_UICR_DEVICE_HW_REV      (uint8_t) NRF_UICR->CUSTOMER[0] & 0xFF          /**< Using the 0 byte of the register to retrieve hardware revision. */

#endif // DK_UICR_H

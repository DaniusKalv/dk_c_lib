/**
 * @file        dk_uicr.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       DK UICR register data.
 * @version     0.1
 * @date        2019-11-23
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
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
} dk_uicr_hw_info_t;

void dk_uicr_short_serial_number_get(uint32_t * p_short_serial_number);

void dk_uicr_full_serial_number_get(uint64_t * p_full_serial_number);

void dk_uicr_hw_info_get(dk_uicr_hw_info_t * p_hw_info);

#endif // DK_UICR_H

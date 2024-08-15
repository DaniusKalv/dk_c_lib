/**
 * @file        dk_uicr.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       DK UICR register data.
 * @version     0.3
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef DK_UICR_H
#define DK_UICR_H

#include <stdint.h>

#include "nrf.h"
#include "nrf_section.h"

#ifdef NRF52840_XXAA

void dk_uicr_full_serial_number_get(uint64_t *p_full_serial_number);

void dk_uicr_hw_info_get(dk_uicr_hw_info_t *p_hw_info);

#endif // DK_UICR_H

/**
 * @file    dk_bootloader_data.h
 * @author  Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief   Enables access to information that is stored in bootloader settings page.
 * @version 0.2
 * @date    2024-08-15
 *
 * @copyright Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef DK_BOOTLOADER_DATA_H
#define DK_BOOTLOADER_DATA_H

#include <stdint.h>

#include "nrf_section.h"

/** @brief Bootloader data struct. */
typedef struct
{
    uint32_t application_version;                                /**< Application version. */
    uint32_t bootloader_version;                                 /**< Bootloader version. */
} dk_bootloader_data_t;

NRF_SECTION_DEF(dk_bootloader_data, const dk_bootloader_data_t); // Define dk_bootloader_data section.

/** @brief Macro for retrieving dk bootloader data from section. */
#define DK_BOOTLOADER_DATA_SECTION_ITEM_GET NRF_SECTION_ITEM_GET(dk_bootloader_data, dk_bootloader_data_t, 0)

#endif

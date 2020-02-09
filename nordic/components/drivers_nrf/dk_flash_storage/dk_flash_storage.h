/**
 * @file        dk_flash_storage.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       DK flash storage access functions.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef DK_FLASH_STORAGE_H
#define DK_FLASH_STORAGE_H

#include <stdint.h>
#include "nrf_fstorage.h"

void dk_flash_storage_init(nrf_fstorage_t * p_fs, void (*fstorage_evt_handler), uint32_t start_address, uint32_t end_address);
void dk_flash_storage_read(nrf_fstorage_t * p_m_fstorage, uint32_t src, void * p_dest, uint32_t len, void (*wait_function)(void));
void dk_flash_storage_write(nrf_fstorage_t * p_m_fstorage, uint32_t dest, const void * p_src, uint32_t len, void (*wait_function)(void));
void dk_flash_storage_erase(nrf_fstorage_t * p_m_fstorage, uint32_t addr, uint32_t pages_cnt);

#endif
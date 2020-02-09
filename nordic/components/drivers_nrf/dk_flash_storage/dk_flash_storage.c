/**
 * @file        dk_flash_storage.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       DK flash storage access functions.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#include "nrf_log.h"
#include "app_error.h"
#include "nrf_fstorage_sd.h"
#include "dk_flash_storage.h"
#include "nrf_strerror.h"

static void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage, void (*wait_function)(void))
{
	/* While fstorage is busy, sleep and wait for an event. */
	while (nrf_fstorage_is_busy(p_fstorage))
	{
		wait_function();
	}
}

void dk_flash_storage_init(nrf_fstorage_t * p_fs, void (*fstorage_evt_handler), uint32_t start_address, uint32_t end_address){
	ret_code_t err_code;

	p_fs->evt_handler = fstorage_evt_handler;
	p_fs->start_addr = start_address;
	p_fs->end_addr = end_address;

	nrf_fstorage_api_t * p_fs_api;
	p_fs_api = &nrf_fstorage_sd;

	err_code = nrf_fstorage_init(p_fs, p_fs_api, NULL);
	APP_ERROR_CHECK(err_code);
}

void dk_flash_storage_read(nrf_fstorage_t * p_m_fstorage, uint32_t src, void * p_dest, uint32_t len, void (*wait_function)(void)){
	ret_code_t err_code;
	
	NRF_LOG_INFO("Reading from 0x%x in flash.", src);
	err_code =  nrf_fstorage_read(p_m_fstorage, src, p_dest, len);

	if(err_code != NRF_SUCCESS){
		NRF_LOG_ERROR("nrf_fstorage_read() returned: %s\r\n", nrf_strerror_get(err_code));
		return;
	}

	wait_for_flash_ready(p_m_fstorage, wait_function);
}

void dk_flash_storage_write(nrf_fstorage_t * p_m_fstorage, uint32_t dest, const void * p_src, uint32_t len, void (*wait_function)(void)){
	ret_code_t err_code;

	NRF_LOG_INFO("Writing to 0x%x in flash.", dest);
	err_code = nrf_fstorage_write(p_m_fstorage, dest, p_src, len, NULL);

	if(err_code != NRF_SUCCESS){
		NRF_LOG_ERROR("nrf_fstorage_write() returned: %s\r\n", nrf_strerror_get(err_code));
		return;
	}

	//wait_for_flash_ready(p_m_fstorage, wait_function);
}

void dk_flash_storage_erase(nrf_fstorage_t * p_m_fstorage, uint32_t addr, uint32_t pages_cnt)
{
	ret_code_t err_code;
	err_code = nrf_fstorage_erase(p_m_fstorage, addr, pages_cnt, NULL);
	
	if (err_code != NRF_SUCCESS)
	{
		NRF_LOG_ERROR("nrf_fstorage_erase() returned: %s\r\n", nrf_strerror_get(err_code));
		return;
	}

	//wait_for_flash_ready(p_m_fstorage, wait_function);
}


/**@brief   Helper function to obtain the last address on the last page of the on-chip flash that
 *          can be used to write user data.
 */
/*static uint32_t nrf5_flash_end_addr_get()
{
	uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];
	uint32_t const page_sz         = NRF_FICR->CODEPAGESIZE;
	uint32_t const code_sz         = NRF_FICR->CODESIZE;

	return (bootloader_addr != 0xFFFFFFFF ?
			bootloader_addr : (code_sz * page_sz));
}

static void print_flash_info(nrf_fstorage_t * p_fstorage)
{
	NRF_LOG_INFO("========| flash info |========");
	NRF_LOG_INFO("erase unit: \t%d bytes",      p_fstorage->p_flash_info->erase_unit);
	NRF_LOG_INFO("program unit: \t%d bytes",    p_fstorage->p_flash_info->program_unit);
	NRF_LOG_INFO("==============================");
}*/
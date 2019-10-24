/**
 * @file        tlv320aic3106.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TLV320AIC3106 audio CODEC driver.
 * @version     0.1
 * @date        2019-10-02
 * 
 * @copyright   Copyright (c) 2019 Danius Kalvaitis
 * 
 */

#include "tlv320aic3106.h"
#include "tlv320aic3106-internal.h"
#include "sdk_errors.h"
#include "sdk_macros.h"

static ret_code_t tlc320aic3106_twi_read(nrfx_twi_t   * p_i2c_instance,
                                         uint8_t        i2c_address,
                                         uint8_t        reg,
                                         uint8_t      * p_data,
                                         uint8_t        data_length)
{
	
}

static ret_code_t tlc320aic3106_twi_write(nrfx_twi_t  * p_i2c_instance, 
                                          uint8_t       i2c_address, 
                                          uint8_t       reg, 
                                          uint8_t     * p_data,
                                          uint8_t       data_length)
{
	
}

static ret_code_t tlv320aic3106_page_select_get(tlv320aic3106_t             * p_tlv320aic3106, 
                                                tlv320aic3106_active_page_t * p_page)
{
	return tlc320aic3106_twi_read(p_tlv320aic3106->p_twi_instance,
	                              p_tlv320aic3106->i2c_address,
	                              TLV320AIC3106_PAGE_SELECT,
	                              (uint8_t *) p_page,
	                              sizeof(tlv320aic3106_active_page_t));
}

static ret_code_t tlv320aic3106_page_select_set(tlv320aic3106_t           * p_tlv320aic3106, 
                                                tlv320aic3106_active_page_t page)
{
	ret_code_t err_code;
	tlv320aic3106_active_page_t page_check;

	if(p_tlv320aic3106->active_page == page)
	{
		return NRF_SUCCESS;
	}

	err_code = tlc320aic3106_twi_write(p_tlv320aic3106->p_twi_instance,
	                                   p_tlv320aic3106->i2c_address,
	                                   TLV320AIC3106_PAGE_SELECT,
	                                   (uint8_t *) &page,
	                                   sizeof(page));
	VERIFY_SUCCESS(err_code);

	err_code = tlv320aic3106_page_select_get(p_tlv320aic3106, &page_check);
	VERIFY_SUCCESS(err_code);

	if(page_check == page)
	{
		p_tlv320aic3106->active_page = page;
		return NRF_SUCCESS;
	}
	else
	{
		return NRF_ERROR_INTERNAL;
	}
}

ret_code_t tlv320aic3106_init(tlv320aic3106_t * p_tlv320aic3106)
{
	
}

ret_code_t tlv320aic3106_soft_rst(tlc320aic3106_t * p_tlv320aic3106)
{
	return tlc320aic3106_twi_write()
}

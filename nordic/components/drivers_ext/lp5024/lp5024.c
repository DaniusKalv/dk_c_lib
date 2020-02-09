/**
 * @file        lp5024.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       LP5024 LED driver.
 * @version     0.1
 * @date        2019-07-02
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "lp5024.h"
#include "lp5024-internal.h"

#define NRF_LOG_MODULE_NAME lp5024
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

/**
 * @brief   Macro for checking if I2C address has to be set to broadcast.
 * 
 * @param   _broadcast  Broadcast value.
 */
#define LP5024_BRADCAST_PROCESS(_broadcast)     \
uint8_t i2c_address = p_lp5024->i2c_address;    \
if(_broadcast)                                  \
{                                               \
	i2c_address = LP5024_I2C_BROADCAST_ADDRESS; \
}

/**
 * @brief       Write to I2C slave.
 * 
 * @param[in]   p_i2c_instance  Pointer to I2C instance. 
 * @param[in]   i2c_address     I2C address.
 * @param[in]   reg             Register address. 
 * @param[in]   p_data          Pointer to write data. 
 * @param[in]   data_length     Data length.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by nrfx_twi_tx.
 */
static ret_code_t twi_write(nrfx_twi_t * p_i2c_instance, uint8_t i2c_address, uint8_t reg, uint8_t * p_data, uint8_t data_length)
{
	lp5024_twi_write_t lp5024_write;

	lp5024_write.reg_address = reg;
	memcpy(lp5024_write.data, p_data, data_length);

	data_length++; //Add the size of register byte

	return nrfx_twi_tx(p_i2c_instance, i2c_address, (uint8_t *)&lp5024_write, data_length, false);
}

ret_code_t lp5024_init(lp5024_t * p_lp5024, bool broadcast, lp5024_config_t * p_lp5024_config)
{
	ret_code_t err_code;

	NRF_LOG_INFO("Initialising LP5024");

	LP5024_BRADCAST_PROCESS(broadcast);

	err_code = lp5024_reset(p_lp5024, broadcast);
	VERIFY_SUCCESS(err_code);

	return twi_write(p_lp5024->p_i2c_instance, i2c_address, LP5024_DEVICE_CONFIG0, (uint8_t *)p_lp5024_config, sizeof(lp5024_config_t));
}

ret_code_t lp5024_set_led_config0(lp5024_t * p_lp5024, bool broadcast, uint8_t * p_led_config0)
{
	ret_code_t err_code;
	LP5024_BRADCAST_PROCESS(broadcast);

	err_code = twi_write(p_lp5024->p_i2c_instance, i2c_address, LP5024_LED_CONFIG0, p_led_config0, sizeof(uint8_t));
	VERIFY_SUCCESS(err_code);

	p_lp5024->config.led_config0 = *p_led_config0;

	return NRF_SUCCESS;
}

ret_code_t lp5024_led_config0_en(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, bool enable)
{
	uint8_t led_config = p_lp5024->config.led_config0;

	if(enable)
	{
		led_config |= 1 << led;
	}
	else
	{
		led_config &= ~(1 << led);
	}
	
	return lp5024_set_led_config0(p_lp5024, broadcast, &led_config);
}

ret_code_t lp5024_set_bank_rgb(lp5024_t * p_lp5024, bool broadcast, lp5024_rgb_t * p_rgb)
{
	LP5024_BRADCAST_PROCESS(broadcast);

	return twi_write(p_lp5024->p_i2c_instance, i2c_address, LP5024_BANK_A_COLOR, (uint8_t *)p_rgb, sizeof(lp5024_rgb_t));
}

ret_code_t lp5024_set_led_rgb(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, lp5024_rgb_t * p_rgb)
{
	uint8_t reg_address = LP5024_OUT0_COLOR + (led * 3);

	LP5024_BRADCAST_PROCESS(broadcast);

	return twi_write(p_lp5024->p_i2c_instance, i2c_address, reg_address, (uint8_t *)p_rgb, sizeof(lp5024_rgb_t));
}

ret_code_t lp5024_set_led_brightness(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, uint8_t brightness)
{
	uint8_t reg_address = LP5024_LED0_BRIGHTNESS + led;

	LP5024_BRADCAST_PROCESS(broadcast);

	return twi_write(p_lp5024->p_i2c_instance, i2c_address, reg_address, &brightness, sizeof(brightness));
}

ret_code_t lp5024_set_led_color(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, lp5024_color_t color, uint8_t color_brightness)
{
	uint8_t reg_address = LP5024_OUT0_COLOR + (led * 3) + color;

	LP5024_BRADCAST_PROCESS(broadcast);

	return twi_write(p_lp5024->p_i2c_instance, i2c_address, reg_address, &color_brightness, sizeof(color_brightness));
}

ret_code_t lp5024_reset(lp5024_t * p_lp5024, bool broadcast)
{
	uint8_t data = 0xFF;
	LP5024_BRADCAST_PROCESS(broadcast);

	return twi_write(p_lp5024->p_i2c_instance, i2c_address, LP5024_RESET, &data, sizeof(uint8_t));
}

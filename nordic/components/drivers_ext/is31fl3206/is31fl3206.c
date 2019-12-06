/**
 * @file        is31fl3206.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       IS31FL3206 LED driver driver :)
 * @version     0.1
 * @date        2019-11-18
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "is31fl3206.h"
#include "is31fl3206-internal.h"

#define NRF_LOG_MODULE_NAME IS31FL3206
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#define IS31FL3206_TWI_BUFFER_SIZE      IS31FL3206_OUT_AMOUNT

typedef struct 
{
	uint8_t reg_address;                        /**< Register address. */
	uint8_t data[IS31FL3206_TWI_BUFFER_SIZE];   /**< Data buffer. */
} is31fl3206_twi_write_t;

static void twi_mngr_callback(ret_code_t result, void * p_user_data)
{
	NRF_LOG_INFO("Result: %x", result);
}

static ret_code_t twi_write(is31fl3206_t const * p_is31fl3206,
                            uint8_t reg,
                            uint8_t * p_data,
                            uint8_t data_length)
{
	is31fl3206_twi_write_t is31fl3206_write;

	is31fl3206_write.reg_address = reg;
	memcpy(is31fl3206_write.data, p_data, data_length);

	data_length++; //Add the size of register address byte

	dk_twi_mngr_transaction_t twi_transaction =
	{
		.i2c_address    = p_is31fl3206->i2c_address,
		.p_data         = (uint8_t *)&is31fl3206_write,
		.data_length    = data_length,
		.callback       = twi_mngr_callback,
		.p_user_data    = (void *)p_is31fl3206
	};
	
	return dk_twi_mngr_schedule(p_is31fl3206->p_dk_twi_mngr_instance,
	                             &twi_transaction);

	// return nrf_twi_mngr_schedule(p_is31fl3206->p_twi_mngr_instance,
	//                              &twi_transaction);
	// return nrf_twi_mngr_perform(p_is31fl3206->p_twi_mngr_instance, NULL, &twi_transfer, 1, NULL);
}

ret_code_t is31fl3206_init(is31fl3206_t * p_is31fl3206)
{
	NRF_LOG_INFO("Initialising IS31FL3206");

	return is31fl3206_reset(p_is31fl3206);
}

ret_code_t is31fl3206_shutdown(is31fl3206_t * p_is31fl3206, bool shutdown)
{
	is31fl3206_shutdown_t shutdown_reg;
	memset(&shutdown_reg, 0, sizeof(shutdown_reg));

	shutdown_reg.normal_operation = !shutdown;

	return twi_write(p_is31fl3206, 
	                 IS31FL3206_SHUTDOWN,
	                 (uint8_t *)&shutdown_reg,
	                 sizeof(shutdown_reg));
}

ret_code_t is31fl3206_set_out_pwm(is31fl3206_t * p_is31fl3206,
                                  is31fl3206_out_t out,
                                  uint8_t pwm)
{
	return twi_write(p_is31fl3206,
	                 IS31FL3206_PWM0 + out,
	                 &pwm,
	                 sizeof(pwm));
}

ret_code_t is31fl3206_update(is31fl3206_t * p_is31fl3206)
{
	uint8_t update_val = IS31FL3206_UPDATE_VAL;
	return twi_write(p_is31fl3206,
	                 IS31FL3206_UPDATE,
	                 &update_val,
	                 sizeof(update_val));
}

ret_code_t is31fl3206_set_out_current(is31fl3206_t * p_is31fl3206,
	                                  is31fl3206_out_t out,
	                                  is31fl3206_led_current_t led_current)
{
	is31fl3206_led_ctrl_t led_current_reg;
	memset(&led_current_reg, 0, sizeof(led_current_reg));

	led_current_reg.led_current = led_current;

	return twi_write(p_is31fl3206,
	                 IS31FL3206_LED_CTRL0 + out,
	                 (uint8_t *)&led_current_reg,
	                 sizeof(led_current_reg));
}

ret_code_t is31fl3206_shutdown_leds(is31fl3206_t * p_is31fl3206,
                                    bool shutdown_leds)
{
	is31fl3206_global_ctrl_t global_ctrl;
	memset(&global_ctrl, 0, sizeof(global_ctrl));

	global_ctrl.shutdown_leds = shutdown_leds;

	return twi_write(p_is31fl3206,
	                 IS31FL3206_GLOBAL_CTRL,
	                 (uint8_t *)&global_ctrl,
	                 sizeof(global_ctrl));
}

ret_code_t is31fl3206_set_out_frequency(is31fl3206_t * p_is31fl3206,
										is31fl3206_ofs_t output_frequency)
{
	is31fl3206_out_frequency_t output_frequency_reg;
	memset(&output_frequency_reg, 0, sizeof(output_frequency_reg));

	output_frequency_reg.ofs = output_frequency;
	
	return twi_write(p_is31fl3206,
	                 IS31FL3206_OUT_FREQUENCY,
	                 (uint8_t *)&output_frequency_reg,
	                 sizeof(output_frequency_reg));
}

ret_code_t is31fl3206_reset(is31fl3206_t * p_is31fl3206)
{
	uint8_t reset_val = IS31FL3206_RESET_VAL;

	return twi_write(p_is31fl3206,
	                 IS31FL3206_RESET,
	                 &reset_val,
	                 sizeof(reset_val));
}
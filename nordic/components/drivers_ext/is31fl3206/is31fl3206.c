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
                            is31fl3206_twi_write_t * p_twi_write,
                            uint8_t buffer_size)
{
	dk_twi_mngr_transaction_t twi_transaction =
	{
		.callback           = twi_mngr_callback,
		.p_user_data        = (void *)p_is31fl3206,
		.transfer.p_data    = (uint8_t *) p_twi_write,
		.transfer.length    = buffer_size,
		.transfer.operation = DK_TWI_MNGR_WRITE_OP(p_is31fl3206->i2c_address),
		.transfer.flags     = 0
	};

	return dk_twi_mngr_schedule(p_is31fl3206->p_dk_twi_mngr_instance,
	                             &twi_transaction);
}

ret_code_t is31fl3206_init(is31fl3206_t * p_is31fl3206)
{
	NRF_LOG_INFO("Initialising IS31FL3206");

	return is31fl3206_reset(p_is31fl3206);
}

ret_code_t is31fl3206_shutdown(is31fl3206_t * p_is31fl3206, bool shutdown)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_shutdown_data, sizeof(is31fl3206_shutdown_t));

	p_shutdown_data->reg_address = IS31FL3206_SHUTDOWN;
	is31fl3206_shutdown_t * p_shutdown_reg = (is31fl3206_shutdown_t *)p_shutdown_data->data;

	memset(p_shutdown_data, 0, sizeof(is31fl3206_shutdown_t));
	p_shutdown_reg->normal_operation = !shutdown;

	return twi_write(p_is31fl3206, 
	                 p_shutdown_data,
	                 p_shutdown_data_size);
}

ret_code_t is31fl3206_set_out_pwm(is31fl3206_t * p_is31fl3206,
                                  is31fl3206_out_t out,
                                  uint8_t pwm)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_pwm_data, sizeof(uint8_t));

	p_pwm_data->reg_address = IS31FL3206_PWM0 + out;
	p_pwm_data->data[0] = pwm;

	return twi_write(p_is31fl3206,
	                 p_pwm_data,
	                 p_pwm_data_size);
}

ret_code_t is31fl3206_update(is31fl3206_t * p_is31fl3206)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_update_data, sizeof(uint8_t));

	p_update_data->reg_address = IS31FL3206_UPDATE;
	p_update_data->data[0] = IS31FL3206_UPDATE_VAL;

	return twi_write(p_is31fl3206,
	                 p_update_data,
	                 p_update_data_size);
}

ret_code_t is31fl3206_set_out_current(is31fl3206_t * p_is31fl3206,
	                                  is31fl3206_out_t out,
	                                  is31fl3206_led_current_t led_current)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_led_current_data, sizeof(is31fl3206_led_ctrl_t));

	p_led_current_data->reg_address = IS31FL3206_LED_CTRL0 + out;
	is31fl3206_led_ctrl_t * p_led_current_reg = (is31fl3206_led_ctrl_t *)p_led_current_data->data;

	memset(p_led_current_reg, 0, sizeof(is31fl3206_led_ctrl_t));

	p_led_current_reg->led_current = led_current;

	return twi_write(p_is31fl3206,
	                 p_led_current_data,
	                 p_led_current_data_size);
}

ret_code_t is31fl3206_shutdown_leds(is31fl3206_t * p_is31fl3206,
                                    bool shutdown_leds)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_global_ctrl_data, sizeof(is31fl3206_global_ctrl_t));

	p_global_ctrl_data->reg_address = IS31FL3206_GLOBAL_CTRL;
	is31fl3206_global_ctrl_t * p_global_ctrl = (is31fl3206_global_ctrl_t *)p_global_ctrl_data->data;

	memset(p_global_ctrl, 0, sizeof(is31fl3206_global_ctrl_t));

	p_global_ctrl->shutdown_leds = shutdown_leds;

	return twi_write(p_is31fl3206,
	                 p_global_ctrl_data,
	                 p_global_ctrl_data_size);
}

ret_code_t is31fl3206_set_out_frequency(is31fl3206_t * p_is31fl3206,
                                        is31fl3206_ofs_t output_frequency)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_out_freq_data, sizeof(is31fl3206_out_frequency_t));

	p_out_freq_data->reg_address = IS31FL3206_OUT_FREQUENCY;
	is31fl3206_out_frequency_t * p_output_frequency_reg = (is31fl3206_out_frequency_t *)p_out_freq_data->data;

	memset(p_output_frequency_reg, 0, sizeof(is31fl3206_out_frequency_t));
	p_output_frequency_reg->ofs = output_frequency;

	return twi_write(p_is31fl3206,
	                 p_out_freq_data,
	                 p_out_freq_data_size);
}

ret_code_t is31fl3206_reset(is31fl3206_t * p_is31fl3206)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_reset_data, sizeof(uint8_t));

	p_reset_data->reg_address = IS31FL3206_RESET;
	p_reset_data->data[0] = IS31FL3206_RESET_VAL;

	return twi_write(p_is31fl3206,
	                 p_reset_data,
	                 p_reset_data_size);
}
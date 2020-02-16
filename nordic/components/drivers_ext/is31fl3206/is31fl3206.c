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
#include "is31fl3206-gamma.h"

#define NRF_LOG_MODULE_NAME IS31FL3206
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#define IS31FL3206_TWI_BUFFER_SIZE      IS31FL3206_OUT_AMOUNT

typedef struct
{
	uint8_t reg_address;                        /**< Register address. */
	uint8_t data[IS31FL3206_TWI_BUFFER_SIZE];   /**< Data buffer. */
} is31fl3206_twi_write_t;

#if(DK_CHECK(DK_IS31FL3206_GAMMA_ENABLED))
#if(DK_IS31FL3206_GAMMA_STEPS == 32)
static uint8_t m_gamma[] = IS31FL3206_GAMMA32;
#elif(DK_IS31FL3206_GAMMA_STEPS == 64)
static uint8_t m_gamma[] = IS31FL3206_GAMMA64;
#else
#error DK_IS31FL3206_GAMMA_STEPS defined incorrectly or not defined
#endif
#endif

static void twi_mngr_callback(ret_code_t result, void * p_user_data)
{
	if(result != NRF_SUCCESS)
	{
		is31fl3206_t * p_is31fl3206 = (is31fl3206_t *)p_user_data;

		NRF_LOG_ERROR("Error: 0x%x", result);
		if(p_is31fl3206->error_handler)
		{
			p_is31fl3206->error_handler(result, p_is31fl3206);
		}
	}
}

static ret_code_t twi_write(is31fl3206_t const * p_is31fl3206,
                            is31fl3206_twi_write_t * p_twi_write,
                            uint8_t buffer_size)
{
	dk_twi_mngr_transaction_t twi_transaction =
	{
		.callback           = twi_mngr_callback,
		.p_user_data        = (void *)p_is31fl3206,
		.transfer           = DK_TWI_MNGR_TX(p_is31fl3206->i2c_address, p_twi_write, buffer_size, 0)
	};

	return dk_twi_mngr_schedule(p_is31fl3206->p_dk_twi_mngr_instance,
	                             &twi_transaction);
}

ret_code_t is31fl3206_init(is31fl3206_t                 * p_is31fl3206,
                           is31fl3206_all_out_current_t * p_all_out_current,
                           is31fl3206_error_handler_t     error_handler)
{
	ret_code_t err_code;
	NRF_LOG_INFO("Initialising IS31FL3206");

	p_is31fl3206->error_handler = error_handler;

	err_code = is31fl3206_reset(p_is31fl3206);
	VERIFY_SUCCESS(err_code);

	if(p_all_out_current)
	{
		err_code = is31fl3206_set_all_out_current(p_is31fl3206, p_all_out_current);
		VERIFY_SUCCESS(err_code);
	}


	return NRF_SUCCESS;
}

ret_code_t is31fl3206_shutdown(is31fl3206_t * p_is31fl3206, bool shutdown)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_shutdown_data, sizeof(is31fl3206_shutdown_t));

	p_shutdown_data->reg_address = IS31FL3206_SHUTDOWN;
	is31fl3206_shutdown_t * p_shutdown_reg = (is31fl3206_shutdown_t *)p_shutdown_data->data;

	memset(p_shutdown_reg, 0, sizeof(is31fl3206_shutdown_t));
	p_shutdown_reg->normal_operation = !shutdown;

	return twi_write(p_is31fl3206, 
	                 p_shutdown_data,
	                 p_shutdown_data_size);
}

#if(DK_CHECK(DK_IS31FL3206_GAMMA_ENABLED))
ret_code_t is31fl3206_set_out_pwm(is31fl3206_t    * p_is31fl3206,
                                  is31fl3206_out_t  out,
                                  uint8_t           pwm,
                                  bool              gamma_correction)
#else
ret_code_t is31fl3206_set_out_pwm(is31fl3206_t    * p_is31fl3206,
                                  is31fl3206_out_t  out,
                                  uint8_t           pwm)
#endif
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_pwm_data, sizeof(uint8_t));

	p_pwm_data->reg_address = IS31FL3206_PWM0 + out;

#if(DK_CHECK(DK_IS31FL3206_GAMMA_ENABLED))
	if(pwm >= sizeof(m_gamma))
	{
		pwm = sizeof(m_gamma) - 1;
	}

	p_pwm_data->data[0] = m_gamma[pwm];
#else
	p_pwm_data->data[0] = pwm;
#endif

	return twi_write(p_is31fl3206,
	                 p_pwm_data,
	                 p_pwm_data_size);
}

#if(DK_CHECK(DK_IS31FL3206_GAMMA_ENABLED))
ret_code_t is31fl3206_set_all_out_pwm(is31fl3206_t                * p_is31fl3206,
                                      is31fl3206_all_out_pwm_t    * p_all_out_pwm,
                                      bool                          gamma_correction)
#else
ret_code_t is31fl3206_set_all_out_pwm(is31fl3206_t                * p_is31fl3206,
                                      is31fl3206_all_out_pwm_t    * p_all_out_pwm)
#endif
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_out_pwm_data, sizeof(is31fl3206_all_out_pwm_t));

	p_out_pwm_data->reg_address = IS31FL3206_PWM0;
	memcpy(p_out_pwm_data->data, p_all_out_pwm->pwm, sizeof(is31fl3206_all_out_pwm_t));

#if(DK_CHECK(DK_IS31FL3206_GAMMA_ENABLED))
	for(uint8_t i = 0; i < sizeof(is31fl3206_all_out_pwm_t); i++)
	{
		if(p_out_pwm_data->data[i] >= sizeof(m_gamma))
		{
			p_out_pwm_data->data[i] = sizeof(m_gamma) - 1;
		}

		p_out_pwm_data->data[i] = m_gamma[p_out_pwm_data->data[i]];
	}
#endif

	return twi_write(p_is31fl3206,
	                 p_out_pwm_data,
	                 p_out_pwm_data_size);
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
	                                  is31fl3206_out_current_t out_current)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_out_current_data, sizeof(is31fl3206_led_ctrl_t));

	p_out_current_data->reg_address = IS31FL3206_LED_CTRL0 + out;
	is31fl3206_led_ctrl_t * p_out_current_reg = (is31fl3206_led_ctrl_t *)p_out_current_data->data;

	p_out_current_reg->out_current = out_current;

	return twi_write(p_is31fl3206,
	                 p_out_current_data,
	                 p_out_current_data_size);
}

ret_code_t is31fl3206_set_all_out_current(is31fl3206_t                 * p_is31fl3206,
                                          is31fl3206_all_out_current_t * p_all_out_current)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_all_out_current_data, sizeof(is31fl3206_all_out_current_t));

	p_all_out_current_data->reg_address = IS31FL3206_LED_CTRL0;
	memcpy(p_all_out_current_data->data, p_all_out_current, sizeof(is31fl3206_all_out_current_t));

	return twi_write(p_is31fl3206,
	                 p_all_out_current_data,
	                 p_all_out_current_data_size);
}

ret_code_t is31fl3206_shutdown_outputs(is31fl3206_t * p_is31fl3206,
                                       bool shutdown_outputs)
{
	DK_TWI_MNGR_BUFF_ALLOC(is31fl3206_twi_write_t, p_global_ctrl_data, sizeof(is31fl3206_global_ctrl_t));

	p_global_ctrl_data->reg_address = IS31FL3206_GLOBAL_CTRL;
	is31fl3206_global_ctrl_t * p_global_ctrl = (is31fl3206_global_ctrl_t *)p_global_ctrl_data->data;

	memset(p_global_ctrl, 0, sizeof(is31fl3206_global_ctrl_t));

	p_global_ctrl->shutdown_leds = shutdown_outputs;

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
/**
 * @file        dk_battery_lvl.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Battery level measuring module.
 * @version     0.1
 * @date        2019-11-24
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_lib_common.h"
#if DK_MODULE_ENABLED(DK_BATTERY_LVL)

#include "dk_battery_lvl.h"
#include "nrfx_saadc.h"
#include "dk_config.h"
#include "dk_common.h"
#include "sdk_macros.h"
#include "app_timer.h"

#define NRF_LOG_MODULE_NAME DK_BATTERY_LVL
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

APP_TIMER_DEF(m_saadc_timeout_timer_id);

static nrf_saadc_value_t m_saadc_buffer;
#define SAADC_BUFFER_SAMPLES 1

static void saadc_callback(nrfx_saadc_evt_t const * p_event){
	if (p_event->type == NRFX_SAADC_EVT_DONE){
		ret_code_t err_code;

		err_code = nrfx_saadc_buffer_convert(p_event->data.done.p_buffer, SAADC_BUFFER_SAMPLES);
		VERIFY_SUCCESS_VOID(err_code);

		for(uint8_t i = 0; i < p_event->data.done.size; i++){
			NRF_LOG_INFO("Analog value %d", p_event->data.done.p_buffer[i]);
			float voltage = ((float)p_event->data.done.p_buffer[i])*6/255;
			NRF_LOG_INFO("Voltage " NRF_LOG_FLOAT_MARKER, NRF_LOG_FLOAT(voltage));
		}
	}
}

static ret_code_t saadc_channel_init(uint8_t saadc_channel){
	ret_code_t err_code;
	nrf_saadc_channel_config_t channel_config =
	{
		.resistor_p = NRF_SAADC_RESISTOR_DISABLED,
		.resistor_n = NRF_SAADC_RESISTOR_DISABLED,
		.gain       = NRF_SAADC_GAIN1_2,
		.reference  = NRF_SAADC_REFERENCE_INTERNAL,
		.acq_time   = NRF_SAADC_ACQTIME_10US,
		.mode       = NRF_SAADC_MODE_SINGLE_ENDED,
		.burst      = NRF_SAADC_BURST_DISABLED,
		.pin_p      = (nrf_saadc_input_t)(SAADC_CH_PSELP_PSELP_VDDHDIV5),
		.pin_n      = NRF_SAADC_INPUT_DISABLED
	};

	err_code = nrfx_saadc_channel_init(saadc_channel, &channel_config);
	VERIFY_SUCCESS(err_code);

	return nrfx_saadc_buffer_convert(&m_saadc_buffer, SAADC_BUFFER_SAMPLES);
}

static ret_code_t saadc_init()
{
	nrfx_saadc_config_t saadc_config =
	{
		.resolution         = (nrf_saadc_resolution_t)NRFX_SAADC_CONFIG_RESOLUTION,
		.oversample         = (nrf_saadc_oversample_t)NRFX_SAADC_CONFIG_OVERSAMPLE,
		.interrupt_priority = NRFX_SAADC_CONFIG_IRQ_PRIORITY,
		.low_power_mode     = NRFX_SAADC_CONFIG_LP_MODE
	};

	return nrfx_saadc_init(&saadc_config, saadc_callback);
}

ret_code_t dk_battery_lvl_init(dk_battery_lvl_t * p_dk_battery_lvl)
{
	ret_code_t err_code;
	VERIFY_PARAM_NOT_NULL(p_dk_battery_lvl);

	#if DK_CHECK(DK_BATTERY_LVL_SAADC_INIT)
		err_code = saadc_init();
		VERIFY_SUCCESS(err_code);
	#endif // DK_CHECK(DK_BATTERY_LVL_SAADC_INIT)

	return saadc_channel_init(p_dk_battery_lvl->saadc_channel);
}

#endif // DK_MODULE_ENABLED(DK_BATTERY_LVL)

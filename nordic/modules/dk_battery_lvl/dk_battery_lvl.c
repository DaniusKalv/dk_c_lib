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

#include "dk_battery_lvl.h"
#include "nrfx_saadc.h"

APP_TIMER_DEF(m_saadc_timeout_timer_id);

static nrf_saadc_value_t m_saadc_buffer;
#define SAADC_BUFFER_SAMPLES 1

static void saadc_callback(nrfx_saadc_evt_t const * p_event){
	if (p_event->type == NRFX_SAADC_EVT_DONE){
		ret_code_t err_code;

		err_code = nrfx_saadc_buffer_convert(p_event->data.done.p_buffer, SAADC_BUFFER_SAMPLES);
		APP_ERROR_CHECK(err_code);

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
	APP_ERROR_CHECK(err_code);

	err_code = nrfx_saadc_buffer_convert(&m_saadc_buffer, SAADC_BUFFER_SAMPLES);
	APP_ERROR_CHECK(err_code);
}

ret_code_t dk_battery_lvl_init(dk_battery_lvl_t * p_dk_battery_lvl, uint8_t saadc_channel)
{

}

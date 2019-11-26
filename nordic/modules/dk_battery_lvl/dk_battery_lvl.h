/**
 * @file        dk_battery_lvl.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Battery level measuring module.
 * @version     0.1
 * @date        2019-11-24
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_BATTERY_LVL
#define DK_BATTERY_LVL

#include "sdk_errors.h"

typedef struct
{
	uint8_t saadc_channel;
} dk_battery_lvl_t;

#define DK_BATTERY_LVL_DEF(_name, _saadc_channel)       \
static dk_battery_lvl_t _name =                         \
{                                                       \
	.saadc_channel = _saadc_channel,                    \
}

ret_code_t dk_battery_lvl_init(dk_battery_lvl_t * p_dk_battery_lvl);

#endif

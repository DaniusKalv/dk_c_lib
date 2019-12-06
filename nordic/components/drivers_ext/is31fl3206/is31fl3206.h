/**
 * @file        is31fl3206.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       IS31FL3206 LED driver driver :)
 * @version     0.1
 * @date        2019-11-18
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef IS31FL3206_H
#define IS31FL3206_H

#include "dk_twi_mngr.h"
#include "sdk_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	bool    normal_operation:1;
	uint8_t _padding0       :7;
} is31fl3206_shutdown_t;

typedef enum
{
	IS31FL3206_LED_CURRENT_0        = 0x00,
	IS31FL3206_LED_CURRENT_MAX      = 0x10,
	IS31FL3206_LED_CURRENT_11_12    = 0x11,
	IS31FL3206_LED_CURRENT_9_12     = 0x12,
	IS31FL3206_LED_CURRENT_7_12     = 0x13,
} is31fl3206_led_current_t;

typedef struct
{
	is31fl3206_led_current_t led_current;
} is31fl3206_led_ctrl_t;

typedef struct
{
	uint8_t _padding0       :7;
	bool    shutdown_leds   :1;
} is31fl3206_global_ctrl_t;

typedef enum
{
	IS31FL3206_OFS_24KHZ  = 0x00,
	IS31FL3206_OFS_3_6KHZ = 0x01,
} is31fl3206_ofs_t;

typedef struct
{
	uint8_t             _padding0   :7;
	is31fl3206_ofs_t    ofs         :1;
} is31fl3206_out_frequency_t;

typedef struct
{
	const dk_twi_mngr_t   * p_dk_twi_mngr_instance; /**< Pointer to TWI manager instance. */
	uint8_t                 i2c_address;            /**< Device I2C slave address. */
} is31fl3206_t;

#define IS31FL3206_DEF(_name, _p_dk_twi_mngr_instance, _i2c_address)    \
static is31fl3206_t _name =                                             \
{                                                                       \
	.p_dk_twi_mngr_instance = _p_dk_twi_mngr_instance,                  \
	.i2c_address = _i2c_address                                         \
};

typedef enum
{
	IS31FL3206_OUT1,
	IS31FL3206_OUT2,
	IS31FL3206_OUT3,
	IS31FL3206_OUT4,
	IS31FL3206_OUT5,
	IS31FL3206_OUT6,
	IS31FL3206_OUT7,
	IS31FL3206_OUT8,
	IS31FL3206_OUT9,
	IS31FL3206_OUT10,
	IS31FL3206_OUT11,
	IS31FL3206_OUT12,
	IS31FL3206_OUT_AMOUNT,
} is31fl3206_out_t;

ret_code_t is31fl3206_init(is31fl3206_t * p_is31fl3206);

ret_code_t is31fl3206_shutdown(is31fl3206_t * p_is31fl3206, bool shutdown);

ret_code_t is31fl3206_set_out_pwm(is31fl3206_t * p_is31fl3206,
                                  is31fl3206_out_t out,
                                  uint8_t pwm);

ret_code_t is31fl3206_update(is31fl3206_t * p_is31fl3206);

ret_code_t is31fl3206_set_out_current(is31fl3206_t * p_is31fl3206,
                                      is31fl3206_out_t out,
                                      is31fl3206_led_current_t led_current);

ret_code_t is31fl3206_shutdown_leds(is31fl3206_t * p_is31fl3206,
                                    bool shutdown_leds);

ret_code_t is31fl3206_set_out_frequency(is31fl3206_t * p_is31fl3206,
                                        is31fl3206_ofs_t output_frequency);

ret_code_t is31fl3206_reset(is31fl3206_t * p_is31fl3206);

#ifdef __cplusplus
}
#endif

#endif //IS31FL3206_H
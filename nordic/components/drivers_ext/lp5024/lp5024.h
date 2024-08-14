/**
 * @file        lp5024.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       LP5024 LED driver.
 * @version     0.1
 * @date        2019-07-02
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef LP5024_H
#define LP5024_H

#include <stdint.h>
#include "nrfx_twi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LP5024_TWI_BUFFER_SIZE  7   /**< Size of a twi TX buffer, maximum amount of registers to be accessed in one TX operation. */

/** @brief LED enumerator. */
typedef enum
{
	LP5024_LED0,        /**< LED0. */
	LP5024_LED1,        /**< LED1. */
	LP5024_LED2,        /**< LED2. */
	LP5024_LED3,        /**< LED3. */
	LP5024_LED4,        /**< LED4. */
	LP5024_LED5,        /**< LED5. */
	LP5024_LED6,        /**< LED6. */
	LP5024_LED7,        /**< LED7. */
	LP5024_LED_AMOUNT   /**< Amount of LEDs. */
} lp5024_led_t;

/** @brief Color enumerator. */
typedef enum
{
	LP5024_COLOR_BLUE,  /**< Blue. */
	LP5024_COLOR_GREEN, /**< Green. */
	LP5024_COLOR_RED    /**< Red. */
} lp5024_color_t;

/** @brief TWI write struct. */
typedef struct 
{
	uint8_t reg_address;                    /**< Register address. */
	uint8_t data[LP5024_TWI_BUFFER_SIZE];   /**< Data buffer. */
} lp5024_twi_write_t;

/** @brief Maximum current options. */
typedef enum
{
	LP5024_MAX_CURRENT_25_5MA   = 0,    /**< Max current = 25.5mA */
	LP5024_MAX_CURRENT_35MA     = 1     /**< Max current = 35mA */
} lp5024_max_current_t;

/** @brief Device configuration 0 register. */
typedef struct
{
	uint8_t _padding1   :6;
	bool    chip_en     :1; /**< 1 = LP5024 enabled, 0 = LP5024 disabled. */
	uint8_t _padding0   :1;
} lp5024_device_config0_t;

/** @brief Device configuration 1 register. */
typedef struct 
{
	bool                    led_global_off      :1; /**< Shut down all LEDs. */
	lp5024_max_current_t    max_current         :1; /**< Max current setting. */
	bool                    pwm_dithering_en    :1; /**< Enable PWM dithering. */
	bool                    auto_incr_en        :1; /**< Enable automatic address increment mode. */
	bool                    power_save_en       :1; /**< Enable automatic power-saving mode. */
	bool                    log_scale_en        :1; /**< Enable logarithmic scale dimming curve. */
	uint8_t                 _padding0           :2;
} lp5024_device_config1_t;

/** @brief LP5024 configuration struct. Contains all configuration registers. */
typedef struct 
{
	lp5024_device_config0_t device_config0;     /**< Device config 0 register. */
	lp5024_device_config1_t device_config1;     /**< Device config 1 register. */
	uint8_t                 led_config0;        /**< LED config 0 register. */
	uint8_t                 bank_brightness;    /**< Bank brightness register. */
} lp5024_config_t;

/** @brief LP5024 driver struct. */
typedef struct
{
	nrfx_twi_t        * p_i2c_instance; /**< Pointer to I2C instance. */
	uint8_t             i2c_address;    /**< Device I2C slave address. */
	lp5024_config_t     config;         /**< Device configuration. */
} lp5024_t;

/**@brief   Macro for defining a LP5024 instance.
 *
 * @param   _name               Name of the instance.
 * @param   _p_i2c_instance     Pointer to i2c instance.
 * @param   _i2c_address        LP5024 i2c address.
 * @hideinitializer
 */
#define LP5024_DEF(_name, _p_i2c_instance, _i2c_address)    \
static lp5024_t _name =                                     \
{                                                           \
	.p_i2c_instance = _p_i2c_instance,                      \
	.i2c_address = _i2c_address                             \
};

/** @brief RGB color. */
typedef struct
{
	uint8_t blue;   /**< Amount of blue. */
	uint8_t green;  /**< Amount of green. */
	uint8_t red;    /**< Amount of red. */
} lp5024_rgb_t;

/**
 * @brief       Initialize LP5024 instance.
 * 
 * @param[in]   p_lp5024        Pointer to LP5024 instance.
 * @param[in]   broadcast       Initialize using broadcast address. Can be used to initialize multiple devices with one function call.
 * @param[in]   p_lp5024_config Pointer to LP5024 configuration.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by internal functions.
 */
ret_code_t lp5024_init(lp5024_t * p_lp5024, bool broadcast, lp5024_config_t * p_lp5024_config);

/**
 * @brief       Set led config 0 register value.
 * 
 * @param[in]   p_lp5024        Pointer to LP5024 instance.
 * @param[in]   broadcast       Use broadcast address. Can be used to write to multiple devices with one function call.
 * @param[in]   p_led_config0   Pointer to LED config 0 value.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by internal functions.
 */
ret_code_t lp5024_set_led_config0(lp5024_t * p_lp5024, bool broadcast, uint8_t * p_led_config0);

/**
 * @brief       Enable/disable specified LED in LED configuration 0 register.
 * 
 * @param[in]   p_lp5024        Pointer to LP5024 instance.
 * @param[in]   broadcast       Use broadcast address. Can be used to write to multiple devices with one function call.
 * @param[in]   led             LED to process.
 * @param[in]   enable          True = enable specified LED, false = disable specified LED.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by internal functions.
 */
ret_code_t lp5024_led_config0_en(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, bool enable);

/**
 * @brief       Set bank RGB color.
 * 
 * @param[in]   p_lp5024        Pointer to LP5024 instance.
 * @param[in]   broadcast       Use broadcast address. Can be used to write to multiple devices with one function call.
 * @param[in]   p_rgb           Pointer to RGB color.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by internal functions.
 */
ret_code_t lp5024_set_bank_rgb(lp5024_t * p_lp5024, bool broadcast, lp5024_rgb_t * p_rgb);

/**
 * @brief       Set specified LED RGB color.
 * 
 * @param[in]   p_lp5024        Pointer to LP5024 instance.
 * @param[in]   broadcast       Use broadcast address. Can be used to write to multiple devices with one function call.
 * @param[in]   led             LED to process.
 * @param[in]   p_rgb           Pointer to RGB color.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by internal functions.
 */
ret_code_t lp5024_set_led_rgb(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, lp5024_rgb_t * p_rgb);

/**
 * @brief       Set specified LED brightness.
 * 
 * @param[in]   p_lp5024        Pointer to LP5024 instance.
 * @param[in]   broadcast       Use broadcast address. Can be used to write to multiple devices with one function call.
 * @param[in]   led             LED to process.
 * @param[in]   brightness      LED brightness (0-255).
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by internal functions.
 */
ret_code_t lp5024_set_led_brightness(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, uint8_t brightness);

/**
 * @brief       Set specified LED color brightness.
 * 
 * @param[in]   p_lp5024            Pointer to LP5024 instance.
 * @param[in]   broadcast           Use broadcast address. Can be used to write to multiple devices with one function call.
 * @param[in]   led                 LED to process.
 * @param[in]   color               Color to process.
 * @param[in]   color_brightness    Color brightness.
 * 
 * @retval      NRF_SUCCESS         On success.
 * @retval      Other               Error codes returned by internal functions.
 */
ret_code_t lp5024_set_led_color(lp5024_t * p_lp5024, bool broadcast, lp5024_led_t led, lp5024_color_t color, uint8_t color_brightness);

/**
 * @brief Reset LP5024
 * 
 * @param[in]   p_lp5024    Pointer to LP5024 instance.
 * @param[in]   broadcast   Use broadcast address. Can be used to write to multiple devices with one function call.
 * 
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by internal functions.
 */
ret_code_t lp5024_reset(lp5024_t * p_lp5024, bool broadcast);

#ifdef __cplusplus
}
#endif

#endif // LP5024_H

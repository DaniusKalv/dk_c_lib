/**
 * @file        tca9548a.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TCA548 driver.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef TCA9548A_H
#define TCA9548A_H

#include <stdint.h>

#include "nrfx_twi.h"
#include "sdk_errors.h"

/** @brief TCA9548A driver struct. */
typedef struct
{
    nrfx_twi_t *p_i2c_instance; /**< Pointer to I2C instance. */
    uint8_t     i2c_address;    /**< Device I2C address. */
} tca9548a_t;

/**@brief   Macro for defining a TCA9548A instance.
 *
 * @param   _name               Name of the instance.
 * @param   _p_i2c_instance     Pointer to i2c instance.
 * @param   _i2c_address        I2C address.
 * @hideinitializer
 */
#define TCA9548A_DEF(_name, _p_i2c_instance, _i2c_address)                                                             \
    static tca9548a_t _name = {.p_i2c_instance = _p_i2c_instance, .i2c_address = _i2c_address}

/** @brief TCA9548A channel control. */
typedef enum
{
    TCA9548A_DISABLE  = (0 << 0), /**< All channels disabled. */
    TCA9548A_CHANNEL0 = (1 << 0), /**< Channel 0 enabled. */
    TCA9548A_CHANNEL1 = (1 << 1), /**< Channel 1 enabled. */
    TCA9548A_CHANNEL2 = (1 << 2), /**< Channel 2 enabled. */
    TCA9548A_CHANNEL3 = (1 << 3), /**< Channel 3 enabled. */
    TCA9548A_CHANNEL4 = (1 << 4), /**< Channel 4 enabled. */
    TCA9548A_CHANNEL5 = (1 << 5), /**< Channel 5 enabled. */
    TCA9548A_CHANNEL6 = (1 << 6), /**< Channel 6 enabled. */
    TCA9548A_CHANNEL7 = (1 << 7)  /**< Channel 7 enabled. */
} tca9548a_channel_enable_t;

/**
 * @brief       Enable TCA9548A channel.
 *
 * @param[in]   p_tca9548a  Pointer to TCA9546A instance.
 * @param[in]   channel     Channel to enable.
 *
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by nrfx_twi_tx function.
 */
ret_code_t tca9548a_enable_channel(tca9548a_t *p_tca9548a, tca9548a_channel_enable_t channel);

#endif // TCA9548A_H

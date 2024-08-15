/**
 * @file        tca9548a.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TCA548 driver.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#include "tca9548a.h"

ret_code_t tca9548a_enable_channel(tca9548a_t *p_tca9548a, tca9548a_channel_enable_t channel)
{
    uint8_t data = channel;
    return nrfx_twi_tx(p_tca9548a->p_i2c_instance, p_tca9548a->i2c_address, &data, sizeof(data), false);
}

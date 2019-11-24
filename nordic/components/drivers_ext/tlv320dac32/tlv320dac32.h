/**
 * @file        tlv320dac32.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TLV320DAC32 codec driver.
 * @version     0.1
 * @date        2019-11-19
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef TLV320DAC32_H
#define TLV320DAC32_H

#include "nrfx_twi.h"
#include "sdk_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	nrfx_twi_t    * p_i2c_instance; /**< Pointer to I2C instance. */
	uint8_t         i2c_address;    /**< Device I2C slave address. */
} tlv320dac32_t;

#define TLV320DAC32_DEF(_name, _p_i2c_instance, _i2c_address)   \
static tlv320dac32_t _name =                                    \
{                                                               \
	.p_i2c_instance = _p_i2c_instance,                          \
	.i2c_address = _i2c_address                                 \
};

ret_code_t tlv320dac32_init(tlv320dac32_t * p_tlv320dac32);

#ifdef __cplusplus
}
#endif

#endif //TLV320DAC32_H

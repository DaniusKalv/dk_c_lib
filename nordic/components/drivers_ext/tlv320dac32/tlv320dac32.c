/**
 * @file        tlv320dac32.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       TLV320DAC32 codec driver.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#include "tlv320dac32.h"

#include "tlv320dac32-internal.h"

#define NRF_LOG_MODULE_NAME TLV320DAC32
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

typedef struct
{
    uint8_t reg_address; /**< Register address. */
    uint8_t data[5];     /**< Data buffer. */
} tlv320dac32_twi_write_t;

static ret_code_t twi_write(nrfx_twi_t *p_i2c_instance,
                            uint8_t     i2c_address,
                            uint8_t     reg,
                            uint8_t    *p_data,
                            uint8_t     data_length)
{
    tlv320dac32_twi_write_t tlv320dac32_write;

    tlv320dac32_write.reg_address = reg;
    memcpy(tlv320dac32_write.data, p_data, data_length);

    data_length++; // Add the size of register byte

    return nrfx_twi_tx(p_i2c_instance, i2c_address, (uint8_t *)&tlv320dac32_write, data_length, false);
}

ret_code_t tlv320dac32_init(tlv320dac32_t *p_tlv320dac32)
{
    NRF_LOG_INFO("Initialising TLV320DAC32");

    uint8_t dummy = 0;

    return twi_write(p_tlv320dac32->p_i2c_instance, p_tlv320dac32->i2c_address, 0x00, &dummy, sizeof(dummy));
}

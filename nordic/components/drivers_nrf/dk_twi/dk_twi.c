/**
 * @file        dk_twi.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#include "dk_twi.h"

#include "sdk_macros.h"

ret_code_t dk_twi_enable(nrfx_twi_t            *p_twi_instance,
                         uint32_t               scl_pin,
                         uint32_t               sda_pin,
                         nrfx_twi_evt_handler_t nrfx_twi_evt_handler,
                         void                  *p_context)
{
    ret_code_t err_code;

    nrfx_twi_config_t nrfx_twi_config = {.scl                = scl_pin,
                                         .sda                = sda_pin,
                                         .frequency          = NRFX_TWI_DEFAULT_CONFIG_FREQUENCY,
                                         .interrupt_priority = NRFX_TWI_DEFAULT_CONFIG_IRQ_PRIORITY};

    err_code = nrfx_twi_init(p_twi_instance, &nrfx_twi_config, nrfx_twi_evt_handler, p_context); // Initialize TWI
    VERIFY_SUCCESS(err_code);

    nrfx_twi_enable(p_twi_instance);                                                             // Enable TWI instance
    return NRF_SUCCESS;
}

void dk_twi_disable(nrfx_twi_t *p_twi_instance)
{
    nrfx_twi_uninit(p_twi_instance); // Deinitialize TWI, this is done instead of disable because when the TWI has to be
                                     // enabled again it has to be reinitialized as mentioned in ERRATA 89

    if (p_twi_instance->drv_inst_idx == 0)
    {
        *(volatile uint32_t *)0x40003FFC =
          0; // Apply workaround mentioned in ERRATA 89, this toggles the POWER register of the peripheral
        *(volatile uint32_t *)0x40003FFC;
        *(volatile uint32_t *)0x40003FFC = 1;
    } else if (p_twi_instance->drv_inst_idx == 1)
    {
        *(volatile uint32_t *)0x40004FFC = 0;
        *(volatile uint32_t *)0x40004FFC;
        *(volatile uint32_t *)0x40004FFC = 1;
    }
}

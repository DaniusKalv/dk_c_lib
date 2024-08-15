/**
 * @file        lp5024-internal.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       LP5024 LED driver internal defines.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef LP5024_INTERNAL_H
#define LP5024_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define LP5024_I2C_BROADCAST_ADDRESS 0x3C /**< I2C slave broadcast address. */

#define LP5024_DEVICE_CONFIG0        0x00 /**< Device configuration register 0. */
#define LP5024_DEVICE_CONFIG1        0x01 /**< Device configuration register 1. */
#define LP5024_LED_CONFIG0           0x02 /**< LED configuration 0 register. */

#define LP5024_BANK_A_COLOR          0x04 /**< Bank A color register. */

#define LP5024_LED0_BRIGHTNESS       0x07 /**< LED 0 brightness register. */

#define LP5024_OUT0_COLOR            0x0F /**< Output 0 color register. */

#define LP5024_RESET                 0x27 /**< Reset register. */

#ifdef __cplusplus
}
#endif

#endif // LP5024_INTERNAL_H

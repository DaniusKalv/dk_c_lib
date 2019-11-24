/**
 * @file dk_01013.h
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief Hardware description for Phil It Up v1.0.
 * 
 * @version 0.1
 * @date 2019-10-24
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_01013_H
#define DK_01013_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

//Force sensor
#define DK_BSP_FS_IN            3   /**< Force sensor input (analog) pin. */

//I2C interfaces
#define DK_BSP_I2C_SCL0         26  /**< I2C interface 0 SCL pin. */
#define DK_BSP_I2C_SDA0         4   /**< I2C interface 0 SDA pin. */

#define DK_BSP_I2C_SCL1         22  /**< I2C interface 1 SCL pin. */
#define DK_BSP_I2C_SDA1         20  /**< I2C interface 1 SDA pin. */

//IS31FL3206 LED driver
#define DK_BSP_IS31FL3206_EN            6   /**< IS31FL3206 LED driver enable pin. */
#define DK_BSP_IS31FL3206_I2C_ITERFACE  0   /**< I2C interface instance that IS31FL3206 is connected to. */

//MLX9061 infrared thermometer
#define DK_BSP_MLX9061_I2C_ITERFACE     0   /**< I2C interface instance that MLX9061 is connected to. */

//I2S interface
#define DK_BSP_I2S_WCLK     NRF_GPIO_PIN_MAP(0,24)  /**< I2S word clock pin. */
#define DK_BSP_I2S_DOUT     NRF_GPIO_PIN_MAP(0,23)  /**< I2S master data out pin. */
#define DK_BSP_I2S_MCLK     NRF_GPIO_PIN_MAP(0,25)  /**< I2S master clock pin. */
#define DK_BSP_I2S_BCLK     NRF_GPIO_PIN_MAP(1,0)   /**< I2S bit clock pin. */

//TLV320DAC codec
#define DK_BSP_TLV320_PWR_EN            NRF_GPIO_PIN_MAP(1,1)   /**< Enable voltage regulator for TLV320 pin. */
#define DK_BSP_TLV320_RST               NRF_GPIO_PIN_MAP(1,3)   /**< TLV320 reset pin (active low). */
#define DK_BSP_TLV320_I2C_ITERFACE      1                       /**< I2C interface instance that TLV320 is connected to. */

//I2C slave addresses
#define DK_BSP_IS31FL3206_I2C_ADDRESS   0x6C    /**< IS31FL3206 I2C address. */
#define DK_BSP_TLV320_I2C_ADDRESS       0x18    /**< TLV320 I2C address. */
#define DK_BSP_MLX9061_I2C_ADDRESS      0x5B    /**< MLX9061 I2C address. */

#ifdef __cplusplus
}
#endif

#endif //DK_01013_H

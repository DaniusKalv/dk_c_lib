/**
 * @file dk_01017.h
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief Hardware description for There Might Be Noise v1.0.
 * 
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_01017_H
#define DK_01017_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

//Oled
#define DK_BSP_OLED_CS          5   /**< Oled chip select (active low) pin. */
#define DK_BSP_OLED_RST         27  /**< Oled reset (active low) pin. */
#define DK_BSP_OLED_DC          7   /**< Oled data (high)/command (low) pin. */
#define DK_BSP_OLED_SCLK        26  /**< Oled serial clock pin. */
#define DK_BSP_OLED_MOSI        6   /**< Oled data pin. */

//I2C Interface
#define DK_BSP_I2C_SCL0         NRF_GPIO_PIN_MAP(1,8)   /**< SCL pin. */
#define DK_BSP_I2C_SDA0         NRF_GPIO_PIN_MAP(0,4)   /**< SDA pin. */

//I2S Interface
#define DK_BSP_I2S_DOUT         NRF_GPIO_PIN_MAP(1,9)   /**< I2S master data out pin. */
#define DK_BSP_I2S_DIN          NRF_GPIO_PIN_MAP(0,8)   /**< I2S master data in pin. */
#define DK_BSP_I2S_WCLK         NRF_GPIO_PIN_MAP(0,12)  /**< I2S word clock pin. */
#define DK_BSP_I2S_BCLK         NRF_GPIO_PIN_MAP(0,11)  /**< I2S bit clock pin. */
#define DK_BSP_I2S_MCLK         NRF_GPIO_PIN_MAP(0,14)  /**< I2S master clock pin. */

//Codec (TLV320AIC3106)
#define DK_BSP_TLV320_RST           16  /**< TLV320AIC3106 reset (active low) pin. */
#define DK_BSP_TLV320_INT           18  /**< TLV320AIC3106 interrupt pin. */
#define DK_BSP_TLV320_I2C_ITERFACE  0   /**< I2C interface instance that TLV320 is connected to. */

//Audio amplifier (TPA3220DDW)
#define DK_BSP_TPA3220_MUTE     17  /**< TPA3220DDW mute (active low) pin. */
#define DK_BSP_TPA3220_HEAD     19  /**< TPA3220DDW head pin. */
#define DK_BSP_TPA3220_RST      20  /**< TPA3220DDW reset (active low) pin. */
#define DK_BSP_TPA3220_FAULT    22  /**< TPA3220DDW fault (active low) pin. */
#define DK_BSP_TPA3220_OTW_CLIP 23  /**< TPA3220DDW clipping and over-temperature warning (active low) pin. */

//Rotary encoder
#define DK_BSP_ENCODER_A        24  /**< Encoder A pin. */
#define DK_BSP_ENCODER_B        21  /**< Encoder B pin. */
#define DK_BSP_ENCODER_SW       25  /**< Encoder switch (active low) pin. */

//I2C slave addresses
#define DK_BSP_TLV320_I2C_ADDRESS  0x18    /**< TLV320 I2C address. */

#ifdef __cplusplus
}
#endif

#endif //DK_01017_H

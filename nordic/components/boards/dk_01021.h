/**
 * @file        dk_01021.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Hardware description for Mr Pickle v1.0.
 * 
 * @version     0.1
 * @date        2019-10-24
 * 
 * @copyright   (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_01021_H
#define DK_01021_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// LED definitions
#define LEDS_NUMBER     4
#define LED1            28
#define LED2            2
#define LED3            3
#define LED4            11

#define LEDS_ACTIVE_STATE 0

#define LEDS_INV_MASK LEDS_MASK

#define LEDS_LIST {LED1, LED2, LED3, LED4}

#define BSP_LED_0      LED_1
#define BSP_LED_1      LED_2
#define BSP_LED_2      LED_3
#define BSP_LED_3      LED_4

// Button definitions
#define BUTTONS_NUMBER  2

#define BUTTON_1        21
#define BUTTON_2        25
#define BUTTON_PULL     NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST {BUTTON_1, BUTTON_2}

#define BSP_BUTTON_0   BUTTON_1
#define BSP_BUTTON_1   BUTTON_2

// Battery level measurement
#define DK_BSP_BATTERY_LVL      30  /**< Battery level input (analog) pin. */
#define DK_BSP_BATTERY_LVL_AIN  6   /**< Battery level analog input. */

// I2C interface
#define DK_BSP_I2C_SCL0         14  /**< I2C interface 0 SCL pin. */
#define DK_BSP_I2C_SDA0         15  /**< I2C interface 0 SDA pin. */

// LIS3MDL
#define DK_BSP_LIS3MDL_I2C_INTERFACE     0   /**< NRF52832 I2C interface instance that LIS3MDL is connected to. */
#define DK_BSP_LIS3MDL_I2C_SW_CHANNEL   6   /**< TCA548 I2C interface channel that LIS3MDL is connected to. */

#define DK_BSP_LIS3MDL_INT              29  /**< LIS3MDL interrupt pin. */

// LSM303
#define DK_BSP_LSM303_I2C_INTERFACE      0   /**< NRF52832 I2C interface instance that LSM303 is connected to. */
#define DK_BSP_LSM303_I2C_SW_CHANNEL    7   /**< TCA548 I2C interface channel that LSM303 is connected to. */

#define DK_BSP_LSM303_INT               12  /**< LSM303 interrupt pin. */

// LSM9DS1
#define DK_BSP_LSM9DS1_I2C_INTERFACE     0   /**< NRF52832 I2C interface instance that LSM9DS1 is connected to. */
#define DK_BSP_LSM9DS1_I2C_SW_CHANNEL   7   /**< TCA548 I2C interface channel that LSM9DS1 is connected to. */

#define DK_BSP_LSM9DS1_A_INT1           8   /**< LSM9DS1 accelerometer interrupt 1 pin. */
#define DK_BSP_LSM9DS1_A_INT2           4   /**< LSM9DS1 accelerometer interrupt 2 pin. */
#define DK_BSP_LSM9DS1_M_INT            6   /**< LSM9DS1 magnetometer interrupt pin. */

//I2C slave addresses
#define DK_BSP_I2C_ADDRESS_TCA9548      0x70    /**< TCA9548 I2C address. */
#define DK_BSP_I2C_ADDRESS_LIS3MDL      0x1E    /**< LIS3MDL I2C address. */
#define DK_BSP_I2C_ADDRESS_LSM303_A     0x1D    /**< LSM303 accelerometer I2C address. */
#define DK_BSP_I2C_ADDRESS_LSM303_M     0x1E    /**< LSM303 magnetometer I2C address. */
#define DK_BSP_I2C_ADDRESS_LSM9DS1_A    0x6A    /**< LSM9DS1 accelerometer I2C address. */
#define DK_BSP_I2C_ADDRESS_LSM9DS1_M    0x1C    /**< LSM9DS1 magnetometer I2C address. */

#ifdef __cplusplus
}
#endif

#endif //DK_01021_H

/**
 * @file        mlx90615-internal.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       MLX90615 register definitions.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef MLX90615_INTERNAL_H
#define MLX90615_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define MLX90615_SLAVE_ADDRESS       0x5B /**< Factory default I2C slave address. */

#define MLX90615_CMD_EEPROM_ADDR     0x10 /**< EEPROM command flag. */
#define MLX90615_CMD_RAM_ADDR        0x20 /**< RAM command flag. */

#define MLX90615_CMD_ENTER_SLEEP     0xC6 /**< Command to enter sleep mode. */
#define MLX90615_CMD_ENTER_SLEEP_PEC 0x6D /**< PEC value of sleep mode enter command. */

/**
 * @brief   Macro to encode an EEPROM command.
 *
 * @param   _address    Command address.
 */
#define MLX90615_CMD_EEPROM(_address) MLX90615_CMD_EEPROM_ADDR | _address

/**
 * @brief   Macro to encode a RAM command.
 *
 * @param   _address    Command address.
 */
#define MLX90615_CMD_RAM(_address) MLX90615_CMD_RAM_ADDR | _address

#define MLX90615_ID0               0xCBE0                    /**< MLX90615 ID. */

#define MLX90615_CMD_EEPROM_ID0    MLX90615_CMD_EEPROM(0x0E) /**< EEPROM ID0 command. */

#define MLX90615_CMD_RAM_T_AMB     MLX90615_CMD_RAM(0x06)    /**< RAM ambient temperature command. */
#define MLX90615_CMD_RAM_T_OBJ     MLX90615_CMD_RAM(0x07)    /**< RAM object temperature command. */

#ifdef __cplusplus
}
#endif

#endif // MLX90615_INTERNAL_H

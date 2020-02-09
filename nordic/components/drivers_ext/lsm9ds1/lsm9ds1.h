/**
 * @file        lsm9ds1.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       LSM9DS1 driver.
 * @version     0.1
 * @date        2020-02-09
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef LSM9DS1_H
#define LSM9DS1_H

#include <stdint.h>
#include "nrfx_twi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int16_t x_axis;     /**< Accelerometer data from x-axis */
	int16_t y_axis;     /**< Accelerometer data from y_axis */
	int16_t z_axis;     /**< Accelerometer data from z_axis */
} lsm9ds1_acc_data_t;

typedef struct {
	int16_t x_axis;     /**< Gyroscope data from x-axis */
	int16_t y_axis;     /**< Gyroscope data from y_axis */
	int16_t z_axis;     /**< Gyroscope data from z_axis */
} lsm9ds1_gyr_data_t;

typedef struct {
	int16_t x_axis;     /**< Magnetometer data from x-axis */
	int16_t y_axis;     /**< Magnetometer data from y_axis */
	int16_t z_axis;     /**< Magnetometer data from z_axis */
} lsm9ds1_mag_data_t;

typedef enum {
	LSM9DS1_ACC_GYR_ODR_P_DOWN  = 0x00,
	LSM9DS1_ACC_GYR_ODR_14_9Hz  = 0x20,
	LSM9DS1_ACC_GYR_ODR_59_5Hz  = 0x40,
	LSM9DS1_ACC_GYR_ODR_119Hz   = 0x60,
	LSM9DS1_ACC_GYR_ODR_238Hz   = 0x80,
	LSM9DS1_ACC_GYR_ODR_476HZ   = 0xA0,
	LSM9DS1_ACC_GYR_ODR_952HZ   = 0xC0	
} lsm9ds1_acc_gyr_odr_t;

typedef enum {
	LSM9DS1_ACC_FS_2G   = 0x00,
	LSM9DS1_ACC_FS_4G   = 0x10,
	LSM9DS1_ACC_FS_8G   = 0x18,
	LSM9DS1_ACC_FS_16G  = 0x08
} lsm9ds1_acc_fs_t;

typedef enum {
	LSM9DS1_INT1_GYRO       = 0x80,
	LSM9DS1_INT1_ACC        = 0x40,
	LSM9DS1_INT1_FSS5       = 0x20,
	LSM9DS1_INT1_OVR        = 0x10,
	LSM9DS1_INT1_FTH        = 0x08,
	LSM9DS1_INT1_BOOT       = 0x04,
	LSM9DS1_INT1_DRDY_G     = 0x02,
	LSM9DS1_INT1_DRDY_XL    = 0x01,
	LSM9DS1_INT1_NONE       = 0x00
} lsm9ds1_acc_int1_src_t;

typedef enum {
	LSM9DS1_INT2_INACT      = 0x80,
	LSM9DS1_INT2_FSS5       = 0x20,
	LSM9DS1_INT2_OVR        = 0x10,
	LSM9DS1_INT2_FTH        = 0x08,
	LSM9DS1_INT2_DRDY_TEMP  = 0x04,
	LSM9DS1_INT2_DRDY_G     = 0x02,
	LSM9DS1_INT2_DRDY_XL    = 0x01,
	LSM9DS1_INT2_NONE       = 0x00
} lsm9ds1_acc_int2_src_t;

typedef enum {
	LSM9DS1_GYR_FS_245DPS   = 0x00,
	LSM9DS1_GYR_FS_500DPS   = 0x01,
	LSM9DS1_GYR_FS_2000DPS  = 0x03
} lsm9ds1_gyr_fs_t;

typedef enum {
	LSM9DS1_MAG_ODR_0_625HZ = 0x00,
	LSM9DS1_MAG_ODR_1_25HZ  = 0x04,
	LSM9DS1_MAG_ODR_2_5HZ   = 0x08,
	LSM9DS1_MAG_ODR_5HZ     = 0x0C,
	LSM9DS1_MAG_ODR_10HZ    = 0x10,
	LSM9DS1_MAG_ODR_20HZ    = 0x14,
	LSM9DS1_MAG_ODR_40HZ    = 0x18,
	LSM9DS1_MAG_ODR_80HZ    = 0x1C
} lsm9ds1_mag_odr_t;

typedef enum {
	LSM9DS1_MAG_FS_4G   = 0x00,
	LSM9DS1_MAG_FS_8G   = 0x20,
	LSM9DS1_MAG_FS_12G  = 0x40,
	LSM9DS1_MAG_FS_16G  = 0x60
} lsm9ds1_mag_fs_t;

typedef enum {
	LSM9DS1_MAG_XY_OM_LP    = 0x00,
	LSM9DS1_MAG_XY_OM_MP    = 0x20,
	LSM9DS1_MAG_XY_OM_HP    = 0x40,
	LSM9DS1_MAG_XY_OM_UHP   = 0x60
} lsm9ds1_mag_xy_om_t;

typedef enum {
	LSM9DS1_MAG_Z_OM_LP     = 0x00,
	LSM9DS1_MAG_Z_OM_MP     = 0x04,
	LSM9DS1_MAG_Z_OM_HP     = 0x08,
	LSM9DS1_MAG_Z_OM_UHP    = 0x0C
} lsm9ds1_mag_z_om_t;

typedef enum {
	LSM9DS1_DISABLED        = 0x00,
	LSM9DS1_ACC_ENABLED     = 0x01,
	LSM9DS1_GYR_ENABLED     = 0x02,
	LSM9DS1_MAG_ENABLED     = 0x04
} lsm9ds1_status_t;

typedef enum {
	LSM9DS1_STATUS_REG_ACC_INT = 0x40,
	LSM9DS1_STATUS_REG_GYR_INT = 0x20
} lsm9ds1_status_reg_t;

typedef struct{
	lsm9ds1_acc_gyr_odr_t   odr;
	lsm9ds1_acc_fs_t        fs;
} lsm9ds1_acc_config_t;

typedef struct{
	lsm9ds1_acc_gyr_odr_t   odr;
	lsm9ds1_gyr_fs_t        fs;
} lsm9ds1_gyr_config_t;

typedef struct{
	lsm9ds1_mag_odr_t   odr;
	lsm9ds1_mag_fs_t    fs;
	lsm9ds1_mag_xy_om_t xy_operating_mode;
	lsm9ds1_mag_z_om_t  z_operating_mode;
} lsm9ds1_mag_config_t;

/**
 * @brief Device descriptor for LSM9DS1 sensor
 */
typedef struct {
	nrfx_twi_t * i2c_instance;     	/**<  Pointer to I2C instance device is connected to */
	uint8_t acc_gyr_i2c_address;  	/**<  Accelerometer & Gyro I2C address */
	uint8_t mag_i2c_address;       	/**<  Magnetometer I2C address */
	lsm9ds1_status_t sensor_status;
	bool int_1;
	bool int_2;
	bool int_m;
	bool mag_data_ready;
	lsm9ds1_acc_config_t acc_config;
	lsm9ds1_gyr_config_t gyr_config;
	lsm9ds1_mag_config_t mag_config;
} lsm9ds1_t;

#define LSM9DS1_DEF(_name, _p_i2c_instance, _acc_gyr_i2c_address, _mag_i2c_address) \
static lsm9ds1_t _name =                                                            \
{                                                                                   \
	.i2c_instance = _p_i2c_instance,                                                \
	.acc_gyr_i2c_address = _acc_gyr_i2c_address,                                    \
	.mag_i2c_address = _mag_i2c_address                                             \
}

bool lsm9ds1_init(lsm9ds1_t * p_lsm9ds1);

bool lsm9ds1_reset(lsm9ds1_t * p_lsm9ds1);

bool lsm9ds1_read_acc(lsm9ds1_t * p_lsm9ds1, lsm9ds1_acc_data_t * p_lsm9ds1_acc_data);

bool lsm9ds1_read_gyr(lsm9ds1_t * p_lsm9ds1, lsm9ds1_gyr_data_t * p_lsm9ds1_gyr_data);

bool lsm9ds1_read_mag(lsm9ds1_t * p_lsm9ds1, lsm9ds1_mag_data_t * p_lsm9ds1_mag_data);

bool lsm9ds1_read_acc_gyr_status(lsm9ds1_t * p_lsm9ds1, uint8_t * status);

bool lsm9ds1_read_acc_int_src(lsm9ds1_t * p_lsm9ds1, uint8_t * int_src);

bool lsm9ds1_read_gyro_int_src(lsm9ds1_t * p_lsm9ds1, uint8_t * int_src);

bool lsm9ds1_enable_acc(lsm9ds1_t * p_lsm9ds1, lsm9ds1_acc_config_t * p_lsm9ds1_acc_config);

bool lsm9ds1_acc_out_enable(lsm9ds1_t * p_lsm9ds1, bool enable);

bool lsm9ds1_acc_power_down(lsm9ds1_t * p_lsm9ds1);

bool lsm9ds1_enable_gyr(lsm9ds1_t * p_lsm9ds1, lsm9ds1_gyr_config_t * p_lsm9ds1_gyr_config);

bool lsm9ds1_gyr_out_enable(lsm9ds1_t * p_lsm9ds1, bool enable);

bool lsm9ds1_gyr_power_down(lsm9ds1_t * p_lsm9ds1);

bool lsm9ds1_enable_mag(lsm9ds1_t * p_lsm9ds1, lsm9ds1_mag_config_t * p_lsm9ds1_mag_config);

bool lsm9ds1_enable_mag_alert_int(lsm9ds1_t * p_lsm9ds1, bool enable);

bool lsm9ds1_set_mag_alert_threshold(lsm9ds1_t * p_lsm9ds1, uint16_t threshold);

bool lsm9ds1_read_mag_int_src(lsm9ds1_t * p_lsm9ds1, uint8_t * int_src);

bool lsm9ds1_mag_power_down(lsm9ds1_t * p_lsm9ds1);

bool lsm9ds1_set_acc_alert_threshold(lsm9ds1_t * p_lsm9ds1, uint8_t threshold);

bool lsm9ds1_enable_acc_alert_int(lsm9ds1_t * p_lsm9ds1, bool enable);

bool lsm9ds1_set_gyro_alert_threshold(lsm9ds1_t * p_lsm9ds1, int16_t threshold);

bool lsm9ds1_enable_gyro_alert_int(lsm9ds1_t * p_lsm9ds1, bool enable);

bool lsm9ds1_set_int1_src(lsm9ds1_t * p_lsm9ds1, uint8_t int_source);

bool lsm9ds1_set_int2_src(lsm9ds1_t * p_lsm9ds1, lsm9ds1_acc_int2_src_t int_source);

#ifdef __cplusplus
}
#endif

#endif // LSM9DS1_H

/**
 * @file        lsm9ds1.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       LSM9DS1 driver.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#include "lsm9ds1.h"

#include "lsm9ds1-internal.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

static bool twi_read(nrfx_twi_t *i2c_instance, uint8_t i2c_address, uint8_t reg, uint8_t *data, uint8_t data_length)
{
    if (data_length > 1)
    {
        reg |= 0x80; // Set the MSB of SUB to enable auto address increment
    }

    if (nrfx_twi_tx(i2c_instance, i2c_address, &reg, sizeof(reg), true) != NRF_SUCCESS)
    {
        NRF_LOG_WARNING("Failed to write twi");
        return false;
    } else
    {
        if (nrfx_twi_rx(i2c_instance, i2c_address, data, data_length) != NRF_SUCCESS)
        {
            NRF_LOG_WARNING("Failed to read twi");
            return false;
        }
    }
    return true;
}

static bool twi_write(nrfx_twi_t *i2c_instance, uint8_t i2c_address, uint8_t reg, uint8_t *data, uint8_t data_length)
{
    uint8_t tx_data[6];

    if (data_length > 1)
    {
        reg |= 0x80; // Set the MSB of SUB to enable auto address increment
    }

    tx_data[0] = reg;

    memcpy(&tx_data[1], data, data_length);

    data_length++;

    if (nrfx_twi_tx(i2c_instance, i2c_address, tx_data, data_length, false) != NRF_SUCCESS)
    {
        NRF_LOG_WARNING("Failed to write twi");
        return false;
    }
    return true;
}

static bool write_acc_gyr_reg(lsm9ds1_t *p_lsm9ds1, uint8_t reg, uint8_t data)
{
    if (!twi_write(p_lsm9ds1->i2c_instance, p_lsm9ds1->acc_gyr_i2c_address, reg, &data, sizeof(data)))
    {
        NRF_LOG_WARNING("Could not configure LSM9DS1 at i2c address: 0x%x", p_lsm9ds1->acc_gyr_i2c_address);
        return false;
    }
    return true;
}

static bool write_mag_reg(lsm9ds1_t *p_lsm9ds1, uint8_t reg, uint8_t data)
{
    if (!twi_write(p_lsm9ds1->i2c_instance, p_lsm9ds1->mag_i2c_address, reg, &data, sizeof(data)))
    {
        NRF_LOG_WARNING("Could not configure LSM9DS1 at i2c address: 0x%x", p_lsm9ds1->mag_i2c_address);
        return false;
    }
    return true;
}

bool lsm9ds1_init(lsm9ds1_t *p_lsm9ds1)
{
    uint8_t data;

    if (twi_read(p_lsm9ds1->i2c_instance,
                 p_lsm9ds1->acc_gyr_i2c_address,
                 LSM9DS1_ACC_GYR_WHO_AM_I_REG,
                 &data,
                 sizeof(data)))
    {
        if (data == LSM9DS1_ACC_GYR_WHO_AM_I)
        {
            NRF_LOG_INFO("Success identifying LSM9DS1 accelerometer & gyro at i2c address: 0x%x",
                         p_lsm9ds1->acc_gyr_i2c_address);
        } else
        {
            return false;
        }
    } else
    {
        NRF_LOG_WARNING("Could not identify LSM9DS1 accelerometer & gyro at i2c address: 0x%x",
                        p_lsm9ds1->acc_gyr_i2c_address);
        return false;
    }

    if (twi_read(p_lsm9ds1->i2c_instance, p_lsm9ds1->mag_i2c_address, LSM9DS1_MAG_WHO_AM_I_REG, &data, sizeof(data)))
    {
        if (data == LSM9DS1_MAG_WHO_AM_I)
        {
            NRF_LOG_INFO("Success identifying LSM9DS1 magnetometer at i2c address: 0x%x", p_lsm9ds1->mag_i2c_address);
        } else
        {
            return false;
        }
    } else
    {
        NRF_LOG_WARNING("Could not identify LSM9DS1 magnetometer at i2c address: 0x%x", p_lsm9ds1->mag_i2c_address);
        return false;
    }

    //-----------------------Reset------------------------------------------

    if (!lsm9ds1_reset(p_lsm9ds1))
    {
        return false;
    }

    //-----------------------Acc---Gyro---Config---------------------------

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG3_G, LSM9DS1_MASK_REG3_G_LP_MODE))
    { // Enable gyro low power mode
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG8, LSM9DS1_MASK_REG8_BDU_LACTIVE_OD_ADD_INC))
    { // Enable BDU, configure interrupts to be active low open-drain, enable register address auto increment
        return false;
    }

    lsm9ds1_acc_out_enable(p_lsm9ds1, false);
    lsm9ds1_gyr_out_enable(p_lsm9ds1, false);

    //--------------------MAG---config---------------------------------------
    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_CTRL_REG5_M, LSM9DS1_MASK_REG5_M_BDU))
    { // Enable BDU
        return false;
    }

    p_lsm9ds1->sensor_status  = LSM9DS1_DISABLED; // Sensor disabled
    p_lsm9ds1->int_1          = false;
    p_lsm9ds1->int_2          = false;
    p_lsm9ds1->int_m          = false;
    p_lsm9ds1->mag_data_ready = false;

    return true;
}

bool lsm9ds1_reset(lsm9ds1_t *p_lsm9ds1)
{
    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG8, LSM9DS1_MASK_REG8_SW_RESET))
    { // Soft reset accelerometer
        return false;
    }

    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_CTRL_REG2_M, LSM9DS1_MASK_REG2_M_SOFT_RST))
    { // Soft reset magnetometer
        return false;
    }

    return true;
}

bool lsm9ds1_read_acc(lsm9ds1_t *p_lsm9ds1, lsm9ds1_acc_data_t *p_lsm9ds1_acc_data)
{
    return twi_read(p_lsm9ds1->i2c_instance,
                    p_lsm9ds1->acc_gyr_i2c_address,
                    LSM9DS1_OUT_X_L_XL,
                    (uint8_t *)p_lsm9ds1_acc_data,
                    sizeof(lsm9ds1_acc_data_t));
}

bool lsm9ds1_read_gyr(lsm9ds1_t *p_lsm9ds1, lsm9ds1_gyr_data_t *p_lsm9ds1_gyr_data)
{
    return twi_read(p_lsm9ds1->i2c_instance,
                    p_lsm9ds1->acc_gyr_i2c_address,
                    LSM9DS1_OUT_X_L_G,
                    (uint8_t *)p_lsm9ds1_gyr_data,
                    sizeof(lsm9ds1_gyr_data_t));
}

bool lsm9ds1_read_mag(lsm9ds1_t *p_lsm9ds1, lsm9ds1_mag_data_t *p_lsm9ds1_mag_data)
{
    return twi_read(p_lsm9ds1->i2c_instance,
                    p_lsm9ds1->mag_i2c_address,
                    LSM9DS1_OUT_X_L_M,
                    (uint8_t *)p_lsm9ds1_mag_data,
                    sizeof(lsm9ds1_mag_data_t));
}

bool lsm9ds1_read_acc_gyr_status(lsm9ds1_t *p_lsm9ds1, uint8_t *status)
{
    return twi_read(p_lsm9ds1->i2c_instance,
                    p_lsm9ds1->acc_gyr_i2c_address,
                    LSM9DS1_STATUS_REG,
                    status,
                    sizeof(uint8_t));
}

bool lsm9ds1_read_acc_int_src(lsm9ds1_t *p_lsm9ds1, uint8_t *int_src)
{
    return twi_read(p_lsm9ds1->i2c_instance,
                    p_lsm9ds1->acc_gyr_i2c_address,
                    LSM9DS1_INT_GEN_SRC_XL,
                    int_src,
                    sizeof(uint8_t));
}

bool lsm9ds1_read_gyro_int_src(lsm9ds1_t *p_lsm9ds1, uint8_t *int_src)
{
    return twi_read(p_lsm9ds1->i2c_instance,
                    p_lsm9ds1->acc_gyr_i2c_address,
                    LSM9DS1_INT_GEN_SRC_G,
                    int_src,
                    sizeof(uint8_t));
}

bool lsm9ds1_enable_acc(lsm9ds1_t *p_lsm9ds1, lsm9ds1_acc_config_t *p_lsm9ds1_acc_config)
{
    if (!lsm9ds1_acc_out_enable(p_lsm9ds1, true))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG6_XL, p_lsm9ds1_acc_config->fs))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG1_G, p_lsm9ds1_acc_config->odr))
    {
        return false;
    }

    p_lsm9ds1->gyr_config.odr = p_lsm9ds1_acc_config->odr; // Accelerometer and gyro ODR is always the same
    p_lsm9ds1->acc_config     = *p_lsm9ds1_acc_config;
    p_lsm9ds1->sensor_status |= LSM9DS1_ACC_ENABLED;
    return true;
}

bool lsm9ds1_acc_out_enable(lsm9ds1_t *p_lsm9ds1, bool enable)
{
    if (enable)
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG5_XL, LSM9DS1_MASK_REG5_XL_OUT_ENABLE))
        {
            return false;
        }
    } else
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG5_XL, LSM9DS1_MASK_REG5_XL_OUT_DISABLE))
        {
            return false;
        }
    }
    return true;
}

bool lsm9ds1_acc_power_down(lsm9ds1_t *p_lsm9ds1)
{
    if (!(p_lsm9ds1->sensor_status & LSM9DS1_GYR_ENABLED))
    { // If gyro not enabled
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG1_G, LSM9DS1_MASK_REG1_G_POWER_DOWN))
        {
            return false;
        }
    } else
    {
        if (!lsm9ds1_acc_out_enable(p_lsm9ds1, false))
        { // Disable accelerometer data output
            return false;
        }
    }

    p_lsm9ds1->sensor_status &= ~LSM9DS1_ACC_ENABLED;
    return true;
}

bool lsm9ds1_enable_gyr(lsm9ds1_t *p_lsm9ds1, lsm9ds1_gyr_config_t *p_lsm9ds1_gyr_config)
{
    if (!lsm9ds1_gyr_out_enable(p_lsm9ds1, true))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG1_G, p_lsm9ds1_gyr_config->odr | p_lsm9ds1_gyr_config->fs))
    {
        return false;
    }

    p_lsm9ds1->acc_config.odr = p_lsm9ds1_gyr_config->odr; // Accelerometer and gyro ODR is always the same
    p_lsm9ds1->gyr_config     = *p_lsm9ds1_gyr_config;     // Accelerometer and gyro ODR is always the same
    p_lsm9ds1->sensor_status |= LSM9DS1_GYR_ENABLED;

    return true;
}

bool lsm9ds1_gyr_out_enable(lsm9ds1_t *p_lsm9ds1, bool enable)
{
    if (enable)
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG4, LSM9DS1_MASK_REG4_G_OUT_ENABLE))
        {
            return false;
        }
    } else
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG4, LSM9DS1_MASK_REG4_G_OUT_DISABLE))
        {
            return false;
        }
    }
    return true;
}

bool lsm9ds1_gyr_power_down(lsm9ds1_t *p_lsm9ds1)
{ // This function disables both the gyro and accelerometer
    if (!(p_lsm9ds1->sensor_status & LSM9DS1_ACC_ENABLED))
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_CTRL_REG1_G, LSM9DS1_MASK_REG1_G_POWER_DOWN))
        {
            return false;
        }
    } else
    {
        if (!lsm9ds1_gyr_out_enable(p_lsm9ds1, false))
        {
            return false;
        }
    }

    p_lsm9ds1->sensor_status &= ~LSM9DS1_GYR_ENABLED;
    return true;
}

bool lsm9ds1_enable_mag(lsm9ds1_t *p_lsm9ds1, lsm9ds1_mag_config_t *p_lsm9ds1_mag_config)
{
    uint8_t data;

    data = p_lsm9ds1_mag_config->odr | p_lsm9ds1_mag_config->xy_operating_mode | LSM9DS1_MASK_REG1_M_TEMP_COMP;

    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_CTRL_REG1_M, data))
    {
        return false;
    }

    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_CTRL_REG2_M, p_lsm9ds1_mag_config->fs))
    {
        return false;
    }

    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_CTRL_REG4_M, p_lsm9ds1_mag_config->z_operating_mode))
    {
        return false;
    }

    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_CTRL_REG3_M, LSM9DS1_MASK_REG3_M_CONT_CONVERSION))
    {
        return false;
    }

    p_lsm9ds1->mag_config.odr               = p_lsm9ds1_mag_config->odr;
    p_lsm9ds1->mag_config.fs                = p_lsm9ds1_mag_config->fs;
    p_lsm9ds1->mag_config.xy_operating_mode = p_lsm9ds1_mag_config->xy_operating_mode;
    p_lsm9ds1->mag_config.z_operating_mode  = p_lsm9ds1_mag_config->z_operating_mode;
    p_lsm9ds1->sensor_status |= LSM9DS1_MAG_ENABLED;
    return true;
}

bool lsm9ds1_enable_mag_alert_int(lsm9ds1_t *p_lsm9ds1, bool enable)
{
    if (enable)
    {
        if (!write_mag_reg(p_lsm9ds1, LSM9DS1_INT_CFG_M, 0xE1))
        {
            return false;
        }
    } else
    {
        if (!write_mag_reg(p_lsm9ds1, LSM9DS1_INT_CFG_M, 0x00))
        {
            return false;
        }
    }
    return true;
}

bool lsm9ds1_set_mag_alert_threshold(lsm9ds1_t *p_lsm9ds1, uint16_t threshold)
{
    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_INT_THS_L, (threshold >> 8)))
    {
        return false;
    }
    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_INT_THS_H, (threshold & 0x7F)))
    { // Set MSB to 0 as specified in the datasheet
        return false;
    }
    return true;
}

bool lsm9ds1_read_mag_int_src(lsm9ds1_t *p_lsm9ds1, uint8_t *int_src)
{
    return twi_read(p_lsm9ds1->i2c_instance, p_lsm9ds1->mag_i2c_address, LSM9DS1_INT_SRC_M, int_src, sizeof(uint8_t));
}

bool lsm9ds1_mag_power_down(lsm9ds1_t *p_lsm9ds1)
{
    if (!write_mag_reg(p_lsm9ds1, LSM9DS1_CTRL_REG3_M, LSM9DS1_MASK_REG3_M_POWER_DOWN))
    {
        return false;
    }
    p_lsm9ds1->sensor_status &= ~LSM9DS1_MAG_ENABLED;
    return true;
}

bool lsm9ds1_set_acc_alert_threshold(lsm9ds1_t *p_lsm9ds1, uint8_t threshold)
{
    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_X_XL, threshold))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_Y_XL, threshold))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_Z_XL, threshold))
    {
        return false;
    }
    return true;
}

bool lsm9ds1_enable_acc_alert_int(lsm9ds1_t *p_lsm9ds1, bool enable)
{
    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_DUR_XL, 0x00 /*LSM9DS1_MASK_INT_GEN_DUR_WAIT_XL | 4*/))
    { // Enable accelerometer wait function (1 samples before exiting the interrupt)
        return false;
    }

    if (enable)
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_CFG_XL, LSM9DS1_MASK_INT_GEN_CFG_XL_HIGH_INT))
        {
            return false;
        }
    } else
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_CFG_XL, LSM9DS1_MASK_INT_GEN_CFG_XL_DISABLED))
        {
            return false;
        }
    }
    return true;
}

bool lsm9ds1_set_gyro_alert_threshold(lsm9ds1_t *p_lsm9ds1, int16_t threshold)
{
    NRF_LOG_INFO("Gyro threshold: %i, max: 0x%0x, min: 0x%x", threshold, INT_15_BIT_MAX, INT_15_BIT_MIN);
    if (threshold > INT_15_BIT_MAX)
        threshold = INT_15_BIT_MAX;
    else if (threshold < INT_15_BIT_MIN)
        threshold = INT_15_BIT_MIN;

    NRF_LOG_INFO("Gyro threshold after: %i", threshold);
    // threshold |= LSM9DS1_MASK_INT_GEN_THS_XH_G_DCRM; // Enable decrement counter
    threshold &= 0x7FFF;

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_XH_G, (threshold >> 8)))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_XL_G, (threshold & 0xFF)))
    {
        return false;
    }

    // threshold &= ~LSM9DS1_MASK_INT_GEN_THS_XH_G_DCRM; // Set DCRM bit to zero as the following registers don't
    // contain it

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_YH_G, (threshold >> 8)))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_YL_G, (threshold & 0xFF)))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_ZH_G, (threshold >> 8)))
    {
        return false;
    }

    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_THS_ZL_G, (threshold & 0xFF)))
    {
        return false;
    }

    return true;
}

bool lsm9ds1_enable_gyro_alert_int(lsm9ds1_t *p_lsm9ds1, bool enable)
{
    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_DUR_G, 0x00))
    { // Gyro no wait
        return false;
    }

    if (enable)
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_CFG_G, LSM9DS1_MASK_INT_GEN_CFG_G_HIGH_INT))
        {
            return false;
        }
    } else
    {
        if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT_GEN_CFG_G, LSM9DS1_MASK_INT_GEN_CFG_G_DISABLED))
        {
            return false;
        }
    }
    return true;
}

bool lsm9ds1_set_int1_src(lsm9ds1_t *p_lsm9ds1, uint8_t int_source)
{
    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT1_CTRL, int_source))
    {
        return false;
    }
    return true;
}

bool lsm9ds1_set_int2_src(lsm9ds1_t *p_lsm9ds1, lsm9ds1_acc_int2_src_t int_source)
{
    if (!write_acc_gyr_reg(p_lsm9ds1, LSM9DS1_INT2_CTRL, int_source))
    {
        return false;
    }
    return true;
}

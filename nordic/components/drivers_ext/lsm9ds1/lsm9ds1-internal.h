/**
 * @file        lsm9ds1-internal.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       LSM9DS1 driver internal defines.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef LSM9DS1_INTERNAL_H
#define LSM9DS1_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define LSM9DS1_ACC_GYR_WHO_AM_I             0x68
#define LSM9DS1_ACC_GYR_WHO_AM_I_REG         0x0F

#define LSM9DS1_INT_GEN_CFG_XL               0x06

#define LSM9DS1_MASK_INT_GEN_CFG_XL_HIGH_INT 0x2A
#define LSM9DS1_MASK_INT_GEN_CFG_XL_DISABLED 0x00

#define LSM9DS1_INT_GEN_THS_X_XL             0x07
#define LSM9DS1_INT_GEN_THS_Y_XL             0x08
#define LSM9DS1_INT_GEN_THS_Z_XL             0x09

#define LSM9DS1_INT_GEN_DUR_XL               0x0A

#define LSM9DS1_MASK_INT_GEN_DUR_WAIT_XL     0x80

#define LSM9DS1_INT1_CTRL                    0x0C
#define LSM9DS1_INT2_CTRL                    0x0D

#define LSM9DS1_CTRL_REG1_G                  0x10

#define LSM9DS1_MASK_REG1_G_POWER_DOWN       0x00

#define LSM9DS1_CTRL_REG3_G                  0x12

#define LSM9DS1_MASK_REG3_G_LP_MODE          0x80

#define LSM9DS1_INT_GEN_SRC_G                0x14

#define LSM9DS1_OUT_X_L_G                    0x18

#define LSM9DS1_CTRL_REG4                    0x1E

#define LSM9DS1_MASK_REG4_G_OUT_ENABLE       0x38
#define LSM9DS1_MASK_REG4_G_OUT_DISABLE      0x00

#define LSM9DS1_CTRL_REG5_XL                 0x1F
#define LSM9DS1_CTRL_REG6_XL                 0x20
#define LSM9DS1_CTRL_REG8                    0x22
#define LSM9DS1_CTRL_REG9                    0x23

#define LSM9DS1_MASK_REG5_XL_OUT_ENABLE      0x38
#define LSM9DS1_MASK_REG5_XL_OUT_DISABLE     0x00
#define LSM9DS1_MASK_REG6_XL_POWER_DOWN      0x00

#define LSM9DS1_MASK_REG8_SW_RESET           0x01
#define LSM9DS1_MASK_REG8_BDU                0x40
#define LSM9DS1_MASK_REG8_H_LACTIVE          0x20
#define LSM9DS1_MASK_REG8_PP_OD              0x10
#define LSM9DS1_MASK_REG8_IF_ADD_INC         0x04
#define LSM9DS1_MASK_REG8_BDU_LACTIVE_OD_ADD_INC                                                                       \
    LSM9DS1_MASK_REG8_BDU | LSM9DS1_MASK_REG8_H_LACTIVE | LSM9DS1_MASK_REG8_PP_OD | LSM9DS1_MASK_REG8_IF_ADD_INC

#define LSM9DS1_INT_GEN_SRC_XL              0x26
#define LSM9DS1_STATUS_REG                  0x27

#define LSM9DS1_OUT_X_L_XL                  0x28
#define LSM9DS1_OUT_X_H_XL                  0x29
#define LSM9DS1_OUT_Y_L_XL                  0x2A
#define LSM9DS1_OUT_Y_H_XL                  0x2B
#define LSM9DS1_OUT_Z_L_XL                  0x2C
#define LSM9DS1_OUT_Z_H_XL                  0x2D

#define LSM9DS1_INT_GEN_CFG_G               0x30

#define LSM9DS1_MASK_INT_GEN_CFG_G_HIGH_INT 0x2A
#define LSM9DS1_MASK_INT_GEN_CFG_G_DISABLED 0x00

#define LSM9DS1_INT_GEN_THS_XH_G            0x31
#define LSM9DS1_INT_GEN_THS_XL_G            0x32
#define LSM9DS1_INT_GEN_THS_YH_G            0x33
#define LSM9DS1_INT_GEN_THS_YL_G            0x34
#define LSM9DS1_INT_GEN_THS_ZH_G            0x35
#define LSM9DS1_INT_GEN_THS_ZL_G            0x36

#define LSM9DS1_MASK_INT_GEN_THS_XH_G_DCRM  0x80

#define LSM9DS1_INT_GEN_DUR_G               0x37
#define LSM9DS1_MASK_INT_GEN_DUR_WAIT_G     0x80

// Magnetometer

#define LSM9DS1_MAG_WHO_AM_I                0x3D
#define LSM9DS1_MAG_WHO_AM_I_REG            0x0F

#define LSM9DS1_CTRL_REG1_M                 0x20
#define LSM9DS1_CTRL_REG2_M                 0x21
#define LSM9DS1_CTRL_REG3_M                 0x22
#define LSM9DS1_CTRL_REG4_M                 0x23
#define LSM9DS1_CTRL_REG5_M                 0x24

#define LSM9DS1_MASK_REG1_M_TEMP_COMP       0x80

#define LSM9DS1_MASK_REG2_M_SOFT_RST        0x04

#define LSM9DS1_MASK_REG3_M_CONT_CONVERSION 0x00
#define LSM9DS1_MASK_REG3_M_POWER_DOWN      0x03

#define LSM9DS1_MASK_REG5_M_BDU             0x40

#define LSM9DS1_OUT_X_L_M                   0x28

#define LSM9DS1_INT_CFG_M                   0x30
#define LSM9DS1_INT_SRC_M                   0x31
#define LSM9DS1_INT_THS_L                   0x32
#define LSM9DS1_INT_THS_H                   0x33

#define INT_15_BIT_MAX                      16383
#define INT_15_BIT_MIN                      -16384

#ifdef __cplusplus
}
#endif

#endif // LSM9DS1_INTERNAL_H

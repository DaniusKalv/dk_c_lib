/**
 * @file        is31fl3206-internal.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       IS31FL3206 LED driver driver internal macros.
 * @version     0.1
 * @date        2019-11-18
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef IS31FL3206_INTERNAL_H
#define IS31FL3206_INTERNAL_H

#define IS31FL3206_SHUTDOWN         0x00

#define IS31FL3206_PWM0             0x04

#define IS31FL3206_UPDATE           0x13
#define IS31FL3206_UPDATE_VAL       0x00

#define IS31FL3206_LED_CTRL0        0x17

#define IS31FL3206_GLOBAL_CTRL      0x26

#define IS31FL3206_OUT_FREQUENCY    0x27

#define IS31FL3206_RESET            0x2F
#define IS31FL3206_RESET_VAL        0x00

#endif //IS31FL3206_INTERNAL_H
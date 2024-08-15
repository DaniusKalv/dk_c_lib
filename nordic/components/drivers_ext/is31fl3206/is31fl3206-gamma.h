/**
 * @file        is31fl3206-gamma.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       IS31FL3206 LED driver 32/64 step gamma correction LUT.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef IS31FL3206_GAMMA_H
#define IS31FL3206_GAMMA_H

#define IS31FL3206_GAMMA32                                                                                             \
    {                                                                                                                  \
        0, 1, 2, 4, 6, 10, 13, 18, 22, 28, 33, 39, 46, 53, 61, 69, 78, 86, 96, 106, 116, 126, 138, 149, 161, 173, 186, \
          199, 212, 226, 240, 255                                                                                      \
    }

#define IS31FL3206_GAMMA64                                                                                             \
    {                                                                                                                  \
        0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 29, 32, 35, 38, 41, 44, 47, 50, 53, 57, 61, 65, \
          69, 73, 77, 81, 85, 89, 94, 99, 104, 109, 114, 119, 124, 129, 134, 140, 146, 152, 158, 164, 170, 176, 182,   \
          188, 195, 202, 209, 216, 223, 230, 237, 244, 251, 255                                                        \
    }

#endif // IS31FL3206_GAMMA_H

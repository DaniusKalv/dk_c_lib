/**
 * @file        sh1106-internal.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Monochrome OLED display driver internal definitions
 * @version     0.1
 * @date        2020-04-02
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef SH1106_INTERNAL_H
#define SH1106_INTERNAL_H

#define SH1106_CMD_COLUMN_ADDRESS_LOW       0x00    /**< Set column address 4 LSB. */
#define SH1106_CMD_COLUMN_ADDRESS_HIGH      0x10    /**< Set column address 4 MSB. */
#define SH1106_COLUMN_ADDRESS_MASK          0x0F    /**< Column address setting bit mask. */
#define SH1106_COLUMN_ADDRESS_BITS          4       /**< Amount of bits that can be set with column address command. */
#define SH1106_COLUMN_ID_MAX                131     /**< Highest column address supported by the IC. */

#define SH1106_CMD_PUMP_VOLTAGE             0x30    /**< Pump voltage command. */

#define SH1106_CMD_START_LINE               0x40    /**< Display start line command. */
#define SH1106_LINE_ID_MAX                  63      /**< Highest line address supported by the IC. */

#define SH1106_CMD_CONTRAST                 0x81    /**< Contrast control register address. */

#define SH1106_CMD_SEGMENT_REMAP            0xA0    /**< Relationship between RAM and column address order. */

#define SH1106_CMD_DISPLAY_FORCE_OFF_ON     0xA4    /**< Force entir display off/on command. RAM data is ignored. */

#define SH1106_CMD_INVERT_DISPLAY           0xA6    /**< Invert display pixels command. */

#define SH1106_CMD_MULTIPLEX_RATIO          0xA8    /**< Multiplex ratio command. */
#define SH1106_MULTIPLEX_RATIO_MAX          0x3F    /**< Maximum allowed multiplex ratio value. */

#define SH1106_CMD_DC_DC_OFF_ON_CTRL        0xAD    /**< DC-DC converter off/on control mode command. */
#define SH1106_CMD_DC_DC_OFF_ON             0x8A    /**< DC-DC converter off/on command. */

#define SH1106_CMD_DISPLAY_OFF_ON           0xAE    /**< Display on/off command. */

#define SH1106_CMD_PAGE_ADDRESS             0xB0    /**< Set page address. */
#define SH1106_PAGE_AMOUNT                  8       /**< Amount of pages. */
#define SH1106_PAGE_ID_MAX                  7       /**< Highest page address supported by the IC. */

#define SH1106_CMD_OUT_SCAN_DIR             0xC0    /**< Common output scan direction command. */
#define SH1106_OUT_SCAN_DIR_BIT_ID          3       /**< Common output scan direction control bit index. */

#define SH1106_CMD_DISPLAY_OFFSET           0xD3    /**< Display offset command. */

#define SH1106_CMD_CLK_FREQ                 0xD5    /**< Display clock frequency control command. */

#define SH1106_CMD_DIS_PRE_CHRG             0xD9    /**< Discharge/precharge period command. */

#define SH1106_CMD_COM_PADS_HW_CONF_CTRL    0xDA    /**< Common pads hardware configuration control command. */
#define SH1106_COM_PADS_HW_CONF             0x02    /**< Common pads hardware configuration setting. */
#define SH1106_COM_PADS_HW_CONF_BIT_ID      4       /**< Common pads hardware configuration bit index. */

#define SH1106_CMD_VCOM_DES_LVL             0xDB    /**< VCOM deselect level command. */

#define SH1106_CMD_READ_MODIFY_WRITE        0xE0    /**< Read modify write command. */
#define SH1106_CMD_READ_MODIFY_WRITE_END    0xEE    /**< Exit read modify write mode. */

#endif // SH1106_INTERNAL_H



/*
set lower column address 0x00 - 0x0F
set higher column address 0x41 - 0x1F

*/

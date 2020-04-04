/**
 * @file        sh1106.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Monochrome OLED display driver
 * @version     0.1
 * @date        2020-04-02
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef SH1106_H
#define SH1106_H

#include "nrf_gpio.h"
#include "nrfx_spi.h"
#include "sdk_errors.h"

typedef enum
{
	SH1106_PUMP_VOLTAGE_6_4     = 0x00, /**< 6.4V */
	SH1106_PUMP_VOLTAGE_7_4     = 0x01, /**< 7.4V */
	SH1106_PUMP_VOLTAGE_8_0     = 0x02, /**< 8V */
	SH1106_PUMP_VOLTAGE_9_0     = 0x03  /**< 9V */
} sh1106_pump_voltage_t;

typedef enum
{
	SH1106_OSC_FREQ_N25     = 0x00, /**< -25% */
	SH1106_OSC_FREQ_N20     = 0x01, /**< -20% */
	SH1106_OSC_FREQ_N15     = 0x02, /**< -15% */
	SH1106_OSC_FREQ_N10     = 0x03, /**< -10% */
	SH1106_OSC_FREQ_N5      = 0x04, /**< -5% */
	SH1106_OSC_FREQ_DEF     = 0x05, /**< Default frequency. */
	SH1106_OSC_FREQ_5       = 0x06, /**< 5% */
	SH1106_OSC_FREQ_10      = 0x07, /**< 10% */
	SH1106_OSC_FREQ_15      = 0x08, /**< 15% */
	SH1106_OSC_FREQ_20      = 0x09, /**< 20% */
	SH1106_OSC_FREQ_25      = 0x0A, /**< 25% */
	SH1106_OSC_FREQ_30      = 0x0B, /**< 30% */
	SH1106_OSC_FREQ_35      = 0x0C, /**< 35% */
	SH1106_OSC_FREQ_40      = 0x0D, /**< 40% */
	SH1106_OSC_FREQ_45      = 0x0E, /**< 45% */
	SH1106_OSC_FREQ_50      = 0x0F  /**< 50% */
} sh1106_osc_freq_t;

typedef struct
{
	uint8_t             clk_div:4;
	sh1106_osc_freq_t   osc_freq:4;
} sh1106_clk_freq_t;

typedef enum
{
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_1   = 0x01, /**< 1 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_2   = 0x02, /**< 2 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_3   = 0x03, /**< 3 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_4   = 0x04, /**< 4 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_5   = 0x05, /**< 5 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_6   = 0x06, /**< 6 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_7   = 0x07, /**< 7 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_8   = 0x08, /**< 8 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_9   = 0x09, /**< 9 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_10  = 0x0A, /**< 10 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_11  = 0x0B, /**< 11 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_12  = 0x0C, /**< 12 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_13  = 0x0D, /**< 13 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_14  = 0x0E, /**< 14 DCLK */
	SH1106_DIS_PRE_CHRG_PERIOD_DCLK_15  = 0x0F  /**< 15 DCLK */
} sh1106_dis_pre_chrg_period_t;

typedef struct
{
	sh1106_dis_pre_chrg_period_t    precharge:4; /**< Precharge period. */
	sh1106_dis_pre_chrg_period_t    discharge:4; /**< Discharge period. */
} sh1106_dis_pre_chrg_t;

typedef struct
{
	uint32_t        reset_pin;
	uint32_t        cs_pin;
	uint32_t        dc_pin;
	nrfx_spi_t    * p_spi_instance;
} sh1106_t;

#define SH1106_DEF(_name, _rst_pin, _cs_pin, _dc_pin, _p_spi_instance) \
static sh1106_t _name =                                                \
{                                                                      \
	.reset_pin      = _rst_pin,                                        \
	.cs_pin         = _cs_pin,                                         \
	.dc_pin         = _dc_pin,                                         \
	.p_spi_instance = _p_spi_instance                                  \
}

ret_code_t sh1106_init(sh1106_t * p_sh1106);

ret_code_t sh1106_set_column_address(sh1106_t * p_sh1106, uint8_t column_address);

ret_code_t sh1106_set_pump_voltage(sh1106_t * p_sh1106, sh1106_pump_voltage_t pump_voltage);

ret_code_t sh1106_set_start_line(sh1106_t * p_sh1106, uint8_t start_line);

ret_code_t sh1106_set_contrast(sh1106_t * p_sh1106, uint8_t contrast);

ret_code_t sh1106_set_segment_remap(sh1106_t * p_sh1106, bool reverse);

ret_code_t sh1106_force_display_on(sh1106_t * p_sh1106, bool on);

ret_code_t sh1106_invert_display(sh1106_t * p_sh1106, bool invert);

ret_code_t sh1106_set_multiplex_ratio(sh1106_t * p_sh1106, uint8_t ratio);

ret_code_t sh1106_set_dc_dc_on(sh1106_t * p_sh1106, bool on);

ret_code_t sh1106_set_display_on(sh1106_t * p_sh1106, bool on);

ret_code_t sh1106_set_page_address(sh1106_t * p_sh1106, uint8_t page);

ret_code_t sh1106_set_scan_direction(sh1106_t * p_sh1106, bool reverse);

ret_code_t sh1106_set_display_offset(sh1106_t * p_sh1106, uint8_t offset);

ret_code_t sh1106_set_clk_freq(sh1106_t * p_sh1106, sh1106_clk_freq_t * p_clk_freq);

ret_code_t sh1106_set_dis_pre_chrg_period(sh1106_t * p_sh1106, sh1106_dis_pre_chrg_t * p_period);

ret_code_t sh1106_set_com_pads_hw_config(sh1106_t * p_sh1106, bool alternative);

ret_code_t sh1106_set_vcom_des_lvl(sh1106_t * p_sh1106, uint8_t level);

ret_code_t sh1106_enter_read_modify_write_mode(sh1106_t * p_sh1106);

ret_code_t sh1106_exit_read_modify_write_mode(sh1106_t * p_sh1106);

ret_code_t sh1106_write_data(sh1106_t * p_sh1106, uint8_t * p_data, uint16_t size);

#endif // SH1106_H

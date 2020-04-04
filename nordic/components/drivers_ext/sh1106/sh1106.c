/**
 * @file        sh1106.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Monochrome OLED display driver
 * @version     0.1
 * @date        2020-04-02
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#include "sh1106.h"
#include "sh1106-internal.h"
#include "nrf_delay.h"
#include "sdk_macros.h"
#include "string.h"

static ret_code_t write(sh1106_t * p_sh1106, const uint8_t * p_data, uint8_t data_size, bool data)
{
	ret_code_t err_code;
	nrfx_spi_xfer_desc_t xfer = NRFX_SPI_XFER_TX(p_data, data_size);

	nrf_gpio_pin_write(p_sh1106->dc_pin, (uint32_t)data);
	nrf_gpio_pin_clear(p_sh1106->cs_pin);
	err_code = nrfx_spi_xfer(p_sh1106->p_spi_instance, &xfer, 0);
	nrf_gpio_pin_set(p_sh1106->cs_pin);

	return err_code;
}

ret_code_t sh1106_init(sh1106_t * p_sh1106)
{
	ret_code_t err_code;

	nrf_gpio_cfg_output(p_sh1106->reset_pin);
	nrf_gpio_cfg_output(p_sh1106->cs_pin);
	nrf_gpio_cfg_output(p_sh1106->dc_pin);

	nrf_gpio_pin_clear(p_sh1106->reset_pin);
	nrf_gpio_pin_set(p_sh1106->cs_pin);
	nrf_gpio_pin_set(p_sh1106->reset_pin);

	sh1106_set_display_on(p_sh1106, false);

	sh1106_clk_freq_t clk_freq =
	{
		.osc_freq   = SH1106_OSC_FREQ_15,
		.clk_div    = 0
	};
	sh1106_set_clk_freq(p_sh1106, &clk_freq);

	sh1106_set_segment_remap(p_sh1106, true);

	sh1106_set_scan_direction(p_sh1106, true);

	sh1106_set_contrast(p_sh1106, 128);

	sh1106_set_vcom_des_lvl(p_sh1106, 0x40);

	sh1106_set_dc_dc_on(p_sh1106, false);

	uint8_t zero = 0;
	for(uint8_t page = 0; page < 8; page++)
	{
		sh1106_set_page_address(p_sh1106, page);
		sh1106_set_column_address(p_sh1106, 0);

		for(uint8_t column = 0; column < 132; column++)
		{
			write(p_sh1106, &zero, sizeof(zero), true);
		}
	}

	sh1106_set_display_on(p_sh1106, true);

	return err_code;
}

ret_code_t sh1106_set_column_address(sh1106_t * p_sh1106, uint8_t column_address)
{
	column_address &= SH1106_COLUMN_ID_MAX;

	uint8_t column_address_cmd[] =
	{
		(SH1106_CMD_COLUMN_ADDRESS_LOW | (column_address & SH1106_COLUMN_ADDRESS_MASK)),
		(SH1106_CMD_COLUMN_ADDRESS_HIGH | (column_address >> SH1106_COLUMN_ADDRESS_BITS))
	};

	return write(p_sh1106, column_address_cmd, sizeof(column_address_cmd), false);
}

ret_code_t sh1106_set_pump_voltage(sh1106_t * p_sh1106, sh1106_pump_voltage_t pump_voltage)
{
	uint8_t pump_voltage_cmd = SH1106_CMD_PUMP_VOLTAGE | pump_voltage;

	return write(p_sh1106, &pump_voltage_cmd, sizeof(pump_voltage_cmd), false);
}

ret_code_t sh1106_set_start_line(sh1106_t * p_sh1106, uint8_t start_line)
{
	uint8_t start_line_cmd = SH1106_CMD_START_LINE | (start_line & SH1106_LINE_ID_MAX);

	return write(p_sh1106, &start_line_cmd, sizeof(start_line_cmd), false);
}

ret_code_t sh1106_set_contrast(sh1106_t * p_sh1106, uint8_t contrast)
{
	uint8_t contrast_cmd[] =
	{
		SH1106_CMD_CONTRAST,
		contrast
	};

	return write(p_sh1106, contrast_cmd, sizeof(contrast_cmd), false);
}

ret_code_t sh1106_set_segment_remap(sh1106_t * p_sh1106, bool reverse)
{
	uint8_t segment_remap_cmd = SH1106_CMD_SEGMENT_REMAP | reverse;

	return write(p_sh1106, &segment_remap_cmd, sizeof(segment_remap_cmd), false);
}

ret_code_t sh1106_force_display_on(sh1106_t * p_sh1106, bool on)
{
	uint8_t display_on_cmd = SH1106_CMD_DISPLAY_FORCE_OFF_ON | on;

	return write(p_sh1106, &display_on_cmd, sizeof(display_on_cmd), false);
}

ret_code_t sh1106_invert_display(sh1106_t * p_sh1106, bool invert)
{
	uint8_t display_invert_cmd = SH1106_CMD_INVERT_DISPLAY | invert;

	return write(p_sh1106, &display_invert_cmd, sizeof(display_invert_cmd), false);
}

ret_code_t sh1106_set_multiplex_ratio(sh1106_t * p_sh1106, uint8_t ratio)
{
	ratio &= SH1106_MULTIPLEX_RATIO_MAX;
	uint8_t multiplex_ratio_cmd [] =
	{
		SH1106_CMD_MULTIPLEX_RATIO,
		ratio
	};

	return write(p_sh1106, multiplex_ratio_cmd, sizeof(multiplex_ratio_cmd), false);
}

ret_code_t sh1106_set_dc_dc_on(sh1106_t * p_sh1106, bool on)
{
	uint8_t dc_dc_cmd[] =
	{
		SH1106_CMD_DC_DC_OFF_ON_CTRL,
		SH1106_CMD_DC_DC_OFF_ON | on
	};

	return write(p_sh1106, dc_dc_cmd, sizeof(dc_dc_cmd), false);
}

ret_code_t sh1106_set_display_on(sh1106_t * p_sh1106, bool on)
{
	uint8_t display_cmd = SH1106_CMD_DISPLAY_OFF_ON | on;

	return write(p_sh1106, &display_cmd, sizeof(display_cmd), false);
}

ret_code_t sh1106_set_page_address(sh1106_t * p_sh1106, uint8_t page)
{
	page &= SH1106_PAGE_ID_MAX;
	uint8_t page_address_cmd = SH1106_CMD_PAGE_ADDRESS | page;

	return write(p_sh1106, &page_address_cmd, sizeof(page_address_cmd), false);
}

ret_code_t sh1106_set_scan_direction(sh1106_t * p_sh1106, bool reverse)
{
	uint8_t scan_dir_cmd = SH1106_CMD_OUT_SCAN_DIR | (reverse << SH1106_OUT_SCAN_DIR_BIT_ID);

	return write(p_sh1106, &scan_dir_cmd, sizeof(scan_dir_cmd), false);
}

ret_code_t sh1106_set_display_offset(sh1106_t * p_sh1106, uint8_t offset)
{
	offset &= SH1106_LINE_ID_MAX;
	uint8_t display_offset_cmd[] =
	{
		SH1106_CMD_DISPLAY_OFFSET,
		offset
	};

	return write(p_sh1106, display_offset_cmd, sizeof(display_offset_cmd), false);
}

ret_code_t sh1106_set_clk_freq(sh1106_t * p_sh1106, sh1106_clk_freq_t * p_clk_freq)
{
	uint8_t clk_freq_cmd[sizeof(SH1106_CMD_CLK_FREQ) + sizeof(sh1106_clk_freq_t)] =
	{
		SH1106_CMD_CLK_FREQ
	};

	memcpy(&clk_freq_cmd[sizeof(SH1106_CMD_CLK_FREQ)], p_clk_freq, sizeof(sh1106_clk_freq_t));

	return write(p_sh1106, clk_freq_cmd, sizeof(clk_freq_cmd), false);
}

ret_code_t sh1106_set_dis_pre_chrg_period(sh1106_t * p_sh1106, sh1106_dis_pre_chrg_t * p_period)
{
	uint8_t period_cmd[sizeof(SH1106_CMD_DIS_PRE_CHRG) + sizeof(sh1106_dis_pre_chrg_t)] =
	{
		SH1106_CMD_DIS_PRE_CHRG
	};

	memcpy(&period_cmd[sizeof(SH1106_CMD_DIS_PRE_CHRG)], p_period, sizeof(sh1106_dis_pre_chrg_t));

	return write(p_sh1106, period_cmd, sizeof(period_cmd), false);
}

ret_code_t sh1106_set_com_pads_hw_config(sh1106_t * p_sh1106, bool alternative)
{
	uint8_t com_pads_hw_config_cmd[] =
	{
		SH1106_CMD_COM_PADS_HW_CONF_CTRL,
		SH1106_COM_PADS_HW_CONF | (alternative << SH1106_COM_PADS_HW_CONF_BIT_ID)
	};

	return write(p_sh1106, com_pads_hw_config_cmd, sizeof(com_pads_hw_config_cmd), false);
}

ret_code_t sh1106_set_vcom_des_lvl(sh1106_t * p_sh1106, uint8_t level)
{
	uint8_t vcom_des_lvl_cmd[] =
	{
		SH1106_CMD_VCOM_DES_LVL,
		level
	};

	return write(p_sh1106, vcom_des_lvl_cmd, sizeof(vcom_des_lvl_cmd), false);
}

ret_code_t sh1106_enter_read_modify_write_mode(sh1106_t * p_sh1106)
{
	uint8_t read_modify_write_cmd = SH1106_CMD_READ_MODIFY_WRITE;

	return write(p_sh1106, &read_modify_write_cmd, sizeof(read_modify_write_cmd), false);
}

ret_code_t sh1106_exit_read_modify_write_mode(sh1106_t * p_sh1106)
{
	uint8_t read_modify_write_exit_cmd = SH1106_CMD_READ_MODIFY_WRITE_END;

	return write(p_sh1106, &read_modify_write_exit_cmd, sizeof(read_modify_write_exit_cmd), false);
}

ret_code_t sh1106_write_data(sh1106_t * p_sh1106, const uint8_t * p_data, uint16_t size)
{
	uint8_t index = 0;
	for(uint8_t page = 0; page < 8; page++)
	{
		sh1106_set_page_address(p_sh1106, page);
		// sh1106_set_column_address(p_sh1106, 0);

		write(p_sh1106, &p_data[index], 132, true);

		index += 132;
	}

	return NRF_SUCCESS;
}

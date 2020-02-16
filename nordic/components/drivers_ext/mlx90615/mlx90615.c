/**
 * @file        mlx90615.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       MLX90615 infrared temperature sensor driver.
 * @version     0.1
 * @date        2020-02-15
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#include "mlx90615.h"
#include "mlx90615-internal.h"

#include "nrf_delay.h"

#define NRF_LOG_MODULE_NAME mlx90615
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"

#define MLX90615_TWI_WRITE_BUFFER_SIZE  1   /**< Maximum amount of data that will be transfered in one write. */
#define MLX90615_TWI_READ_BUFFER_SIZE   2   /**< Maximum amount of data that will be read in one read. */

typedef struct
{
	uint8_t reg_address;                          /**< Register address. */
	uint8_t data[MLX90615_TWI_WRITE_BUFFER_SIZE]; /**< Data buffer. */
} mlx90615_twi_write_t;

typedef struct
{
	uint8_t reg_address;                         /**< register address. */
	uint8_t data[MLX90615_TWI_READ_BUFFER_SIZE]; /**< Data buffer. */
} mlx90615_twi_read_t;

/**
 * @brief   Macro for converting raw data to Celcius represented as float.
 * 
 * @param   _raw_data   Raw MLX90615 data.
 */
#define RAW_TO_CELCIUS_FLOAT(_raw_data) (float)(((float)(_raw_data & 0x7FFF) * 0.02) - 273.15)

/**
 * @brief   Macro for converting raw data to Celcius represented as int8_t.
 * 
 * @param   _raw_data   Raw MLX90615 data.
 */
#define RAW_TO_CELCIUS_INT8(_raw_data) (int8_t)RAW_TO_CELCIUS_FLOAT(_raw_data)

/**
 * @brief       Function to be called by twi manager upon twi transaction result.
 * 
 * @param[in]   result      Transaction result.
 * @param[in]   p_user_data Pointer to user data that was supplied upon creation of twi request.
 */
static void twi_mngr_callback(ret_code_t result, void * p_user_data)
{
	if(result != NRF_SUCCESS)
	{
		mlx90615_t * p_mlx90615 = (mlx90615_t *)p_user_data;

		NRF_LOG_ERROR("Error: 0x%x", result);
	}
}

static ret_code_t twi_write(mlx90615_t const * p_mlx90615,
                            mlx90615_twi_write_t * p_twi_write,
                            uint8_t buffer_size)
{
	dk_twi_mngr_transaction_t twi_transaction =
	{
		.callback           = twi_mngr_callback,
		.p_user_data        = (void *)p_mlx90615,
		.transfer           = DK_TWI_MNGR_WRITE(MLX90615_SLAVE_ADDRESS, p_twi_write, buffer_size, 0)
	};

	return dk_twi_mngr_schedule(p_mlx90615->p_dk_twi_mngr_instance,
	                            &twi_transaction);
}

static void wait_for_transfer_complete()
{
	NRF_LOG_INFO("Wait amount");
	while(NRF_LOG_PROCESS());
	nrf_delay_ms(100);
}

static ret_code_t twi_read_blocking(mlx90615_t const * p_mlx90615,
                                    uint8_t   reg,
                                    uint8_t * p_buffer,
                                    uint8_t   buffer_size)
{
	dk_twi_mngr_transfer_t twi_transfer = DK_TWI_MNGR_TX_RX(MLX90615_SLAVE_ADDRESS, &reg, sizeof(reg), p_buffer, buffer_size, NRFX_TWI_FLAG_TX_NO_STOP);
	return dk_twi_mngr_perform(p_mlx90615->p_dk_twi_mngr_instance, &twi_transfer, wait_for_transfer_complete);
}

/**
 * @brief       Perform a TWI read operation.
 * 
 * @param[in]   p_i2c_instance  Pointer to I2C instance.
 * @param[in]   i2c_address     I2C slave address.
 * @param[in]   command         Command.
 * @param[out]  p_data          Pointer to where data will be read to.
 * @param[in]   data_length     Data length.
 * 
 * @retval      NRF_SUCCESS     On success.
 * @retval      Other           Error codes returned by nrfx_twi functions.
 */
static ret_code_t twi_read(nrfx_twi_t * p_i2c_instance, uint8_t i2c_address, uint8_t command, uint8_t * p_data, uint8_t data_length)
{
	ret_code_t err_code;

	err_code = nrfx_twi_tx(p_i2c_instance, i2c_address, &command, sizeof(command), true);
	VERIFY_SUCCESS(err_code);

	err_code = nrfx_twi_rx(p_i2c_instance, i2c_address, p_data, data_length);
	VERIFY_SUCCESS(err_code);

	return NRF_SUCCESS;
}

ret_code_t mlx90615_init(mlx90615_t * p_mlx90615, mlx90615_evt_handler_t evt_handler)
{
	ret_code_t err_code;
	uint16_t chip_id0;

	err_code = twi_read_blocking(p_mlx90615, MLX90615_CMD_EEPROM_ID0, &chip_id0, sizeof(chip_id0));
	VERIFY_SUCCESS(err_code);

	NRF_LOG_INFO("Chip id 0x%x", chip_id0);

	if (chip_id0 != MLX90615_ID0)
	{
		return NRF_ERROR_NOT_FOUND;
	}

	return NRF_SUCCESS;
}

ret_code_t mlx90615_read_amb_temp_int8(mlx90615_t * p_mlx90615)
{
	ret_code_t err_code;
	uint16_t data;

	// err_code = twi_read(p_mlx90615->p_i2c_instance, p_mlx90615->i2c_address, MLX90615_CMD_RAM_TAMB, (uint8_t *)&data, sizeof(data));
	// VERIFY_SUCCESS(err_code);

	// *p_temp_c = RAW_TO_CELCIUS_INT8(data);

	return NRF_SUCCESS;
}

ret_code_t mlx90615_read_obj_temp_int8(mlx90615_t * p_mlx90615)
{
	ret_code_t err_code;
	uint16_t data;

	// err_code = twi_read(p_mlx90615->p_i2c_instance, p_mlx90615->i2c_address, MLX90615_CMD_RAM_TOBJ1, (uint8_t *)&data, sizeof(data));
	// VERIFY_SUCCESS(err_code);

	// *p_temp_c = RAW_TO_CELCIUS_INT8(data);

	return NRF_SUCCESS;
}

ret_code_t mlx90615_read_amb_temp_float(mlx90615_t * p_mlx90615)
{
	ret_code_t err_code;
	uint16_t data;

	// err_code = twi_read(p_mlx90615->p_i2c_instance, p_mlx90615->i2c_address, MLX90615_CMD_RAM_TAMB, (uint8_t *)&data, sizeof(data));
	// VERIFY_SUCCESS(err_code);

	// *p_temp_c = RAW_TO_CELCIUS_FLOAT(data);

	return NRF_SUCCESS;
}

ret_code_t mlx90615_read_obj_temp_float(mlx90615_t * p_mlx90615)
{
	ret_code_t err_code;
	uint16_t data;

	// err_code = twi_read(p_mlx90615->p_i2c_instance, p_mlx90615->i2c_address, MLX90615_CMD_RAM_TOBJ1, (uint8_t *)&data, sizeof(data));
	// VERIFY_SUCCESS(err_code);

	// *p_temp_c = RAW_TO_CELCIUS_FLOAT(data);

	return NRF_SUCCESS;
}

ret_code_t mlx90615_sleep_mode_enter(mlx90615_t * p_mlx90615)
{
	ret_code_t err_code;
	uint8_t command[] = { MLX90615_CMD_ENTER_SLEEP, MLX90615_CMD_ENTER_SLEEP_PEC };

	// err_code = nrfx_twi_tx(p_mlx90615->p_i2c_instance, p_mlx90615->i2c_address, command, sizeof(command), false);
	// VERIFY_SUCCESS(err_code);

	return NRF_SUCCESS;
}

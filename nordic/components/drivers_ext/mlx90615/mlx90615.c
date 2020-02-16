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

/** @brief TWI write structure. */
typedef struct
{
	uint8_t reg_address;                          /**< Register address. */
	uint8_t data[MLX90615_TWI_WRITE_BUFFER_SIZE]; /**< Data buffer. */
} mlx90615_twi_write_t;

/** @brief TWI read structure. */
typedef struct
{
	uint8_t reg_address;                         /**< Register address. */
	uint8_t data[MLX90615_TWI_READ_BUFFER_SIZE]; /**< Data buffer. */
} mlx90615_twi_read_t;

/**
 * @brief   Macro for converting raw data to Celcius represented as float.
 * 
 * @param   p_raw_data  Pointer to raw MLX90615 data.
 */
#define RAW_TO_CELCIUS_FLOAT(p_raw_data) (float)(((float)(uint16_decode(p_raw_data) & 0x7FFF) * 0.02) - 273.15)

/**
 * @brief   Macro for converting raw data to Celcius represented as int8_t.
 * 
 * @param   p_raw_data  Pointer to raw MLX90615 data.
 */
#define RAW_TO_CELCIUS_INT8(p_raw_data) (int8_t)RAW_TO_CELCIUS_FLOAT(p_raw_data)

/**
 * @brief       Function to be called by twi manager upon twi transaction result.
 * 
 * @param[in]   result      Transaction result.
 * @param[in]   p_user_data Pointer to user data that was supplied upon creation of twi request.
 */

/**
 * @brief               Function to be called by twi manager upon twi transaction result.
 * 
 * @param[in]           result        Transaction result.
 * @param[in]           evt           Event.
 * @param[in]           p_transfer    Pointer to transfer data.
 * @param[in]           p_user_data   Pointer to user data.
 */
static void twi_mngr_callback(ret_code_t result, uint8_t evt, dk_twi_mngr_transfer_t * p_transfer, void * p_user_data)
{
	mlx90615_t * p_mlx90615 = (mlx90615_t *)p_user_data;

	mlx90615_evt_t mlx90615_evt =
	{
		.p_mlx90615 = p_mlx90615,
		.type       = evt
	};

	if(result != NRF_SUCCESS)
	{
		NRF_LOG_ERROR("Error: 0x%x", result);

		if(p_mlx90615->evt_handler)
		{
			mlx90615_evt.type            = MLX90615_EVT_TYPE_ERROR;
			mlx90615_evt.params.err_code = result;

			p_mlx90615->evt_handler(&mlx90615_evt);
		}
	}
	else
	{
		if(p_mlx90615->evt_handler == NULL)
		{
			return;
		}

		switch(evt)
		{
			case MLX90615_EVT_TYPE_AMB_TEMP_INT8_READY:
			case MLX90615_EVT_TYPE_OBJ_TEMP_INT8_READY:
				mlx90615_evt.params.int8_temp = RAW_TO_CELCIUS_INT8(p_transfer->transfer_description.p_secondary_buf);
				break;
			case MLX90615_EVT_TYPE_AMB_TEMP_FLOAT_READY:
			case MLX90615_EVT_TYPE_OBJ_TEMP_FLOAT_READY:
				mlx90615_evt.params.float_temp = RAW_TO_CELCIUS_FLOAT(p_transfer->transfer_description.p_secondary_buf);
				break;
			default:
				break;
		}

		p_mlx90615->evt_handler(&mlx90615_evt);
	}
}

/**
 * @brief       Function for scheduling a twi TX transfer using dk_twi_mngr.
 * 
 * @param[in]   p_mlx90615  Pointer to mlx90615 instance.
 * @param[in]   p_twi_write Pointer to write buffer.
 * @param[in]   write_size  Write buffer size.
 * @param[in]   evt_type    Event type.
 * 
 * @return      NRF_SUCCESS     Upon successful twi transaction scheduling.
 * @return      Other           Error codes returned by @dk_twi_mngr_schedule.
 */
static ret_code_t twi_write(mlx90615_t     const * p_mlx90615,
                            mlx90615_twi_write_t * p_twi_write,
                            uint8_t                write_size,
                            mlx90615_evt_type_t    evt_type)
{
	dk_twi_mngr_transaction_t twi_transaction =
	{
		.callback           = twi_mngr_callback,
		.p_user_data        = (void *)p_mlx90615,
		.event_type         = evt_type,
		.transfer           = DK_TWI_MNGR_TX(MLX90615_SLAVE_ADDRESS, (uint8_t *)p_twi_write, write_size, 0)
	};

	return dk_twi_mngr_schedule(p_mlx90615->p_dk_twi_mngr_instance,
	                            &twi_transaction);
}

/**
 * @brief   Function to be called by dk_twi_mngr when waiting for a tansfer to finish.
 *          Just waits for one millisecond.
 */
static void wait_for_transfer_complete()
{
	nrf_delay_ms(1);
}

/**
 * @brief       Perform a blocking twi read.
 * 
 * @note        Use this function when data must be read imediately (ie during initialization).
 * 
 * @param[in]   p_mlx90615  Pointer to mlx90615 instance.
 * @param[in]   reg         Register address to read from.
 * @param[in]   p_buffer    Pointer to read buffer.
 * @param[in]   buffer_size Read buffer size.
 * 
 * @return      NRF_SUCESS  Upon successful twi transaction.
 * @return      Other       Error codes returned by @dk_twi_mngr_perform function.
 */
static ret_code_t twi_read_blocking(mlx90615_t const * p_mlx90615,
                                    uint8_t   reg,
                                    uint8_t * p_buffer,
                                    uint8_t   buffer_size)
{
	dk_twi_mngr_transfer_t twi_transfer = DK_TWI_MNGR_TX_RX(MLX90615_SLAVE_ADDRESS,
	                                                        &reg,
	                                                        sizeof(reg),
	                                                        p_buffer,
	                                                        buffer_size,
	                                                        NRFX_TWI_FLAG_TX_NO_STOP);

	return dk_twi_mngr_perform(p_mlx90615->p_dk_twi_mngr_instance, &twi_transfer, wait_for_transfer_complete);
}

/**
 * @brief       Perform a TWI read operation (non-blocking).
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

/**
 * @brief       Perform a TWI read operation (non-blocking).
 * 
 * @param[in]   p_mlx90615  Pointer to MLX90615 instance.
 * @param[in]   p_twi_read  Pointer twi_read struct.
 * @param[in]   read_size   Amount of bytes to read.
 * @param[in]   evt_type    Event type.
 * 
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by dk_twi_mngr functions.
 */
static ret_code_t twi_read(mlx90615_t    const * p_mlx90615,
                           mlx90615_twi_read_t * p_twi_read,
                           uint8_t               read_size,
                           mlx90615_evt_type_t   evt_type)
{
	dk_twi_mngr_transaction_t twi_transaction =
	{
		.callback           = twi_mngr_callback,
		.p_user_data        = (void *)p_mlx90615,
		.event_type         = evt_type,
		.transfer           = DK_TWI_MNGR_TX_RX(MLX90615_SLAVE_ADDRESS,
		                                        &p_twi_read->reg_address,
		                                        sizeof(p_twi_read->reg_address),
		                                        p_twi_read->data,
		                                        read_size-1,
		                                        0)
	};

	return dk_twi_mngr_schedule(p_mlx90615->p_dk_twi_mngr_instance,
	                            &twi_transaction);
}

ret_code_t mlx90615_init(mlx90615_t * p_mlx90615, mlx90615_evt_handler_t evt_handler)
{
	ret_code_t err_code;
	uint16_t chip_id0;

	p_mlx90615->evt_handler = evt_handler;

	err_code = twi_read_blocking(p_mlx90615, MLX90615_CMD_EEPROM_ID0, (uint8_t *)&chip_id0, sizeof(chip_id0));
	VERIFY_SUCCESS(err_code);

	if (chip_id0 != MLX90615_ID0)
	{
		return NRF_ERROR_NOT_FOUND;
	}

	return NRF_SUCCESS;
}

ret_code_t mlx90615_read_amb_temp_int8(mlx90615_t * p_mlx90615)
{
	DK_TWI_MNGR_BUFF_ALLOC(mlx90615_twi_read_t, p_amb_read, sizeof(uint16_t));

	p_amb_read->reg_address = MLX90615_CMD_RAM_TAMB;

	return twi_read(p_mlx90615, p_amb_read, p_amb_read_size, MLX90615_EVT_TYPE_AMB_TEMP_INT8_READY);
}

ret_code_t mlx90615_read_obj_temp_int8(mlx90615_t * p_mlx90615)
{
	DK_TWI_MNGR_BUFF_ALLOC(mlx90615_twi_read_t, p_obj_read, sizeof(uint16_t));

	p_obj_read->reg_address = MLX90615_CMD_RAM_TOBJ;

	return twi_read(p_mlx90615, p_obj_read, p_obj_read_size, MLX90615_EVT_TYPE_OBJ_TEMP_INT8_READY);
}

ret_code_t mlx90615_read_amb_temp_float(mlx90615_t * p_mlx90615)
{
	DK_TWI_MNGR_BUFF_ALLOC(mlx90615_twi_read_t, p_amb_read, sizeof(uint16_t));

	p_amb_read->reg_address = MLX90615_CMD_RAM_TAMB;

	return twi_read(p_mlx90615, p_amb_read, p_amb_read_size, MLX90615_EVT_TYPE_AMB_TEMP_FLOAT_READY);
}

ret_code_t mlx90615_read_obj_temp_float(mlx90615_t * p_mlx90615)
{
	DK_TWI_MNGR_BUFF_ALLOC(mlx90615_twi_read_t, p_obj_read, sizeof(uint16_t));

	p_obj_read->reg_address = MLX90615_CMD_RAM_TOBJ;

	return twi_read(p_mlx90615, p_obj_read, p_obj_read_size, MLX90615_EVT_TYPE_OBJ_TEMP_FLOAT_READY);
}

ret_code_t mlx90615_sleep_mode_enter(mlx90615_t * p_mlx90615)
{
	DK_TWI_MNGR_BUFF_ALLOC(mlx90615_twi_write_t, p_sleep_cmd, sizeof(uint8_t));

	p_sleep_cmd->reg_address = MLX90615_CMD_ENTER_SLEEP;
	p_sleep_cmd->data[0]     = MLX90615_CMD_ENTER_SLEEP_PEC;

	return twi_write(p_mlx90615, p_sleep_cmd, p_sleep_cmd_size, MLX90615_CMD_ENTER_SLEEP);
}

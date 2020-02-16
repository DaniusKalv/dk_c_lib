/**
 * @file        mlx90615.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       MLX90615 infrared temperature sensor driver.
 * @version     0.1
 * @date        2020-02-15
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#ifndef MLX90615_H
#define MLX90615_H

#include "dk_twi_mngr.h"
#include "sdk_errors.h"
#include "dk_config.h"
#include "dk_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mlx90615_s mlx90615_t; // Forward declaraation

typedef enum
{
	MLX90615_EVT_TYPE_AMB_TEMP_INT8_READY,
	MLX90615_EVT_TYPE_OBJ_TEMP_INT8_READY,
	MLX90615_EVT_TYPE_AMB_TEMP_FLOAT_READY,
	MLX90615_EVT_TYPE_OBJ_TEMP_FLOAT_READY,
	MLX90615_EVT_TYPE_ENTER_SLEEP_SUCCESS,
	MLX90615_EVT_TYPE_ERROR
} mlx90615_evt_type_t;

typedef struct
{
	mlx90615_t        * p_mlx90615;
	mlx90615_evt_type_t evt_type;
	union
	{
		int8_t      int8_temp;
		float       float_temp;
		ret_code_t  err_code;
	} params;
} mlx90615_evt_t;

typedef void (* mlx90615_evt_handler_t)(mlx90615_evt_t * p_mlx90615_evt);

/** @brief MLX90615 driver structure. */
struct mlx90615_s
{
	const dk_twi_mngr_t     * p_dk_twi_mngr_instance; /**< Pointer to TWI manager instance. */
	mlx90615_evt_handler_t    evt_handler;            /**< Event handler. */
};

/**@brief   Macro for defining a MLX90615 instance.
 *
 * @param   _name                       Name of the instance.
 * @param   _p_dk_twi_mngr_instance     Pointer to twi manager instance.
 * @hideinitializer
 */
#define MLX90615_DEF(_name, _p_dk_twi_mngr_instance)    \
static mlx90615_t _name =                               \
{                                                       \
	.p_dk_twi_mngr_instance = _p_dk_twi_mngr_instance,  \
	.evt_handler            = NULL                      \
}

/**
 * @brief       Initialize MLX90615.
 * 
 * @param[in]   p_mlx90615          Pointer to MLX90615 instance.
 * @param[in]   evt_handler         Pointer to event handler function.
 * 
 * @retval      NRF_SUCCESS         On success.
 * @retval      NRF_ERROR_NOT_FOUND If value read from @ref MLX90615_CMD_EEPROM_ID0 did not match @ref MLX90615_ID0.
 * @retval      Other               Error codes returned by twi manager functions.
 */
ret_code_t mlx90615_init(mlx90615_t * p_mlx90615, mlx90615_evt_handler_t evt_handler);

/**
 * @brief       Read ambient temperature (int8 format).
 * 
 * @param[in]   p_mlx90615  Pointer to MLX90615 instance.
 * 
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by twi manager functions.
 */
ret_code_t mlx90615_read_amb_temp_int8(mlx90615_t * p_mlx90615);

/**
 * @brief       Read object temperature (int8 format).
 * 
 * @param[in]   p_mlx90615  Pointer to MLX90615 instance.
 * 
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by twi manager functions.
 */
ret_code_t mlx90615_read_obj_temp_int8(mlx90615_t * p_mlx90615);

/**
 * @brief       Read ambient temperature (float format).
 *) 
 * @param[in]   p_mlx90615  Pointer to MLX90615 instance.
 * 
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by twi manager functions.
 */
ret_code_t mlx90615_read_amb_temp_float(mlx90615_t * p_mlx90615);

/**
 * @brief       Read object temperature (float format).
 * 
 * @param[in]   p_mlx90615  Pointer to MLX90615 instance.
 * 
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by twi manager functions.
 */
ret_code_t mlx90615_read_obj_temp_float(mlx90615_t * p_mlx90615);

/**
 * @brief       Enter sleep mode.
 * 
 * @param[in]   p_mlx90615  Pointer to MLX90615 instance.
 * 
 * @retval      NRF_SUCCESS On success.
 * @retval      Other       Error codes returned by twi manager functions.
 */
ret_code_t mlx90615_sleep_mode_enter(mlx90615_t * p_mlx90615);

#ifdef __cplusplus
}
#endif

#endif // MLX90615_H

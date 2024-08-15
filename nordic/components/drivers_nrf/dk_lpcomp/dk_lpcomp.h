/**
 * @file        dk_lpcomp.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Low power comparator driver.
 * @version     0.2
 * @date        2024-08-15
 *
 * @copyright   Copyright (c) Danius Kalvaitis 2024 All rights reserved
 *
 */

#ifndef DK_LPCOMP_H
#define DK_LPCOMP_H

#include "sdk_errors.h"
#include <hal/nrf_lpcomp.h>

/** @brief Low power comparator event handler. */
typedef void (*dk_lpcomp_event_handler_t)(nrf_lpcomp_event_t event);

/** @brief Low power comparator interrupt flags. */
typedef struct
{
    bool ready_en : 1; /**< Enable ready interrupt. */
    bool down_en  : 1; /**< Enable down interrupt. */
    bool up_en    : 1; /**< Enable up interrupt. */
    bool cross_en : 1; /**< Enable cross interrupt. */
} dk_lpcomp_int_flags_t;

/** @brief Low power comparator configuration structure. */
typedef struct
{
    nrf_lpcomp_config_t   hal;                /**< HAL config. */
    nrf_lpcomp_input_t    input;              /**< Input selection. */
    uint8_t               interrupt_priority; /**< Interrupt priority. */
    dk_lpcomp_int_flags_t int_flags;          /**< Interrupt flags. */
} dk_lpcomp_config_t;

/** @brief Redeclaration of low power comparator task type. */
typedef nrf_lpcomp_task_t dk_lpcomp_task_t;

/** @brief Redeclaration of low power comparator event type. */
typedef nrf_lpcomp_event_t dk_lpcomp_event_t;

/** @brief Default @ref dk_lpcomp_int_flags_t values initialization. */
#define DK_LPCOMP_INT_FLAGS_DEFAULT_CONFIG {.ready_en = false, .down_en = false, .up_en = false, .cross_en = false}

#ifdef NRF52_SERIES
/** @brief Default @ref dk_lpcomp_config_t configuration. */
#define DK_LPCOMP_DEFAULT_CONFIG                                                                                       \
    {.hal                = {(nrf_lpcomp_ref_t)NRFX_LPCOMP_CONFIG_REFERENCE,                                            \
                            (nrf_lpcomp_detect_t)NRFX_LPCOMP_CONFIG_DETECTION,                                         \
                            (nrf_lpcomp_hysteresis_t)NRFX_LPCOMP_CONFIG_HYST},                                         \
     .input              = (nrf_lpcomp_input_t)NRFX_LPCOMP_CONFIG_INPUT,                                               \
     .interrupt_priority = NRFX_LPCOMP_CONFIG_IRQ_PRIORITY,                                                            \
     .int_flags          = DK_LPCOMP_INT_FLAGS_DEFAULT_CONFIG}
#else
/** @brief Default @ref dk_lpcomp_config_t configuration. */
#define DK_LPCOMP_DEFAULT_CONFIG                                                                                       \
    {.hal   = {(nrf_lpcomp_ref_t)NRFX_LPCOMP_CONFIG_REFERENCE, (nrf_lpcomp_detect_t)NRFX_LPCOMP_CONFIG_DETECTION},     \
     .input = (nrf_lpcomp_input_t)NRFX_LPCOMP_CONFIG_INPUT,                                                            \
     .interrupt_priority = NRFX_LPCOMP_CONFIG_IRQ_PRIORITY,                                                            \
     .int_flags          = DK_LPCOMP_INT_FLAGS_DEFAULT_CONFIG}
#endif

/**
 * @brief       Initialize low power comparator.
 *
 * @param[in]   p_config                    Pointer to configuration structure.
 * @param[in]   event_handler               Event handler.
 *
 * @retval      NRFX_SUCCESS                On success.
 * @retval      NRFX_ERROR_INVALID_STATE    If driver is already initialized.
 * @retval      NRFX_ERROR_BUSY             If nrfx_prs_acquire function fails.
 * @retval      Other                       Error codes returned by internal functions.
 */
ret_code_t dk_lpcomp_init(dk_lpcomp_config_t const *p_config, dk_lpcomp_event_handler_t event_handler);

/**
 * @brief   Get low power comparator task address.
 *
 * @param   lpcomp_task     Task address to get.
 *
 * @return  uint32_t        Low power comparator task address.
 */
uint32_t dk_lpcomp_task_address_get(dk_lpcomp_task_t lpcomp_task);

/**
 * @brief   Get low power comparator event address.
 *
 * @param   lpcomp_event    Event address to get.
 *
 * @return uint32_t         Low power comparator task address.
 */
uint32_t dk_lpcomp_event_address_get(dk_lpcomp_event_t lpcomp_event);

/**
 * @brief Deinitialize low power comparator.
 *
 */
void dk_lpcomp_deinit(void);

/**
 * @brief Enable low power comparator.
 *
 */
void dk_lpcomp_enable(void);

/**
 * @brief Disable low power comparator.
 *
 */
void dk_lpcomp_disable(void);

/**
 * @brief   Get low power comparator result value.
 *
 * @return  true    Input voltage is below the reference threshold.
 * @return  false   Input voltage is above the reference threshold.
 */
bool dk_lpcomp_result_get(void);

#endif

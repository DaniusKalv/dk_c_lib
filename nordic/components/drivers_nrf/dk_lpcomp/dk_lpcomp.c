/**
 * @file        dk_lpcomp.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Low power comparator driver.
 * @version     0.1
 * @date        2020-01-27
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2020 All rights reserved
 * 
 */

#include "dk_lpcomp.h"
#include "nrfx_common.h"
#if NRFX_CHECK(NRFX_PRS_ENABLED)
	#include "nrfx_prs.h"
#endif

static dk_lpcomp_event_handler_t m_lpcomp_event_handler = NULL; // Variable to save event handler pointer.
static nrfx_drv_state_t m_state = NRFX_DRV_STATE_UNINITIALIZED; // Variable to keep track of driver state.

/**
 * @brief       Low power comparator event handler execution function.
 * 
 * @param[in]   event       Low power comparator event.
 * @param[in]   event_mask  Low power comparator event mask.
 */
static void lpcomp_execute_handler(nrf_lpcomp_event_t event, uint32_t event_mask)
{
	if (nrf_lpcomp_event_check(event) && nrf_lpcomp_int_enable_check(event_mask))
	{
		nrf_lpcomp_event_clear(event);
		m_lpcomp_event_handler(event);
	}
}

/**
 * @brief Interrupt handler.
 * 
 */
void nrfx_lpcomp_irq_handler(void)
{
	lpcomp_execute_handler(NRF_LPCOMP_EVENT_READY, LPCOMP_INTENSET_READY_Msk);
	lpcomp_execute_handler(NRF_LPCOMP_EVENT_DOWN, LPCOMP_INTENSET_DOWN_Msk);
	lpcomp_execute_handler(NRF_LPCOMP_EVENT_UP, LPCOMP_INTENSET_UP_Msk);
	lpcomp_execute_handler(NRF_LPCOMP_EVENT_CROSS, LPCOMP_INTENSET_CROSS_Msk);
}

ret_code_t dk_lpcomp_init(dk_lpcomp_config_t const * p_config, dk_lpcomp_event_handler_t event_handler)
{
	NRFX_ASSERT(p_config);

	if (m_state != NRFX_DRV_STATE_UNINITIALIZED)
	{ // LPCOMP driver is already initialized
		return NRFX_ERROR_INVALID_STATE;
	}

	m_lpcomp_event_handler = event_handler;

#if NRFX_CHECK(NRFX_PRS_ENABLED)
	if (event_handler != NULL)
	{
		if (nrfx_prs_acquire(NRF_LPCOMP, nrfx_lpcomp_irq_handler) != NRFX_SUCCESS)
		{
			return NRFX_ERROR_BUSY;
		}
	}
#endif

	nrf_lpcomp_configure(&(p_config->hal));

	nrf_lpcomp_input_select(p_config->input);

	if (event_handler != NULL)
	{
		if (p_config->int_flags.ready_en)
		{
			nrf_lpcomp_int_enable(LPCOMP_INTENSET_READY_Msk);
		}

		if (p_config->int_flags.down_en)
		{
			nrf_lpcomp_int_enable(LPCOMP_INTENSET_DOWN_Msk);
		}

		if (p_config->int_flags.up_en)
		{
			nrf_lpcomp_int_enable(LPCOMP_INTENSET_UP_Msk);
		}

		if (p_config->int_flags.cross_en)
		{
			nrf_lpcomp_int_enable(LPCOMP_INTENSET_CROSS_Msk);
		}
	}

	nrf_lpcomp_shorts_enable(NRF_LPCOMP_SHORT_READY_SAMPLE_MASK);

	if (event_handler != NULL)
	{
		NRFX_IRQ_PRIORITY_SET(LPCOMP_IRQn, p_config->interrupt_priority);
		NRFX_IRQ_ENABLE(LPCOMP_IRQn);
	}

	m_state = NRFX_DRV_STATE_INITIALIZED;

	return NRFX_SUCCESS;
}

uint32_t dk_lpcomp_task_address_get(dk_lpcomp_task_t lpcomp_task)
{
	return (uint32_t)nrf_lpcomp_task_address_get(lpcomp_task);
}

uint32_t dk_lpcomp_event_address_get(dk_lpcomp_event_t lpcomp_event)
{
	return (uint32_t)nrf_lpcomp_event_address_get(lpcomp_event);
}

void dk_lpcomp_deinit(void)
{
	NRFX_ASSERT(m_state != NRFX_DRV_STATE_UNINITIALIZED);

	if(m_lpcomp_event_handler != NULL)
	{
		NRFX_IRQ_DISABLE(LPCOMP_IRQn);
	}
	
	dk_lpcomp_disable();

#if NRFX_CHECK(NRFX_PRS_ENABLED)
	if(m_lpcomp_event_handler != NULL)
	{
		nrfx_prs_release(NRF_LPCOMP);
	}
#endif

	m_state = NRFX_DRV_STATE_UNINITIALIZED;
	m_lpcomp_event_handler = NULL;
}

void dk_lpcomp_enable(void)
{
	NRFX_ASSERT(m_state == NRFX_DRV_STATE_INITIALIZED);
	nrf_lpcomp_enable();
	nrf_lpcomp_task_trigger(NRF_LPCOMP_TASK_START);
	m_state = NRFX_DRV_STATE_POWERED_ON;
}

void dk_lpcomp_disable(void)
{
	NRFX_ASSERT(m_state == NRFX_DRV_STATE_POWERED_ON);
	nrf_lpcomp_disable();
	nrf_lpcomp_task_trigger(NRF_LPCOMP_TASK_STOP);
	m_state = NRFX_DRV_STATE_INITIALIZED;
}

bool dk_lpcomp_result_get(void)
{
	return (bool)NRF_LPCOMP->RESULT;
}

/**
 * @file        dk_uicr.c
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       DK UICR register data.
 * @version     0.1
 * @date        2019-11-23
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_uicr.h"
#include "dk_config.h"

/* Write HW ID and HW version values to UICR. */
volatile hug_uicr_hw_info_t hug_uicr_hw_info __attribute__ ((section(".dk_uicr_hw_info"))) = 
{
	.hw_id  = HW_ID,
	.hw_v   = HW_VERSION
};

#ifdef NRF52840_XXAA

/* Write HUG_UICR_REGOUT0 value to vout UICR register. */
volatile hug_uicr_regout0_t hug_uicr_regout0 __attribute__ ((section(".uicr_regout0"))) =
{
	.vout = DK_UICR_REGOUT0
};

#endif

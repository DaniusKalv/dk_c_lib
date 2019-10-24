/**
 * @file dk_uicr.c
 * @author Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @version 0.1
 * @date 2019-10-20
 * 
 * @copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#include "dk_uicr.h"

/* Write HW ID and HW version values to UICR. */
volatile hug_uicr_hw_info_t hug_uicr_hw_info __attribute__ ((section(".dk_uicr_hw_info"))) = 
{
	.hw_id  = HW_ID,
	.hw_v   = HW_VERSION
};

#ifdef NRF52840_XXAA

#if(HUG_UICR_REGOUT0_ENABLED)
/* Write HUG_UICR_REGOUT0 value to vout UICR register. */
volatile hug_uicr_regout0_t hug_uicr_regout0 __attribute__ ((section(".uicr_regout0"))) =
{
	.vout = HUG_UICR_REGOUT0
};
#endif

#endif

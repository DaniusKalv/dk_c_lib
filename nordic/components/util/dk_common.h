/**
 * @file        dk_common.h
 * @author      Danius Kalvaitis (danius.kalvaitis@gmail.com)
 * @brief       Common defines and macros for firmware developed by Danius Kalvaitis.
 * @version     0.1
 * @date        2019-11-26
 * 
 * @copyright   Copyright (c) Danius Kalvaitis 2019 All rights reserved
 * 
 */

#ifndef DK_COMMON_H
#define DK_COMMON_H

/**
 * @brief Check if selected module is enabled
 *
 * Correct from Lint point of view (not using default of undefined value).
 *
 * Usage:
 * @code
   #if DK_MODULE_ENABLED(DK_BATTERY_LVL)
   ...
   #endif
 * @endcode
 *
 * @param module The module name.
 *
 * @retval 1 The macro <module>_ENABLE is defined and is non-zero.
 * @retval 0 The macro <module>_ENABLE is not defined or it equals zero.
 *
 * @note
 * This macro intentionally does not implement second expansion level.
 * The name of the module to be checked has to be given directly as a parameter.
 * And given parameter would be connected with @c _ENABLED postfix directly
 * without evaluating its value.
 */
#define DK_MODULE_ENABLED(module) \
    ((defined(module ## _ENABLED) && (module ## _ENABLED)) ? 1 : 0)

/**
 * @brief Macro for checking if the specified identifier is defined and it has
 *        a non-zero value.
 *
 * Normally, preprocessors treat all undefined identifiers as having the value
 * zero. However, some tools, like static code analyzers, can issue a warning
 * when such identifier is evaluated. This macro gives the possibility to suppress
 * such warnings only in places where this macro is used for evaluation, not in
 * the whole analyzed code.
 */
#define DK_CHECK(module_enabled)  (module_enabled)

#endif // DK_COMMON_H

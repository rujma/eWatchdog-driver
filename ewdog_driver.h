#ifndef EWDOG_DRIVER_H
#define EWDOG_DRIVER_H

/*!
 * @file ewdog_driver.h
 * @brief External Watchdog Peripheral Access Layer
 *
 */

/* ----------------------------------------------------------------------------
   -- External Watchdog Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup External_Watchdog External Watchdog Peripheral Access Layer
 * @{
 */


/*!
 * @brief Enumeration of the possible external watchdog status
 *
 * Implements : ewd_status_t
 */
typedef enum
{
    EWD_ENABLED         = 0U,       /**< External watchdog enabled */
    EWD_DISABLED        = 1U,       /**< External watchdog disabled */
    EWD_STATUS_SUCCESS  = 2U,       /**< External watchdog config success */
    EWD_STATUS_ERROR    = 3U        /**< External watchdog config error */
}ewd_status_t;

/*!
 * @brief Configures the external watchdog
 *
 * This function configures the external watchdog to the
 * correct board pins
 * 
 */

void EWD_Config(void);

/*!
 * @brief Enable the external watchdog
 *
 * This function enables the external watchdog
 * using board GPIO 
 * 
 */
void EWD_Enable(void);

/*!
 * @brief Disable the external watchdog
 *
 * This function disables the external watchdog
 * using board GPIO 
 * 
 */
void EWD_Disable(void);

/*!
 * @brief Checks the status of external watchdog
 *
 * This function checks if the watchdog is enabled or
 * disabled
 * 
 * @param[out] the success 1 or failure 0 of the configuration
 */
ewd_status_t EWD_Is_Enabled(void);

/*!
 * @brief Kick the watchdog
 *
 * This function toggles the WDI input
 * to reset the watchdog
 * 
 */
void EWD_Kick(void);

/*!
 * @}
 */ /* end of group External_Watchdog */



#endif
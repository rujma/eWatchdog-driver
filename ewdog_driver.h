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
    EWD_ENABLED         = 0x00U,
    EWD_DISABLED        = 0x01U,
    EWD_STATUS_SUCCESS  = 0x10U,
    EWD_STATUS_ERROR    = 0x11U
}ewd_status_t;

/*!
 * @brief Configures the external watchdog
 *
 * This function configures the external watchdog to the
 * correct board pins
 * 
 * @param[out] the success 1 or failure 0 of the configuration
 */

ewd_status_t EWD_Config(void);

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
 * @brief Starts the pulses to periodically reset the 
 * external watchdog
 *
 * This function starts the PWM to periodically reset 
 * the external watchdog
 * 
 */
void EWD_Start(void);

/*!
 * @brief Stops the pulses that periodically reset the 
 * external watchdog
 *
 * This function stops the PWM that periodically reset 
 * the external watchdog
 * 
 */
void EWD_Stop(void);


/*!
 * @}
 */ /* end of group External_Watchdog */



#endif
#include "common/device_registers.h"
#include "ewdog_driver.h"

#define ENABLE_PORT          PORTC
#define ENABLE_GPIO_PORT     PTC 
#define ENABLE_PIN           13U

/*!
 * @brief Configures the external watchdog
 *
 * This function configures the external watchdog to the
 * correct board pins
 * 
 * @param[out] the success 1 or failure 0 of the configuration
 */

ewd_status_t EWD_Config(void)
{
    /* Configure enable pin */
    uint32_t reg, directions;
    reg = ENABLE_PORT->PCR[ENABLE_PIN];
    reg &= ~(PORT_PCR_MUX_MASK);
    reg |= PORT_PCR_MUX(1U);
    ENABLE_PORT->PCR[ENABLE_PIN] = reg;
    directions = ENABLE_GPIO_PORT->PDDR;
    directions |= (1UL << ENABLE_PIN);
    ENABLE_GPIO_PORT->PDDR = directions; 
    /* Configure pwm */
}

/*!
 * @brief Enable the external watchdog
 *
 * This function enables the external watchdog
 * using board GPIO 
 * 
 */
void EWD_Enable(void)
{
    /* Set the external watchdog EN pin to low */
    uint32_t pins = ENABLE_GPIO_PORT->PDOR;
    pins &= ~(1U << ENABLE_PIN);
    pins |= (1U << ENABLE_PIN);
    ENABLE_GPIO_PORT->PDOR = GPIO_PDOR_PDO(pins);
}

/*!
 * @brief Disable the external watchdog
 *
 * This function disables the external watchdog
 * using board GPIO 
 * 
 */
void EWD_Disable(void)
{
    /* Set the external watchdog EN pin to high */
    uint32_t pins = (uint32_t)ENABLE_GPIO_PORT->PDOR;
    pins &= (uint32_t)(~((uint32_t)1U << ENABLE_PIN));
    ENABLE_GPIO_PORT->PDOR = GPIO_PDOR_PDO(pins);
}

/*!
 * @brief Checks the status of external watchdog
 *
 * This function checks if the watchdog is enabled or
 * disabled
 * 
 * @param[out] the success 1 or failure 0 of the configuration
 */
ewd_status_t EWD_Is_Enabled(void)
{
    uint32_t pins = (uint32_t)(ENABLE_GPIO_PORT->PDOR);
    pins = (pins & (uint32_t)(1 << ENABLE_PIN)) >> ENABLE_PIN;
    if(pins == 0)
    {
        return EWD_ENABLED;
    }
    else
    {
        return EWD_DISABLED;
    }
    
}

/*!
 * @brief Starts the pulses to periodically reset the 
 * external watchdog
 *
 * This function starts the PWM to periodically resets
 * the external watchdog
 * 
 */
void EWD_Start(void)
{
    /* Set the clock source for the PWM */
    FTM0->SC = (((FTM0->SC) & ~(FTM_SC_CLKS_MASK)) | FTM_SC_CLKS(0x01U));
}

/*!
 * @brief Stops the pulses that periodically reset the 
 * external watchdog
 *
 * This function stops the PWM that periodically resets 
 * the external watchdog
 * 
 */
void EWD_Stop(void)
{
    /* Unset the clock source for the PWM */
    FTM0->SC = (((FTM0->SC) & ~(FTM_SC_CLKS_MASK)) | FTM_SC_CLKS(0x00U));
}


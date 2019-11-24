#include "common/device_registers.h"
#include "ewdog_driver.h"

#define ENABLE_PORT          PORTC
#define ENABLE_GPIO_PORT     PTC 
#define ENABLE_PIN           8U
#define WDI_PORT             PORTB
#define WDI_GPIO_PORT        PTB
#define WDI_PIN              6U

/*FUNCTION**********************************************************************
 *
 * Function Name : EWD_Config
 * Description   : This function configures the enable pin and the wdi pin
 *
 *END**************************************************************************/
void EWD_Config(void)
{
    /* CHECK S32K116EVB - FLEXTIMER MODULE AND S32K1XX COOKBOOK FOR CONFIGURATIONS */
    /* Configure enable pin */
    uint32_t reg, directions;
    reg = ENABLE_PORT->PCR[ENABLE_PIN];
    reg &= ~(PORT_PCR_MUX_MASK);
    reg |= PORT_PCR_MUX(1U);
    ENABLE_PORT->PCR[ENABLE_PIN] = reg;
    directions = ENABLE_GPIO_PORT->PDDR;
    directions |= (1UL << ENABLE_PIN);
    ENABLE_GPIO_PORT->PDDR = directions;
    /* Configure WDI pin */
    reg = (uint32_t) 0;
    directions = (uint32_t) 0;
    reg = WDI_PORT->PCR[WDI_PIN];
    reg &= ~(PORT_PCR_MUX_MASK);
    reg |= PORT_PCR_MUX(1U);
    WDI_PORT->PCR[WDI_PIN] = reg;
    directions = WDI_GPIO_PORT->PDDR;
    directions |= (1UL << WDI_PIN);
    WDI_GPIO_PORT->PDDR = directions;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EWD_Enable
 * Description   : This function toggles the enable pin output
 * to enable the external watchdog
 *
 *END**************************************************************************/
void EWD_Enable(void)
{
    /* Set the external watchdog EN pin to low */
    uint32_t pins = ENABLE_GPIO_PORT->PDOR;
    pins &= ~(1U << ENABLE_PIN);
    pins |= (1U << ENABLE_PIN);
    ENABLE_GPIO_PORT->PDOR = GPIO_PDOR_PDO(pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EWD_Disable
 * Description   : This function toggles the disable pin output
 * to enable the external watchdog
 *
 *END**************************************************************************/
void EWD_Disable(void)
{
    /* Set the external watchdog EN pin to high */
    uint32_t pins = (uint32_t)ENABLE_GPIO_PORT->PDOR;
    pins &= (uint32_t)(~((uint32_t)1U << ENABLE_PIN));
    ENABLE_GPIO_PORT->PDOR = GPIO_PDOR_PDO(pins);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EWD_Enable
 * Description   : This function returns the status of the enable pin of 
 * the external watchdog
 *
 *END**************************************************************************/
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

/*FUNCTION**********************************************************************
 *
 * Function Name : EWD_Kick
 * Description   : This function toggles the WDI pin to reset the watchdog
 * 
 *END**************************************************************************/
void EWD_Kick(void)
{
    /* Ticks to wait for pulse duration */
    unsigned long n = 50;
    /* Unset the WDI pin */
    uint32_t pins = (uint32_t)WDI_GPIO_PORT->PDOR;
    pins &= (uint32_t)(~((uint32_t)1U << WDI_PIN));
    WDI_GPIO_PORT->PDOR = GPIO_PDOR_PDO(pins);
    /* Wait for 1us */
    while(n--){
        __asm volatile ("NOP");
    }
    /* Set the WDI pin */
    pins = (uint32_t) 0;
    pins = (uint32_t)WDI_GPIO_PORT->PDOR;
    pins &= ~(1U << WDI_PIN);
    pins |= (1U << WDI_PIN);
    WDI_GPIO_PORT->PDOR = GPIO_PDOR_PDO(pins);
}





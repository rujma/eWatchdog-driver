#include "common/device_registers.h"
#include "ewdog_driver.h"

#define ENABLE_PORT          PORTC
#define ENABLE_GPIO_PORT     PTC 
#define ENABLE_PIN           8U
#define PWM_PORT             PORTB
#define PWM_PIN              6U

/*FUNCTION**********************************************************************
 *
 * Function Name : EWD_Config
 * Description   : This function configures the enable pin, the pwm pin and the
 * FTM1 timer to generate a PWM
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
    /* Configure pwm pin */
    reg = (uint32_t) 0;
    reg = PWM_PORT->PCR[PWM_PIN];
    reg &= ~(PORT_PCR_MUX_MASK);
    reg |= PORT_PCR_MUX(2U);
    PWM_PORT->PCR[PWM_PIN] = reg;
    /* Configure pwm timer */
    FTM1->MODE |= FTM_MODE_WPDIS(1U);                   /* Disable register write protections */
    FTM1->FMS = FTM_FMS_WPEN(0U);
    FTM1->CONF = FTM_CONF_BDMMODE(3U);                  /* Set up debug mode */
    FTM1->SC = FTM_SC_PWMEN6(1U) |                      /* Select channel 6 output */
               FTM_SC_TOIE(0U) |                        /* No timer overflow flag */
               FTM_SC_CPWMS(0U) |                       /* center aligned pwm up count */
               FTM_SC_CLKS(0U) |                        /* No clock (disabled) */
               FTM_SC_PS(6U);                           /* Prescaler of 64: 48MHz / 64 = 750 kHz */
    FTM1->COMBINE = (uint32_t) 0;
    FTM1->POL = (uint32_t) 0;
    FTM1->MOD = 62500 - 1;                              /* Period = MOD - CNTIN + 1 ticks */
    FTM1->CONTROLS[PWM_PIN].CnSC = FTM_CnSC_MSB(1U) |   /* FTM1 ch6: edge-aligned PWM, low true pulses */
                                   FTM_CnSC_MSA(0U) |   /* MSB:MSA (chan Mode Select)=0b1X, Edge Align PWM*/
                                   FTM_CnSC_ELSB(0U) |  /* ELSB:ELSA (chan Edge/Level Select)=0bX1, low true */
                                   FTM_CnSC_ELSA(1U);
    FTM1->CONTROLS[PWM_PIN].CnV = 46875;                /* FTM1 ch6 compare value (~75% duty cycle) */
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
 * Function Name : EWD_Enable
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
 * Function Name : EWD_Enable
 * Description   : This function stops the PWM by unassigning the clock source
 * for the FTM1 timer
 * 
 *END**************************************************************************/
void EWD_Start(void)
{
    /* Set the clock source for the PWM */
    FTM0->SC = (((FTM0->SC) & ~(FTM_SC_CLKS_MASK)) | FTM_SC_CLKS(0x01U));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EWD_Enable
 * Description   : This function starts the PWM by assigning the clock source
 * for the FTM1 timer
 * 
 *END**************************************************************************/
void EWD_Stop(void)
{
    /* Unset the clock source for the PWM */
    FTM0->SC = (((FTM0->SC) & ~(FTM_SC_CLKS_MASK)) | FTM_SC_CLKS(0x00U));
}


/*******************************************************************************
* File Name: ADC_RADAR_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_RADAR.h"


/***************************************
* Local data allocation
***************************************/

static ADC_RADAR_BACKUP_STRUCT  ADC_RADAR_backup =
{
    ADC_RADAR_DISABLED
};


/*******************************************************************************
* Function Name: ADC_RADAR_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_RADAR_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_RADAR_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_RADAR_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_RADAR_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_RADAR_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_RADAR_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_RADAR_Sleep(void)
{
    if((ADC_RADAR_PWRMGR_SAR_REG  & ADC_RADAR_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_RADAR_SAR_CSR0_REG & ADC_RADAR_SAR_SOF_START_CONV) != 0u)
        {
            ADC_RADAR_backup.enableState = ADC_RADAR_ENABLED | ADC_RADAR_STARTED;
        }
        else
        {
            ADC_RADAR_backup.enableState = ADC_RADAR_ENABLED;
        }
        ADC_RADAR_Stop();
    }
    else
    {
        ADC_RADAR_backup.enableState = ADC_RADAR_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_RADAR_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_RADAR_Sleep() was called. If the component was enabled before the
*  ADC_RADAR_Sleep() function was called, the
*  ADC_RADAR_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_RADAR_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_RADAR_Wakeup(void)
{
    if(ADC_RADAR_backup.enableState != ADC_RADAR_DISABLED)
    {
        ADC_RADAR_Enable();
        #if(ADC_RADAR_DEFAULT_CONV_MODE != ADC_RADAR__HARDWARE_TRIGGER)
            if((ADC_RADAR_backup.enableState & ADC_RADAR_STARTED) != 0u)
            {
                ADC_RADAR_StartConvert();
            }
        #endif /* End ADC_RADAR_DEFAULT_CONV_MODE != ADC_RADAR__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */

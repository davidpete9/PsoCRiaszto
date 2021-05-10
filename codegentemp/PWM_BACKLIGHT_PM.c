/*******************************************************************************
* File Name: PWM_BACKLIGHT_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_BACKLIGHT.h"

static PWM_BACKLIGHT_backupStruct PWM_BACKLIGHT_backup;


/*******************************************************************************
* Function Name: PWM_BACKLIGHT_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_BACKLIGHT_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_BACKLIGHT_SaveConfig(void) 
{

    #if(!PWM_BACKLIGHT_UsingFixedFunction)
        #if(!PWM_BACKLIGHT_PWMModeIsCenterAligned)
            PWM_BACKLIGHT_backup.PWMPeriod = PWM_BACKLIGHT_ReadPeriod();
        #endif /* (!PWM_BACKLIGHT_PWMModeIsCenterAligned) */
        PWM_BACKLIGHT_backup.PWMUdb = PWM_BACKLIGHT_ReadCounter();
        #if (PWM_BACKLIGHT_UseStatus)
            PWM_BACKLIGHT_backup.InterruptMaskValue = PWM_BACKLIGHT_STATUS_MASK;
        #endif /* (PWM_BACKLIGHT_UseStatus) */

        #if(PWM_BACKLIGHT_DeadBandMode == PWM_BACKLIGHT__B_PWM__DBM_256_CLOCKS || \
            PWM_BACKLIGHT_DeadBandMode == PWM_BACKLIGHT__B_PWM__DBM_2_4_CLOCKS)
            PWM_BACKLIGHT_backup.PWMdeadBandValue = PWM_BACKLIGHT_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_BACKLIGHT_KillModeMinTime)
             PWM_BACKLIGHT_backup.PWMKillCounterPeriod = PWM_BACKLIGHT_ReadKillTime();
        #endif /* (PWM_BACKLIGHT_KillModeMinTime) */

        #if(PWM_BACKLIGHT_UseControl)
            PWM_BACKLIGHT_backup.PWMControlRegister = PWM_BACKLIGHT_ReadControlRegister();
        #endif /* (PWM_BACKLIGHT_UseControl) */
    #endif  /* (!PWM_BACKLIGHT_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_BACKLIGHT_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_BACKLIGHT_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_BACKLIGHT_RestoreConfig(void) 
{
        #if(!PWM_BACKLIGHT_UsingFixedFunction)
            #if(!PWM_BACKLIGHT_PWMModeIsCenterAligned)
                PWM_BACKLIGHT_WritePeriod(PWM_BACKLIGHT_backup.PWMPeriod);
            #endif /* (!PWM_BACKLIGHT_PWMModeIsCenterAligned) */

            PWM_BACKLIGHT_WriteCounter(PWM_BACKLIGHT_backup.PWMUdb);

            #if (PWM_BACKLIGHT_UseStatus)
                PWM_BACKLIGHT_STATUS_MASK = PWM_BACKLIGHT_backup.InterruptMaskValue;
            #endif /* (PWM_BACKLIGHT_UseStatus) */

            #if(PWM_BACKLIGHT_DeadBandMode == PWM_BACKLIGHT__B_PWM__DBM_256_CLOCKS || \
                PWM_BACKLIGHT_DeadBandMode == PWM_BACKLIGHT__B_PWM__DBM_2_4_CLOCKS)
                PWM_BACKLIGHT_WriteDeadTime(PWM_BACKLIGHT_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_BACKLIGHT_KillModeMinTime)
                PWM_BACKLIGHT_WriteKillTime(PWM_BACKLIGHT_backup.PWMKillCounterPeriod);
            #endif /* (PWM_BACKLIGHT_KillModeMinTime) */

            #if(PWM_BACKLIGHT_UseControl)
                PWM_BACKLIGHT_WriteControlRegister(PWM_BACKLIGHT_backup.PWMControlRegister);
            #endif /* (PWM_BACKLIGHT_UseControl) */
        #endif  /* (!PWM_BACKLIGHT_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_BACKLIGHT_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_BACKLIGHT_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_BACKLIGHT_Sleep(void) 
{
    #if(PWM_BACKLIGHT_UseControl)
        if(PWM_BACKLIGHT_CTRL_ENABLE == (PWM_BACKLIGHT_CONTROL & PWM_BACKLIGHT_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_BACKLIGHT_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_BACKLIGHT_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_BACKLIGHT_UseControl) */

    /* Stop component */
    PWM_BACKLIGHT_Stop();

    /* Save registers configuration */
    PWM_BACKLIGHT_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_BACKLIGHT_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_BACKLIGHT_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_BACKLIGHT_Wakeup(void) 
{
     /* Restore registers values */
    PWM_BACKLIGHT_RestoreConfig();

    if(PWM_BACKLIGHT_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_BACKLIGHT_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

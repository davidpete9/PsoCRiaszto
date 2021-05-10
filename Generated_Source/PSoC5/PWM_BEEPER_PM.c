/*******************************************************************************
* File Name: PWM_BEEPER_PM.c
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

#include "PWM_BEEPER.h"

static PWM_BEEPER_backupStruct PWM_BEEPER_backup;


/*******************************************************************************
* Function Name: PWM_BEEPER_SaveConfig
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
*  PWM_BEEPER_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_BEEPER_SaveConfig(void) 
{

    #if(!PWM_BEEPER_UsingFixedFunction)
        #if(!PWM_BEEPER_PWMModeIsCenterAligned)
            PWM_BEEPER_backup.PWMPeriod = PWM_BEEPER_ReadPeriod();
        #endif /* (!PWM_BEEPER_PWMModeIsCenterAligned) */
        PWM_BEEPER_backup.PWMUdb = PWM_BEEPER_ReadCounter();
        #if (PWM_BEEPER_UseStatus)
            PWM_BEEPER_backup.InterruptMaskValue = PWM_BEEPER_STATUS_MASK;
        #endif /* (PWM_BEEPER_UseStatus) */

        #if(PWM_BEEPER_DeadBandMode == PWM_BEEPER__B_PWM__DBM_256_CLOCKS || \
            PWM_BEEPER_DeadBandMode == PWM_BEEPER__B_PWM__DBM_2_4_CLOCKS)
            PWM_BEEPER_backup.PWMdeadBandValue = PWM_BEEPER_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_BEEPER_KillModeMinTime)
             PWM_BEEPER_backup.PWMKillCounterPeriod = PWM_BEEPER_ReadKillTime();
        #endif /* (PWM_BEEPER_KillModeMinTime) */

        #if(PWM_BEEPER_UseControl)
            PWM_BEEPER_backup.PWMControlRegister = PWM_BEEPER_ReadControlRegister();
        #endif /* (PWM_BEEPER_UseControl) */
    #endif  /* (!PWM_BEEPER_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_BEEPER_RestoreConfig
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
*  PWM_BEEPER_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_BEEPER_RestoreConfig(void) 
{
        #if(!PWM_BEEPER_UsingFixedFunction)
            #if(!PWM_BEEPER_PWMModeIsCenterAligned)
                PWM_BEEPER_WritePeriod(PWM_BEEPER_backup.PWMPeriod);
            #endif /* (!PWM_BEEPER_PWMModeIsCenterAligned) */

            PWM_BEEPER_WriteCounter(PWM_BEEPER_backup.PWMUdb);

            #if (PWM_BEEPER_UseStatus)
                PWM_BEEPER_STATUS_MASK = PWM_BEEPER_backup.InterruptMaskValue;
            #endif /* (PWM_BEEPER_UseStatus) */

            #if(PWM_BEEPER_DeadBandMode == PWM_BEEPER__B_PWM__DBM_256_CLOCKS || \
                PWM_BEEPER_DeadBandMode == PWM_BEEPER__B_PWM__DBM_2_4_CLOCKS)
                PWM_BEEPER_WriteDeadTime(PWM_BEEPER_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_BEEPER_KillModeMinTime)
                PWM_BEEPER_WriteKillTime(PWM_BEEPER_backup.PWMKillCounterPeriod);
            #endif /* (PWM_BEEPER_KillModeMinTime) */

            #if(PWM_BEEPER_UseControl)
                PWM_BEEPER_WriteControlRegister(PWM_BEEPER_backup.PWMControlRegister);
            #endif /* (PWM_BEEPER_UseControl) */
        #endif  /* (!PWM_BEEPER_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_BEEPER_Sleep
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
*  PWM_BEEPER_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_BEEPER_Sleep(void) 
{
    #if(PWM_BEEPER_UseControl)
        if(PWM_BEEPER_CTRL_ENABLE == (PWM_BEEPER_CONTROL & PWM_BEEPER_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_BEEPER_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_BEEPER_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_BEEPER_UseControl) */

    /* Stop component */
    PWM_BEEPER_Stop();

    /* Save registers configuration */
    PWM_BEEPER_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_BEEPER_Wakeup
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
*  PWM_BEEPER_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_BEEPER_Wakeup(void) 
{
     /* Restore registers values */
    PWM_BEEPER_RestoreConfig();

    if(PWM_BEEPER_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_BEEPER_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

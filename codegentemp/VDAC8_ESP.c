/*******************************************************************************
* File Name: VDAC8_ESP.c  
* Version 1.90
*
* Description:
*  This file provides the source code to the API for the 8-bit Voltage DAC 
*  (VDAC8) User Module.
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "VDAC8_ESP.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 VDAC8_ESP_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 VDAC8_ESP_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static VDAC8_ESP_backupStruct VDAC8_ESP_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: VDAC8_ESP_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC8_ESP_Init(void) 
{
    VDAC8_ESP_CR0 = (VDAC8_ESP_MODE_V );

    /* Set default data source */
    #if(VDAC8_ESP_DEFAULT_DATA_SRC != 0 )
        VDAC8_ESP_CR1 = (VDAC8_ESP_DEFAULT_CNTL | VDAC8_ESP_DACBUS_ENABLE) ;
    #else
        VDAC8_ESP_CR1 = (VDAC8_ESP_DEFAULT_CNTL | VDAC8_ESP_DACBUS_DISABLE) ;
    #endif /* (VDAC8_ESP_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(VDAC8_ESP_DEFAULT_STRB != 0)
        VDAC8_ESP_Strobe |= VDAC8_ESP_STRB_EN ;
    #endif/* (VDAC8_ESP_DEFAULT_STRB != 0) */

    /* Set default range */
    VDAC8_ESP_SetRange(VDAC8_ESP_DEFAULT_RANGE); 

    /* Set default speed */
    VDAC8_ESP_SetSpeed(VDAC8_ESP_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: VDAC8_ESP_Enable
********************************************************************************
* Summary:
*  Enable the VDAC8
* 
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC8_ESP_Enable(void) 
{
    VDAC8_ESP_PWRMGR |= VDAC8_ESP_ACT_PWR_EN;
    VDAC8_ESP_STBY_PWRMGR |= VDAC8_ESP_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(VDAC8_ESP_restoreVal == 1u) 
        {
             VDAC8_ESP_CR0 = VDAC8_ESP_backup.data_value;
             VDAC8_ESP_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VDAC8_ESP_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as
*  executing the stop function.
*
* Parameters:
*  Power: Sets power level between off (0) and (3) high power
*
* Return:
*  void 
*
* Global variables:
*  VDAC8_ESP_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void VDAC8_ESP_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(VDAC8_ESP_initVar == 0u)
    { 
        VDAC8_ESP_Init();
        VDAC8_ESP_initVar = 1u;
    }

    /* Enable power to DAC */
    VDAC8_ESP_Enable();

    /* Set default value */
    VDAC8_ESP_SetValue(VDAC8_ESP_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: VDAC8_ESP_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC8_ESP_Stop(void) 
{
    /* Disble power to DAC */
    VDAC8_ESP_PWRMGR &= (uint8)(~VDAC8_ESP_ACT_PWR_EN);
    VDAC8_ESP_STBY_PWRMGR &= (uint8)(~VDAC8_ESP_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        VDAC8_ESP_backup.data_value = VDAC8_ESP_CR0;
        VDAC8_ESP_CR0 = VDAC8_ESP_CUR_MODE_OUT_OFF;
        VDAC8_ESP_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VDAC8_ESP_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC8_ESP_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    VDAC8_ESP_CR0 &= (uint8)(~VDAC8_ESP_HS_MASK);
    VDAC8_ESP_CR0 |=  (speed & VDAC8_ESP_HS_MASK);
}


/*******************************************************************************
* Function Name: VDAC8_ESP_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:
*  Range: Sets one of Three valid ranges.
*
* Return:
*  void 
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC8_ESP_SetRange(uint8 range) 
{
    VDAC8_ESP_CR0 &= (uint8)(~VDAC8_ESP_RANGE_MASK);      /* Clear existing mode */
    VDAC8_ESP_CR0 |= (range & VDAC8_ESP_RANGE_MASK);      /*  Set Range  */
    VDAC8_ESP_DacTrim();
}


/*******************************************************************************
* Function Name: VDAC8_ESP_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  void 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void VDAC8_ESP_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 VDAC8_ESP_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    VDAC8_ESP_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        VDAC8_ESP_Data = value;
        CyExitCriticalSection(VDAC8_ESP_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VDAC8_ESP_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  range:  1V or 4V range.  See constants.
*
* Return:
*  void
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void VDAC8_ESP_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((VDAC8_ESP_CR0 & VDAC8_ESP_RANGE_MASK) >> 2) + VDAC8_ESP_TRIM_M7_1V_RNG_OFFSET;
    VDAC8_ESP_TR = CY_GET_XTND_REG8((uint8 *)(VDAC8_ESP_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */

/*******************************************************************************
* File Name: GSM_DTR.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_GSM_DTR_H) /* Pins GSM_DTR_H */
#define CY_PINS_GSM_DTR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "GSM_DTR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 GSM_DTR__PORT == 15 && ((GSM_DTR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    GSM_DTR_Write(uint8 value);
void    GSM_DTR_SetDriveMode(uint8 mode);
uint8   GSM_DTR_ReadDataReg(void);
uint8   GSM_DTR_Read(void);
void    GSM_DTR_SetInterruptMode(uint16 position, uint16 mode);
uint8   GSM_DTR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the GSM_DTR_SetDriveMode() function.
     *  @{
     */
        #define GSM_DTR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define GSM_DTR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define GSM_DTR_DM_RES_UP          PIN_DM_RES_UP
        #define GSM_DTR_DM_RES_DWN         PIN_DM_RES_DWN
        #define GSM_DTR_DM_OD_LO           PIN_DM_OD_LO
        #define GSM_DTR_DM_OD_HI           PIN_DM_OD_HI
        #define GSM_DTR_DM_STRONG          PIN_DM_STRONG
        #define GSM_DTR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define GSM_DTR_MASK               GSM_DTR__MASK
#define GSM_DTR_SHIFT              GSM_DTR__SHIFT
#define GSM_DTR_WIDTH              1u

/* Interrupt constants */
#if defined(GSM_DTR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in GSM_DTR_SetInterruptMode() function.
     *  @{
     */
        #define GSM_DTR_INTR_NONE      (uint16)(0x0000u)
        #define GSM_DTR_INTR_RISING    (uint16)(0x0001u)
        #define GSM_DTR_INTR_FALLING   (uint16)(0x0002u)
        #define GSM_DTR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define GSM_DTR_INTR_MASK      (0x01u) 
#endif /* (GSM_DTR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define GSM_DTR_PS                     (* (reg8 *) GSM_DTR__PS)
/* Data Register */
#define GSM_DTR_DR                     (* (reg8 *) GSM_DTR__DR)
/* Port Number */
#define GSM_DTR_PRT_NUM                (* (reg8 *) GSM_DTR__PRT) 
/* Connect to Analog Globals */                                                  
#define GSM_DTR_AG                     (* (reg8 *) GSM_DTR__AG)                       
/* Analog MUX bux enable */
#define GSM_DTR_AMUX                   (* (reg8 *) GSM_DTR__AMUX) 
/* Bidirectional Enable */                                                        
#define GSM_DTR_BIE                    (* (reg8 *) GSM_DTR__BIE)
/* Bit-mask for Aliased Register Access */
#define GSM_DTR_BIT_MASK               (* (reg8 *) GSM_DTR__BIT_MASK)
/* Bypass Enable */
#define GSM_DTR_BYP                    (* (reg8 *) GSM_DTR__BYP)
/* Port wide control signals */                                                   
#define GSM_DTR_CTL                    (* (reg8 *) GSM_DTR__CTL)
/* Drive Modes */
#define GSM_DTR_DM0                    (* (reg8 *) GSM_DTR__DM0) 
#define GSM_DTR_DM1                    (* (reg8 *) GSM_DTR__DM1)
#define GSM_DTR_DM2                    (* (reg8 *) GSM_DTR__DM2) 
/* Input Buffer Disable Override */
#define GSM_DTR_INP_DIS                (* (reg8 *) GSM_DTR__INP_DIS)
/* LCD Common or Segment Drive */
#define GSM_DTR_LCD_COM_SEG            (* (reg8 *) GSM_DTR__LCD_COM_SEG)
/* Enable Segment LCD */
#define GSM_DTR_LCD_EN                 (* (reg8 *) GSM_DTR__LCD_EN)
/* Slew Rate Control */
#define GSM_DTR_SLW                    (* (reg8 *) GSM_DTR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define GSM_DTR_PRTDSI__CAPS_SEL       (* (reg8 *) GSM_DTR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define GSM_DTR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) GSM_DTR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define GSM_DTR_PRTDSI__OE_SEL0        (* (reg8 *) GSM_DTR__PRTDSI__OE_SEL0) 
#define GSM_DTR_PRTDSI__OE_SEL1        (* (reg8 *) GSM_DTR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define GSM_DTR_PRTDSI__OUT_SEL0       (* (reg8 *) GSM_DTR__PRTDSI__OUT_SEL0) 
#define GSM_DTR_PRTDSI__OUT_SEL1       (* (reg8 *) GSM_DTR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define GSM_DTR_PRTDSI__SYNC_OUT       (* (reg8 *) GSM_DTR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(GSM_DTR__SIO_CFG)
    #define GSM_DTR_SIO_HYST_EN        (* (reg8 *) GSM_DTR__SIO_HYST_EN)
    #define GSM_DTR_SIO_REG_HIFREQ     (* (reg8 *) GSM_DTR__SIO_REG_HIFREQ)
    #define GSM_DTR_SIO_CFG            (* (reg8 *) GSM_DTR__SIO_CFG)
    #define GSM_DTR_SIO_DIFF           (* (reg8 *) GSM_DTR__SIO_DIFF)
#endif /* (GSM_DTR__SIO_CFG) */

/* Interrupt Registers */
#if defined(GSM_DTR__INTSTAT)
    #define GSM_DTR_INTSTAT            (* (reg8 *) GSM_DTR__INTSTAT)
    #define GSM_DTR_SNAP               (* (reg8 *) GSM_DTR__SNAP)
    
	#define GSM_DTR_0_INTTYPE_REG 		(* (reg8 *) GSM_DTR__0__INTTYPE)
#endif /* (GSM_DTR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_GSM_DTR_H */


/* [] END OF FILE */

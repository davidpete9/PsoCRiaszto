/*******************************************************************************
* File Name: BACKLIGHT.h  
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

#if !defined(CY_PINS_BACKLIGHT_H) /* Pins BACKLIGHT_H */
#define CY_PINS_BACKLIGHT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BACKLIGHT_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BACKLIGHT__PORT == 15 && ((BACKLIGHT__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BACKLIGHT_Write(uint8 value);
void    BACKLIGHT_SetDriveMode(uint8 mode);
uint8   BACKLIGHT_ReadDataReg(void);
uint8   BACKLIGHT_Read(void);
void    BACKLIGHT_SetInterruptMode(uint16 position, uint16 mode);
uint8   BACKLIGHT_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BACKLIGHT_SetDriveMode() function.
     *  @{
     */
        #define BACKLIGHT_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BACKLIGHT_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BACKLIGHT_DM_RES_UP          PIN_DM_RES_UP
        #define BACKLIGHT_DM_RES_DWN         PIN_DM_RES_DWN
        #define BACKLIGHT_DM_OD_LO           PIN_DM_OD_LO
        #define BACKLIGHT_DM_OD_HI           PIN_DM_OD_HI
        #define BACKLIGHT_DM_STRONG          PIN_DM_STRONG
        #define BACKLIGHT_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BACKLIGHT_MASK               BACKLIGHT__MASK
#define BACKLIGHT_SHIFT              BACKLIGHT__SHIFT
#define BACKLIGHT_WIDTH              1u

/* Interrupt constants */
#if defined(BACKLIGHT__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BACKLIGHT_SetInterruptMode() function.
     *  @{
     */
        #define BACKLIGHT_INTR_NONE      (uint16)(0x0000u)
        #define BACKLIGHT_INTR_RISING    (uint16)(0x0001u)
        #define BACKLIGHT_INTR_FALLING   (uint16)(0x0002u)
        #define BACKLIGHT_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BACKLIGHT_INTR_MASK      (0x01u) 
#endif /* (BACKLIGHT__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BACKLIGHT_PS                     (* (reg8 *) BACKLIGHT__PS)
/* Data Register */
#define BACKLIGHT_DR                     (* (reg8 *) BACKLIGHT__DR)
/* Port Number */
#define BACKLIGHT_PRT_NUM                (* (reg8 *) BACKLIGHT__PRT) 
/* Connect to Analog Globals */                                                  
#define BACKLIGHT_AG                     (* (reg8 *) BACKLIGHT__AG)                       
/* Analog MUX bux enable */
#define BACKLIGHT_AMUX                   (* (reg8 *) BACKLIGHT__AMUX) 
/* Bidirectional Enable */                                                        
#define BACKLIGHT_BIE                    (* (reg8 *) BACKLIGHT__BIE)
/* Bit-mask for Aliased Register Access */
#define BACKLIGHT_BIT_MASK               (* (reg8 *) BACKLIGHT__BIT_MASK)
/* Bypass Enable */
#define BACKLIGHT_BYP                    (* (reg8 *) BACKLIGHT__BYP)
/* Port wide control signals */                                                   
#define BACKLIGHT_CTL                    (* (reg8 *) BACKLIGHT__CTL)
/* Drive Modes */
#define BACKLIGHT_DM0                    (* (reg8 *) BACKLIGHT__DM0) 
#define BACKLIGHT_DM1                    (* (reg8 *) BACKLIGHT__DM1)
#define BACKLIGHT_DM2                    (* (reg8 *) BACKLIGHT__DM2) 
/* Input Buffer Disable Override */
#define BACKLIGHT_INP_DIS                (* (reg8 *) BACKLIGHT__INP_DIS)
/* LCD Common or Segment Drive */
#define BACKLIGHT_LCD_COM_SEG            (* (reg8 *) BACKLIGHT__LCD_COM_SEG)
/* Enable Segment LCD */
#define BACKLIGHT_LCD_EN                 (* (reg8 *) BACKLIGHT__LCD_EN)
/* Slew Rate Control */
#define BACKLIGHT_SLW                    (* (reg8 *) BACKLIGHT__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BACKLIGHT_PRTDSI__CAPS_SEL       (* (reg8 *) BACKLIGHT__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BACKLIGHT_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BACKLIGHT__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BACKLIGHT_PRTDSI__OE_SEL0        (* (reg8 *) BACKLIGHT__PRTDSI__OE_SEL0) 
#define BACKLIGHT_PRTDSI__OE_SEL1        (* (reg8 *) BACKLIGHT__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BACKLIGHT_PRTDSI__OUT_SEL0       (* (reg8 *) BACKLIGHT__PRTDSI__OUT_SEL0) 
#define BACKLIGHT_PRTDSI__OUT_SEL1       (* (reg8 *) BACKLIGHT__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BACKLIGHT_PRTDSI__SYNC_OUT       (* (reg8 *) BACKLIGHT__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BACKLIGHT__SIO_CFG)
    #define BACKLIGHT_SIO_HYST_EN        (* (reg8 *) BACKLIGHT__SIO_HYST_EN)
    #define BACKLIGHT_SIO_REG_HIFREQ     (* (reg8 *) BACKLIGHT__SIO_REG_HIFREQ)
    #define BACKLIGHT_SIO_CFG            (* (reg8 *) BACKLIGHT__SIO_CFG)
    #define BACKLIGHT_SIO_DIFF           (* (reg8 *) BACKLIGHT__SIO_DIFF)
#endif /* (BACKLIGHT__SIO_CFG) */

/* Interrupt Registers */
#if defined(BACKLIGHT__INTSTAT)
    #define BACKLIGHT_INTSTAT            (* (reg8 *) BACKLIGHT__INTSTAT)
    #define BACKLIGHT_SNAP               (* (reg8 *) BACKLIGHT__SNAP)
    
	#define BACKLIGHT_0_INTTYPE_REG 		(* (reg8 *) BACKLIGHT__0__INTTYPE)
#endif /* (BACKLIGHT__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BACKLIGHT_H */


/* [] END OF FILE */

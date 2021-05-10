/*******************************************************************************
* File Name: REED_RELAY.h  
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

#if !defined(CY_PINS_REED_RELAY_H) /* Pins REED_RELAY_H */
#define CY_PINS_REED_RELAY_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "REED_RELAY_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 REED_RELAY__PORT == 15 && ((REED_RELAY__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    REED_RELAY_Write(uint8 value);
void    REED_RELAY_SetDriveMode(uint8 mode);
uint8   REED_RELAY_ReadDataReg(void);
uint8   REED_RELAY_Read(void);
void    REED_RELAY_SetInterruptMode(uint16 position, uint16 mode);
uint8   REED_RELAY_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the REED_RELAY_SetDriveMode() function.
     *  @{
     */
        #define REED_RELAY_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define REED_RELAY_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define REED_RELAY_DM_RES_UP          PIN_DM_RES_UP
        #define REED_RELAY_DM_RES_DWN         PIN_DM_RES_DWN
        #define REED_RELAY_DM_OD_LO           PIN_DM_OD_LO
        #define REED_RELAY_DM_OD_HI           PIN_DM_OD_HI
        #define REED_RELAY_DM_STRONG          PIN_DM_STRONG
        #define REED_RELAY_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define REED_RELAY_MASK               REED_RELAY__MASK
#define REED_RELAY_SHIFT              REED_RELAY__SHIFT
#define REED_RELAY_WIDTH              1u

/* Interrupt constants */
#if defined(REED_RELAY__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in REED_RELAY_SetInterruptMode() function.
     *  @{
     */
        #define REED_RELAY_INTR_NONE      (uint16)(0x0000u)
        #define REED_RELAY_INTR_RISING    (uint16)(0x0001u)
        #define REED_RELAY_INTR_FALLING   (uint16)(0x0002u)
        #define REED_RELAY_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define REED_RELAY_INTR_MASK      (0x01u) 
#endif /* (REED_RELAY__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define REED_RELAY_PS                     (* (reg8 *) REED_RELAY__PS)
/* Data Register */
#define REED_RELAY_DR                     (* (reg8 *) REED_RELAY__DR)
/* Port Number */
#define REED_RELAY_PRT_NUM                (* (reg8 *) REED_RELAY__PRT) 
/* Connect to Analog Globals */                                                  
#define REED_RELAY_AG                     (* (reg8 *) REED_RELAY__AG)                       
/* Analog MUX bux enable */
#define REED_RELAY_AMUX                   (* (reg8 *) REED_RELAY__AMUX) 
/* Bidirectional Enable */                                                        
#define REED_RELAY_BIE                    (* (reg8 *) REED_RELAY__BIE)
/* Bit-mask for Aliased Register Access */
#define REED_RELAY_BIT_MASK               (* (reg8 *) REED_RELAY__BIT_MASK)
/* Bypass Enable */
#define REED_RELAY_BYP                    (* (reg8 *) REED_RELAY__BYP)
/* Port wide control signals */                                                   
#define REED_RELAY_CTL                    (* (reg8 *) REED_RELAY__CTL)
/* Drive Modes */
#define REED_RELAY_DM0                    (* (reg8 *) REED_RELAY__DM0) 
#define REED_RELAY_DM1                    (* (reg8 *) REED_RELAY__DM1)
#define REED_RELAY_DM2                    (* (reg8 *) REED_RELAY__DM2) 
/* Input Buffer Disable Override */
#define REED_RELAY_INP_DIS                (* (reg8 *) REED_RELAY__INP_DIS)
/* LCD Common or Segment Drive */
#define REED_RELAY_LCD_COM_SEG            (* (reg8 *) REED_RELAY__LCD_COM_SEG)
/* Enable Segment LCD */
#define REED_RELAY_LCD_EN                 (* (reg8 *) REED_RELAY__LCD_EN)
/* Slew Rate Control */
#define REED_RELAY_SLW                    (* (reg8 *) REED_RELAY__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define REED_RELAY_PRTDSI__CAPS_SEL       (* (reg8 *) REED_RELAY__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define REED_RELAY_PRTDSI__DBL_SYNC_IN    (* (reg8 *) REED_RELAY__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define REED_RELAY_PRTDSI__OE_SEL0        (* (reg8 *) REED_RELAY__PRTDSI__OE_SEL0) 
#define REED_RELAY_PRTDSI__OE_SEL1        (* (reg8 *) REED_RELAY__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define REED_RELAY_PRTDSI__OUT_SEL0       (* (reg8 *) REED_RELAY__PRTDSI__OUT_SEL0) 
#define REED_RELAY_PRTDSI__OUT_SEL1       (* (reg8 *) REED_RELAY__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define REED_RELAY_PRTDSI__SYNC_OUT       (* (reg8 *) REED_RELAY__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(REED_RELAY__SIO_CFG)
    #define REED_RELAY_SIO_HYST_EN        (* (reg8 *) REED_RELAY__SIO_HYST_EN)
    #define REED_RELAY_SIO_REG_HIFREQ     (* (reg8 *) REED_RELAY__SIO_REG_HIFREQ)
    #define REED_RELAY_SIO_CFG            (* (reg8 *) REED_RELAY__SIO_CFG)
    #define REED_RELAY_SIO_DIFF           (* (reg8 *) REED_RELAY__SIO_DIFF)
#endif /* (REED_RELAY__SIO_CFG) */

/* Interrupt Registers */
#if defined(REED_RELAY__INTSTAT)
    #define REED_RELAY_INTSTAT            (* (reg8 *) REED_RELAY__INTSTAT)
    #define REED_RELAY_SNAP               (* (reg8 *) REED_RELAY__SNAP)
    
	#define REED_RELAY_0_INTTYPE_REG 		(* (reg8 *) REED_RELAY__0__INTTYPE)
#endif /* (REED_RELAY__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_REED_RELAY_H */


/* [] END OF FILE */

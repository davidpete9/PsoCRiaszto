/*******************************************************************************
* File Name: COL4.h  
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

#if !defined(CY_PINS_COL4_H) /* Pins COL4_H */
#define CY_PINS_COL4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "COL4_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 COL4__PORT == 15 && ((COL4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    COL4_Write(uint8 value);
void    COL4_SetDriveMode(uint8 mode);
uint8   COL4_ReadDataReg(void);
uint8   COL4_Read(void);
void    COL4_SetInterruptMode(uint16 position, uint16 mode);
uint8   COL4_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the COL4_SetDriveMode() function.
     *  @{
     */
        #define COL4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define COL4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define COL4_DM_RES_UP          PIN_DM_RES_UP
        #define COL4_DM_RES_DWN         PIN_DM_RES_DWN
        #define COL4_DM_OD_LO           PIN_DM_OD_LO
        #define COL4_DM_OD_HI           PIN_DM_OD_HI
        #define COL4_DM_STRONG          PIN_DM_STRONG
        #define COL4_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define COL4_MASK               COL4__MASK
#define COL4_SHIFT              COL4__SHIFT
#define COL4_WIDTH              1u

/* Interrupt constants */
#if defined(COL4__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in COL4_SetInterruptMode() function.
     *  @{
     */
        #define COL4_INTR_NONE      (uint16)(0x0000u)
        #define COL4_INTR_RISING    (uint16)(0x0001u)
        #define COL4_INTR_FALLING   (uint16)(0x0002u)
        #define COL4_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define COL4_INTR_MASK      (0x01u) 
#endif /* (COL4__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define COL4_PS                     (* (reg8 *) COL4__PS)
/* Data Register */
#define COL4_DR                     (* (reg8 *) COL4__DR)
/* Port Number */
#define COL4_PRT_NUM                (* (reg8 *) COL4__PRT) 
/* Connect to Analog Globals */                                                  
#define COL4_AG                     (* (reg8 *) COL4__AG)                       
/* Analog MUX bux enable */
#define COL4_AMUX                   (* (reg8 *) COL4__AMUX) 
/* Bidirectional Enable */                                                        
#define COL4_BIE                    (* (reg8 *) COL4__BIE)
/* Bit-mask for Aliased Register Access */
#define COL4_BIT_MASK               (* (reg8 *) COL4__BIT_MASK)
/* Bypass Enable */
#define COL4_BYP                    (* (reg8 *) COL4__BYP)
/* Port wide control signals */                                                   
#define COL4_CTL                    (* (reg8 *) COL4__CTL)
/* Drive Modes */
#define COL4_DM0                    (* (reg8 *) COL4__DM0) 
#define COL4_DM1                    (* (reg8 *) COL4__DM1)
#define COL4_DM2                    (* (reg8 *) COL4__DM2) 
/* Input Buffer Disable Override */
#define COL4_INP_DIS                (* (reg8 *) COL4__INP_DIS)
/* LCD Common or Segment Drive */
#define COL4_LCD_COM_SEG            (* (reg8 *) COL4__LCD_COM_SEG)
/* Enable Segment LCD */
#define COL4_LCD_EN                 (* (reg8 *) COL4__LCD_EN)
/* Slew Rate Control */
#define COL4_SLW                    (* (reg8 *) COL4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define COL4_PRTDSI__CAPS_SEL       (* (reg8 *) COL4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define COL4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) COL4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define COL4_PRTDSI__OE_SEL0        (* (reg8 *) COL4__PRTDSI__OE_SEL0) 
#define COL4_PRTDSI__OE_SEL1        (* (reg8 *) COL4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define COL4_PRTDSI__OUT_SEL0       (* (reg8 *) COL4__PRTDSI__OUT_SEL0) 
#define COL4_PRTDSI__OUT_SEL1       (* (reg8 *) COL4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define COL4_PRTDSI__SYNC_OUT       (* (reg8 *) COL4__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(COL4__SIO_CFG)
    #define COL4_SIO_HYST_EN        (* (reg8 *) COL4__SIO_HYST_EN)
    #define COL4_SIO_REG_HIFREQ     (* (reg8 *) COL4__SIO_REG_HIFREQ)
    #define COL4_SIO_CFG            (* (reg8 *) COL4__SIO_CFG)
    #define COL4_SIO_DIFF           (* (reg8 *) COL4__SIO_DIFF)
#endif /* (COL4__SIO_CFG) */

/* Interrupt Registers */
#if defined(COL4__INTSTAT)
    #define COL4_INTSTAT            (* (reg8 *) COL4__INTSTAT)
    #define COL4_SNAP               (* (reg8 *) COL4__SNAP)
    
	#define COL4_0_INTTYPE_REG 		(* (reg8 *) COL4__0__INTTYPE)
#endif /* (COL4__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_COL4_H */


/* [] END OF FILE */

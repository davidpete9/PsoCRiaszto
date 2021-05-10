/*******************************************************************************
* File Name: RADAR.h  
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

#if !defined(CY_PINS_RADAR_H) /* Pins RADAR_H */
#define CY_PINS_RADAR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RADAR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RADAR__PORT == 15 && ((RADAR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    RADAR_Write(uint8 value);
void    RADAR_SetDriveMode(uint8 mode);
uint8   RADAR_ReadDataReg(void);
uint8   RADAR_Read(void);
void    RADAR_SetInterruptMode(uint16 position, uint16 mode);
uint8   RADAR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the RADAR_SetDriveMode() function.
     *  @{
     */
        #define RADAR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define RADAR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define RADAR_DM_RES_UP          PIN_DM_RES_UP
        #define RADAR_DM_RES_DWN         PIN_DM_RES_DWN
        #define RADAR_DM_OD_LO           PIN_DM_OD_LO
        #define RADAR_DM_OD_HI           PIN_DM_OD_HI
        #define RADAR_DM_STRONG          PIN_DM_STRONG
        #define RADAR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define RADAR_MASK               RADAR__MASK
#define RADAR_SHIFT              RADAR__SHIFT
#define RADAR_WIDTH              1u

/* Interrupt constants */
#if defined(RADAR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RADAR_SetInterruptMode() function.
     *  @{
     */
        #define RADAR_INTR_NONE      (uint16)(0x0000u)
        #define RADAR_INTR_RISING    (uint16)(0x0001u)
        #define RADAR_INTR_FALLING   (uint16)(0x0002u)
        #define RADAR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define RADAR_INTR_MASK      (0x01u) 
#endif /* (RADAR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RADAR_PS                     (* (reg8 *) RADAR__PS)
/* Data Register */
#define RADAR_DR                     (* (reg8 *) RADAR__DR)
/* Port Number */
#define RADAR_PRT_NUM                (* (reg8 *) RADAR__PRT) 
/* Connect to Analog Globals */                                                  
#define RADAR_AG                     (* (reg8 *) RADAR__AG)                       
/* Analog MUX bux enable */
#define RADAR_AMUX                   (* (reg8 *) RADAR__AMUX) 
/* Bidirectional Enable */                                                        
#define RADAR_BIE                    (* (reg8 *) RADAR__BIE)
/* Bit-mask for Aliased Register Access */
#define RADAR_BIT_MASK               (* (reg8 *) RADAR__BIT_MASK)
/* Bypass Enable */
#define RADAR_BYP                    (* (reg8 *) RADAR__BYP)
/* Port wide control signals */                                                   
#define RADAR_CTL                    (* (reg8 *) RADAR__CTL)
/* Drive Modes */
#define RADAR_DM0                    (* (reg8 *) RADAR__DM0) 
#define RADAR_DM1                    (* (reg8 *) RADAR__DM1)
#define RADAR_DM2                    (* (reg8 *) RADAR__DM2) 
/* Input Buffer Disable Override */
#define RADAR_INP_DIS                (* (reg8 *) RADAR__INP_DIS)
/* LCD Common or Segment Drive */
#define RADAR_LCD_COM_SEG            (* (reg8 *) RADAR__LCD_COM_SEG)
/* Enable Segment LCD */
#define RADAR_LCD_EN                 (* (reg8 *) RADAR__LCD_EN)
/* Slew Rate Control */
#define RADAR_SLW                    (* (reg8 *) RADAR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RADAR_PRTDSI__CAPS_SEL       (* (reg8 *) RADAR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RADAR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RADAR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RADAR_PRTDSI__OE_SEL0        (* (reg8 *) RADAR__PRTDSI__OE_SEL0) 
#define RADAR_PRTDSI__OE_SEL1        (* (reg8 *) RADAR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RADAR_PRTDSI__OUT_SEL0       (* (reg8 *) RADAR__PRTDSI__OUT_SEL0) 
#define RADAR_PRTDSI__OUT_SEL1       (* (reg8 *) RADAR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RADAR_PRTDSI__SYNC_OUT       (* (reg8 *) RADAR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(RADAR__SIO_CFG)
    #define RADAR_SIO_HYST_EN        (* (reg8 *) RADAR__SIO_HYST_EN)
    #define RADAR_SIO_REG_HIFREQ     (* (reg8 *) RADAR__SIO_REG_HIFREQ)
    #define RADAR_SIO_CFG            (* (reg8 *) RADAR__SIO_CFG)
    #define RADAR_SIO_DIFF           (* (reg8 *) RADAR__SIO_DIFF)
#endif /* (RADAR__SIO_CFG) */

/* Interrupt Registers */
#if defined(RADAR__INTSTAT)
    #define RADAR_INTSTAT            (* (reg8 *) RADAR__INTSTAT)
    #define RADAR_SNAP               (* (reg8 *) RADAR__SNAP)
    
	#define RADAR_0_INTTYPE_REG 		(* (reg8 *) RADAR__0__INTTYPE)
#endif /* (RADAR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RADAR_H */


/* [] END OF FILE */

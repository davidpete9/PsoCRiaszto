/*******************************************************************************
* File Name: PHRES.h  
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

#if !defined(CY_PINS_PHRES_H) /* Pins PHRES_H */
#define CY_PINS_PHRES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PHRES_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PHRES__PORT == 15 && ((PHRES__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    PHRES_Write(uint8 value);
void    PHRES_SetDriveMode(uint8 mode);
uint8   PHRES_ReadDataReg(void);
uint8   PHRES_Read(void);
void    PHRES_SetInterruptMode(uint16 position, uint16 mode);
uint8   PHRES_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the PHRES_SetDriveMode() function.
     *  @{
     */
        #define PHRES_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define PHRES_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define PHRES_DM_RES_UP          PIN_DM_RES_UP
        #define PHRES_DM_RES_DWN         PIN_DM_RES_DWN
        #define PHRES_DM_OD_LO           PIN_DM_OD_LO
        #define PHRES_DM_OD_HI           PIN_DM_OD_HI
        #define PHRES_DM_STRONG          PIN_DM_STRONG
        #define PHRES_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define PHRES_MASK               PHRES__MASK
#define PHRES_SHIFT              PHRES__SHIFT
#define PHRES_WIDTH              1u

/* Interrupt constants */
#if defined(PHRES__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in PHRES_SetInterruptMode() function.
     *  @{
     */
        #define PHRES_INTR_NONE      (uint16)(0x0000u)
        #define PHRES_INTR_RISING    (uint16)(0x0001u)
        #define PHRES_INTR_FALLING   (uint16)(0x0002u)
        #define PHRES_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define PHRES_INTR_MASK      (0x01u) 
#endif /* (PHRES__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PHRES_PS                     (* (reg8 *) PHRES__PS)
/* Data Register */
#define PHRES_DR                     (* (reg8 *) PHRES__DR)
/* Port Number */
#define PHRES_PRT_NUM                (* (reg8 *) PHRES__PRT) 
/* Connect to Analog Globals */                                                  
#define PHRES_AG                     (* (reg8 *) PHRES__AG)                       
/* Analog MUX bux enable */
#define PHRES_AMUX                   (* (reg8 *) PHRES__AMUX) 
/* Bidirectional Enable */                                                        
#define PHRES_BIE                    (* (reg8 *) PHRES__BIE)
/* Bit-mask for Aliased Register Access */
#define PHRES_BIT_MASK               (* (reg8 *) PHRES__BIT_MASK)
/* Bypass Enable */
#define PHRES_BYP                    (* (reg8 *) PHRES__BYP)
/* Port wide control signals */                                                   
#define PHRES_CTL                    (* (reg8 *) PHRES__CTL)
/* Drive Modes */
#define PHRES_DM0                    (* (reg8 *) PHRES__DM0) 
#define PHRES_DM1                    (* (reg8 *) PHRES__DM1)
#define PHRES_DM2                    (* (reg8 *) PHRES__DM2) 
/* Input Buffer Disable Override */
#define PHRES_INP_DIS                (* (reg8 *) PHRES__INP_DIS)
/* LCD Common or Segment Drive */
#define PHRES_LCD_COM_SEG            (* (reg8 *) PHRES__LCD_COM_SEG)
/* Enable Segment LCD */
#define PHRES_LCD_EN                 (* (reg8 *) PHRES__LCD_EN)
/* Slew Rate Control */
#define PHRES_SLW                    (* (reg8 *) PHRES__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PHRES_PRTDSI__CAPS_SEL       (* (reg8 *) PHRES__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PHRES_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PHRES__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PHRES_PRTDSI__OE_SEL0        (* (reg8 *) PHRES__PRTDSI__OE_SEL0) 
#define PHRES_PRTDSI__OE_SEL1        (* (reg8 *) PHRES__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PHRES_PRTDSI__OUT_SEL0       (* (reg8 *) PHRES__PRTDSI__OUT_SEL0) 
#define PHRES_PRTDSI__OUT_SEL1       (* (reg8 *) PHRES__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PHRES_PRTDSI__SYNC_OUT       (* (reg8 *) PHRES__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(PHRES__SIO_CFG)
    #define PHRES_SIO_HYST_EN        (* (reg8 *) PHRES__SIO_HYST_EN)
    #define PHRES_SIO_REG_HIFREQ     (* (reg8 *) PHRES__SIO_REG_HIFREQ)
    #define PHRES_SIO_CFG            (* (reg8 *) PHRES__SIO_CFG)
    #define PHRES_SIO_DIFF           (* (reg8 *) PHRES__SIO_DIFF)
#endif /* (PHRES__SIO_CFG) */

/* Interrupt Registers */
#if defined(PHRES__INTSTAT)
    #define PHRES_INTSTAT            (* (reg8 *) PHRES__INTSTAT)
    #define PHRES_SNAP               (* (reg8 *) PHRES__SNAP)
    
	#define PHRES_0_INTTYPE_REG 		(* (reg8 *) PHRES__0__INTTYPE)
#endif /* (PHRES__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PHRES_H */


/* [] END OF FILE */

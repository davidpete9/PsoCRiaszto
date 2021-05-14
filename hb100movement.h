/* ========================================
 *
 * A mozgaserzekelo interruptja, es jelzo fuggvenye, ami flagat allit, amennyiben riasztas van.
 *
 *
 * ========================================
*/

#ifndef HB100MOVEMENT_H
#define HB100MOVEMENT_H

#include "project.h"

/*
A radar AD konverterenek az interruptja.
Folyamatos min-max kereses ahhoz, hogy metudjuk az amplitudot.
*/
CY_ISR(ADC_SAR_RADAR_ISR);

/*
Periodikusan fut le, itt ellenorizem a min-max szamolast.
Ha az amplitudo nagyobb egy  erteknel, akkor riasztas van.
*/
void check_movement(uint8 * is_movement_alert);

void reset_radar_min_max_values();

#endif

/* [] END OF FILE */

/* ========================================
 *
 * A mozgaserzekelo interruptja, es jelzo fuggvenye, ami flagat allit, amennyiben riasztas van.
 *
 *
 * ========================================
*/

#include "project.h"

uint16 minAmp = 30000;
uint16 maxAmp = 0;
uint32 lastCountRadar = 0;

/*
A radar AD konverterenek az interruptja.
Folyamatos min-max kereses ahhoz, hogy metudjuk az amplitudot.
*/
CY_ISR(ADC_SAR_RADAR_ISR ) {
 
    volatile uint16 AD_result = ADC_SAR_RADAR_GetResult16();

    if (AD_result < minAmp) {
        minAmp = AD_result;
    }  
    if (AD_result > maxAmp) {
        maxAmp = AD_result;
    }
}

/*
Periodikusan fut le, itt ellenorizem a min-max szamolast.
Ha az amplitudo nagyobb egy  erteknel, akkor riasztas van.
*/
void check_movement(uint8 * is_movement_alert) {
    volatile int16 amp = maxAmp-minAmp;
    //char str[10];
    //sprintf(str, "%d\r\n", amp);
    //UART_PutString(str);
    if (amp > 3000) {
        //Mozgas erzekelve.
        *is_movement_alert = 1u;
        //TODO: csak akkor riasszon be, ha nem csak pl. egyetlen ilyen eszleles volt, hanem tobb is
        //mivel van esely ra, hogy az csak egy valahonnan egy felszedett zavar.
        }
        maxAmp = 0;
        minAmp = 30000;
}

void reset_radar_min_max_values() {
        maxAmp = 0;
        minAmp = 30000;
}




/* [] END OF FILE */

/* ========================================
 *
 * A mozgaserzekelo interruptja, es jelzo fuggvenye, ami flagat allit, amennyiben riasztas van.
 *
 *
 * ========================================
*/

#include "project.h"

/*
Uj radar modullal csak ennyi
*/
void check_movement(uint8 * is_movement_alert) {
    if (RADAR_Read() == 0u) {
        *is_movement_alert = 0u;
    }
    else {
        *is_movement_alert = 1u;
    }
}




/* [] END OF FILE */

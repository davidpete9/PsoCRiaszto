/* ========================================
 *
 * 4x4-es matrix billenyuzet kezelese
 *
 *
 * ========================================
*/

#ifndef KEYPAD_H
#define KEYPAD_H

#include "project.h"


//Visszaadja a legutobb lenyomott gomb erteket
uint8 get_pressed_key();

/* 
Megnezem, hogy van e lenyomott billentyu, ha nincs 0-s erteket ad,
ha van akkor a billenyu ASCII kodjaval. 
Gomb felengedesekor menek csak! .*/
void readKey();

#endif

/* [] END OF FILE */

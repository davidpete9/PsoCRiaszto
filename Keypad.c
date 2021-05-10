/* ========================================
 *
 * 4x4-es matrix billenyuzet kezelese
 *
 *
 * ========================================
*/

#include "project.h"


uint8 waiting_for_relese = 0u;
uint8 PRESSED_KEY = 0;


//Visszaadja a legutobb lenyomott gomb erteket, es torli az erteket.
uint8 get_pressed_key() {
    uint8 temp = PRESSED_KEY;
    PRESSED_KEY = 0;
    return temp;    
}

/*
Csak akkor allitom be az erteket ujra, ha felengettem egy azota a billenyut.
Igy ha folyamatosan nyomva tartok egy gombot, akkor csak egye lenyomast vesz figyelembe
*/
void set_result_when_relesed(char pressed) {
    if (pressed != 0 && waiting_for_relese == 0u) {
        PRESSED_KEY = pressed;
        waiting_for_relese = 1u;
    }
    if (waiting_for_relese == 1u && pressed == 0)
        waiting_for_relese = 0u;
}


/* 
Megnezem, hogy van e lenyomott billentyu, ha nincs 0-s erteket ad,
ha van akkor a billenyu ASCII kodjaval. 
Gomb felengedesekor menek csak! .*/
void readKey() {
    
   
    //Feszultseget adok egyes sorokra, megnezem az oszlopokat.
    //Egyszerre csak egy gomb megnyomasat tamogatja a renszer.  (egyelore?) 
    
    char pressed = 0;
    
    ROW1_Write(1u);
    pressed = COL1_Read() != 0 ? '1' : pressed;
    pressed = COL2_Read() != 0 ? '2' : pressed;
    pressed = COL3_Read() != 0 ? '3' : pressed;
    pressed = COL4_Read() != 0 ? 'A' : pressed;
    
    //Egy valamit nyomva tartok
    if (pressed != 0) {
         ROW1_Write(0u);
         set_result_when_relesed(pressed);
         return;
    }
    
    ROW1_Write(0u);
    ROW2_Write(1u);
   
    pressed = COL1_Read() != 0 ? '4' : pressed;
    pressed = COL2_Read() != 0 ? '5' : pressed;
    pressed = COL3_Read() != 0 ? '6' : pressed;
    pressed = COL4_Read() != 0 ? 'B' : pressed;
    
    //Egy valamit nyomva tartok
    if (pressed != 0) {
         ROW2_Write(0u);
         set_result_when_relesed(pressed);
         return;
    }
    
    ROW2_Write(0u);
    ROW3_Write(1u);
   
    pressed = COL1_Read() != 0 ? '7' : pressed;
    pressed = COL2_Read() != 0 ? '8' : pressed;
    pressed = COL3_Read() != 0 ? '9' : pressed;
    pressed = COL4_Read() != 0 ? 'C' : pressed;
    
    //Egy valamit nyomva tartok
    if (pressed != 0) {
         ROW3_Write(0u);
         set_result_when_relesed(pressed);
         return;
    }
    
    ROW3_Write(0u);
    ROW4_Write(1u);
   
    pressed = COL1_Read() != 0 ? '*' : pressed;
    pressed = COL2_Read() != 0 ? '0' : pressed;
    pressed = COL3_Read() != 0 ? '#' : pressed;
    pressed = COL4_Read() != 0 ? 'D' : pressed;
    ROW4_Write(0u);   
    
    
    set_result_when_relesed(pressed);
}


/* [] END OF FILE */

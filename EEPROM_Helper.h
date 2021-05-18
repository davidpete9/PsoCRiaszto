/* ========================================
 *
 *  Csak kigyujtom az EEPROM cimeit a kulonbozo beallitasoknak
 * KEZDOCIM: 0000
 *
 * ========================================
*/

#ifndef EEPROM_HELPER_H
#define EEPROM_HELPER_H

#include "project.h"

//8 byton tarolhato adatok
#define NIGHT_FROM_ADDR 0 
#define NIGHT_UNITL_ADDR 1 
#define AUTO_WATCH_ACTIVATED_ADDR 2 //Automatikusan atvaltson figyelo modba az ejszaka folyaman?
#define USE_PHOTO_RES_ADDR 3 //Figyeljem a fotorezisztor figyelo modban?


//16 byton tarolhatok - 2 cimet foglal egy adat 
#define BEEP_LENGTH_ADDR 6 //6-7 Hany masodpercig csipogjon?
#define WATCH_DELAY_ADDR 8 //Ennyit varok az elesitesre. 
#define DEACT_DELAY_ADDR 10 //Ennyi idot hagyok riasztas utan hogy a kodot beirjak

//Telefonszamoknak, es a kodnak - mindegyik egy sor foglaljon el (1 sor 16*8 byte) 
//Kezdodjon a 32-es cimtol, ami a 3. sor kezdete
#define CODE_ADDR 32 //32-47
#define PHONE_NUM_1_ADDR 48 //48-63
#define PHONE_NUM_2_ADDR 64//64-79
#define PHONE_NUM_3_ADDR 80//80-95

void StoreInt8(uint8 data, uint16 addr);

uint8 ReadInt8(uint16 addr);

void StoreInt16(uint16 data, uint16 addr);

uint16 ReadInt16(uint16 addr);

/*
Kiolvas egymas utan n erteket egy sorbol
Egy sorban 16 db cim van.
*/
void ReadInt8ArrayFromRow(uint8 * values, uint8 n, uint16 addr);

/*
Egy sorba ir, a sor addr/16
*/
void StoreInt8ArrayToRow(uint8 * values, uint16 addr);

#endif

/* [] END OF FILE */

/* ========================================

 *
 * ========================================
*/


#include "project.h"


void StoreInt8(uint8 data, uint16 addr) {
    EEPROM_WriteByte(data, addr);
}

uint8 ReadInt8(uint16 addr) {
    return EEPROM_ReadByte(addr);
}

void StoreInt16(uint16 data, uint16 addr) {
    uint8 first_byte = (data&0xff00) >> 8;
    uint8 second_byte = data&0x00ff;
    EEPROM_WriteByte(first_byte, addr);
    EEPROM_WriteByte(second_byte, addr+1);
}

uint16 ReadInt16(uint16 addr) {
    uint8 first_byte = EEPROM_ReadByte(addr);
    uint8 second_byte = EEPROM_ReadByte(addr+1);
    uint16 result = (first_byte << 8)|second_byte;
    return result;
}


/*
Kiolvas egymas utan n erteket egy sorbol
Egy sorban 16 db cim van.
*/
void ReadInt8ArrayFromRow(uint8 * values, uint8 n, uint16 addr) {
   
    n = n > 16 ? 16 : n; //Egy sorban max. 16 elem lehet
    for (uint8 i = 0; i < n; i++) {
        values[i] = EEPROM_ReadByte(addr+i);
    }
}

void StoreInt8ArrayToRow(uint8 * values, uint16 addr) {
 
    uint8 row_number = addr / 16; //Az also egeszreszet veszi. Ennyiedik sor
    EEPROM_Write(values, row_number);  
    
}

//WriteRow





/* [] END OF FILE */

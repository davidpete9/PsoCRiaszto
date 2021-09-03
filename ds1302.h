/* ========================================
*
* Fuggvenykonyvtar a DS1302-es modul hasznalatahoz
* Felhasznalva: Oran tanult modszer a kommutnikaciohoz,
* DS1302 datasheet a cimzeshez: https://www.hestore.hu/prod_getfile.php?id=9509
* Le kell hogy legyen teve egy SPIM nevu SPI Bidirectional nevu SPI Master.
*/

#ifndef DS1302_H
#define DS1302_H

#include "project.h"

void SPIM_IO_RD_Addr(uint8 addr);

uint8 SPIM_IO_RD_Data();

int get_hour();

int get_minute();

int get_seconds();

int get_day();
int get_month();

int get_year();

void print_time_string(char* str);

void print_date(char* str);

void print_datetime(char* str);

#endif

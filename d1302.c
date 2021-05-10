/* ========================================
*
* Fuggvenykonyvtar a DS1302-es modul hasznalatahoz
* Felhasznalva: Oran tanult modszer a kommutnikaciohoz,
* DS1302 datasheet a cimzeshez: https://www.hestore.hu/prod_getfile.php?id=9509
* Le kell hogy legyen teve egy SPIM nevu SPI Bidirectional nevu SPI Master.
*/
#include "project.h"

void SPIM_IO_RD_Addr(uint8 addr) {
    volatile uint8 stat;
    SS_Write(1u);
    
    do {
        stat = SPIM_ReadTxStatus();
    }while(!(stat & SPIM_STS_TX_FIFO_NOT_FULL)); //van hely a fifo-ban
    
    SPIM_TxEnable(); //irok
    SPIM_WriteTxData(addr|0x01); // 1 ... 1
    do {
        stat = SPIM_ReadTxStatus();
    }while(!(stat & SPIM_STS_SPI_DONE)); // kesz a kiiras
    SPIM_TxDisable();
    SPIM_ClearFIFO();
    
}

uint8 SPIM_IO_RD_Data() {
    volatile uint8 stat, data;
    SS_Write(1u);
    SPIM_TxDisable();
    SPIM_WriteTxData(0x42); //csak az orajelert
    do {
    stat = SPIM_ReadTxStatus();
    }while(!(stat & SPIM_STS_SPI_DONE));//megvarom, h az osszes orajelet kiadja
    
    do {
       stat = SPIM_ReadRxStatus();
    }while(!(stat & SPIM_STS_RX_FIFO_NOT_EMPTY)); // a fifo nem ures
    data = SPIM_ReadRxData();
    SS_Write(0u);
    SPIM_ClearFIFO();
    return (data); 
    
}

int get_hour() {
   SPIM_IO_RD_Addr(0x84);
   uint8 resp = SPIM_IO_RD_Data();
   uint8 h_0 = (resp&0x0f);
   uint8 h_10 = (resp&0x30) >> 4;
   return h_10*10+h_0;
}

int get_minute() {
   SPIM_IO_RD_Addr(0x82);
   uint8 resp = SPIM_IO_RD_Data();
   uint8 min_0 = (resp&0x0f);
   uint8 min_10 = (resp&0x70) >> 4;
   return min_10*10+min_0;
}

int get_seconds() {

    SPIM_IO_RD_Addr(0x80);
    uint8 resp = SPIM_IO_RD_Data(); 
    uint8 sec_0 = (resp&0x0f);
    uint8 sec_10 = (resp&0x70) >> 4;
    return sec_10*10+sec_0;
}

int get_day() {
    SPIM_IO_RD_Addr(0x86);
    uint8 resp = SPIM_IO_RD_Data();
    uint8 date_0 = (resp&0x0f);
    uint8 date_10 = (resp&0x30) >> 4;
    return date_10*10+date_0;
}

int get_month() {
    SPIM_IO_RD_Addr(0x88);
    uint8 resp = SPIM_IO_RD_Data();
    uint8 month_0 = (resp&0x0f);
    uint8 month_10 = (resp&0x01) >> 4;
    return month_10*10+month_0;
}

int get_year() {
    SPIM_IO_RD_Addr(0x8C);
    uint8 resp = SPIM_IO_RD_Data();
    uint8 y_0 = (resp&0x0f);
    uint8 y_10 = (resp&0xf0) >> 4;
    return (y_10*10+y_0)+2000;
}


void print_time_string(char* str) {    
    sprintf(str, "%02d:%02d",get_hour(), get_minute());
}

void print_date(char* str) {
    sprintf(str, "20%02d-%02d-%02d",get_year(), get_month(), get_day());
}
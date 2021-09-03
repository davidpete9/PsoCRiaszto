#ifndef MAIN_H
#define MAIN_H

#include "project.h"
#include "TaskFIFO.h"
    
extern char command_buffer[255];    

extern TaskFIFO gsm_fifo;
    
void tone(uint16 freq, uint16 duration);    

#endif

/* [] END OF FILE */

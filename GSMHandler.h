/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef GSMHANDLER_H
#define GSMHANDLER_H


#include "TaskFIFO.h"
#include "project.h"

void send_AT_command();
void send_ATCMGF_command();

void send_SMSStarter_command();
void send_sms_command();

void start_sms_sending_sequence(TaskFIFO * gsm_fifo, char * m, char *n);

#endif

/* [] END OF FILE */

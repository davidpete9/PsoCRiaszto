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


#include "project.h"
#include "TaskFIFO.h"


char * message;
char * number;

void send_AT_command() {
    UART_GSM_PutString("AT");
    UART_GSM_PutChar(0x0D);
}

void send_ATCMGF_command() {
    UART_GSM_PutString("AT+CMGF=1");
    UART_GSM_PutChar(0x0D);
}

void send_SMSStarter_command() {
    char str[25];
    sprintf(str, "AT+CMGS=\"%s\"", number);
    UART_GSM_PutString(str);
    UART_GSM_PutChar(0x0D);
}

void send_sms_command() {
 UART_GSM_PutString(message);
 UART_GSM_PutChar(0x1A);
}

void start_sms_sending_sequence(TaskFIFO * gsm_fifo, char * m, char *n) {
    
    message = m;
    number = n;
    
    
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_AT_command, 500));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_ATCMGF_command, 2000));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_SMSStarter_command, 4000));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_sms_command, 4000));
  
}



/* [] END OF FILE */

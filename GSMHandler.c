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
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString("SMS sending 1/4..");
}

void send_ATCMGF_command() {
    UART_GSM_PutString("AT+CMGF=1");
    UART_GSM_PutChar(0x0D);
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString("SMS sending 2/4..");
}

void send_SMSStarter_command() {
    char str[25];
    sprintf(str, "AT+CMGS=\"%s\"", number);
    UART_GSM_PutString(str);
    UART_GSM_PutChar(0x0D);
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString("SMS sending 3/4..");
}

void send_sms_command() {
 UART_GSM_PutString(message);
 UART_GSM_PutChar(0x1A);
 LCD_ClearDisplay();
 LCD_Position(0u, 0u);
 LCD_PrintString("SMS sending 4/4..");
}

void power_on_modul() {
    GSM_DTR_Write(1u);
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString("SMS sending 0/4");
    LCD_Position(1u, 0u);
    LCD_PrintString("Power on..");
}

void power_down_cmd() {
    UART_GSM_PutString("AT+CPOWD=1");
    UART_GSM_PutChar(0x0D);
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString("Powering down..");
}

void power_off() {
    GSM_DTR_Write(0u);
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString("Powered off.");
}

void start_sms_sending_sequence(TaskFIFO * gsm_fifo, char * m, char *n) {
    
    message = m;
    number = n;
    
    pushTask(gsm_fifo,create_new_noparam_onetimetask(power_on_modul, 100));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_AT_command, 6000));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_ATCMGF_command, 2000));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_SMSStarter_command, 4000));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(send_sms_command, 4000));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(power_down_cmd, 10000));
    pushTask(gsm_fifo,create_new_noparam_onetimetask(power_off, 5000));
  
}



/* [] END OF FILE */

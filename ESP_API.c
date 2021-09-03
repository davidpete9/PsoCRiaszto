

#include "project.h"
#include "TaskFIFO.h"
#include "ds1302.h"
#include "GSMHandler.h"
#include "SettingsHelper.h"
#include "main.h"

char num_buffer[6];
char lastNonNum = 0;

uint8 cmd_index = 0;

uint8 isNumberic(uint8 d) {
return (d == '0'  || d == '1' || d == '2' || d == '3' || d == '4' || d == '5' || d == '6' 
    || d == '7'  || d == '8'  || d == '9') ? 1 : 0;
}

int16 readNumFromCommandUntilNonNum() {
    lastNonNum = 0;
    uint8 num_ind = 0;   
    while (cmd_index < 255 && command_buffer[cmd_index] != '\0') {
      char next = command_buffer[cmd_index++];
    
      if (isNumberic(next) == 1) {
        num_buffer[num_ind++] = next;
      }
      else {
        lastNonNum = next;
        if (num_ind == 0)
          return -1;
        num_buffer[num_ind] = '\0';
        return toInt(num_buffer);
      }
    }
    if (num_ind == 0)
          return -1;
    num_buffer[num_ind] = '\0';
    return toInt(num_buffer);
}


void getSysTime() {
    char str[20];
    print_datetime(str);
    UART_ESP_PutString(str);
}

void beepCommand() { //Csipogas -  C400 1000 / frekvencia hossz
    cmd_index = 0;
    int16 freq = readNumFromCommandUntilNonNum();
    int16 duration = readNumFromCommandUntilNonNum();
    
    
    if (freq < 0) {
      UART_ESP_PutString("Not valid command! #1");
      return;
    }
    
    if (duration < 0) {
        UART_ESP_PutString("Not valid command! #2");
        return;
    }
    
    tone(freq, duration);
    UART_ESP_PutChar('1');
}

char first_row[16+1];
char second_row[16+1];

void lcdCommand() {//LCD-re iras - L16 Szep napunk van ma / hossz + uzenet tordel, ha nem eleg
    cmd_index = 0;
    int16 len = readNumFromCommandUntilNonNum();
    if (len <= 0 || lastNonNum != ' ') {
      UART_ESP_PutString("Not valid command! #1");
      return;
    }
    
     uint8 row_len = len > 16 ? 16 : len;
     for (uint8 i = 0; i < row_len; i++) {
       first_row[i] = command_buffer[cmd_index++];
    }
    first_row[row_len] = '\0';
    if (len > 16) {
    row_len = len > 32 ? 16 : len-16; 
        for (uint8 i = 0; i < row_len; i++) {
           second_row[i] = command_buffer[cmd_index++];
        }
        second_row[row_len] = '\0';
        print_lines_to_lcd(first_row, second_row);
    }
    else {
        print_lines_to_lcd(first_row,"                ");
    }
}

char full_phone_num[13];
char sms_message[221];

void smsCommand() {//SMS kuldes - K702668307 16 Szep napunk van. / szam 9 jegy + uzenet hossza + uzenet
    cmd_index = 0;
    char phone_num[10];
    uint8 phone_num_ind = 0;
    while (command_buffer[cmd_index] != '\0') {
        if (isNumberic(command_buffer[cmd_index])) {
             phone_num[phone_num_ind++] = command_buffer[cmd_index++];
        }
        else if (command_buffer[cmd_index] == ' ') {
            cmd_index++;
            phone_num[phone_num_ind] = '\0';
            break;
        }
        if (phone_num_ind >= 10) {
            UART_ESP_PutString("Phone num too long.");
            return;
        }
    }
    if (phone_num_ind <= 2) {
        UART_ESP_PutString("Phone num too short.");
        return;
    }
    if (phone_num[phone_num_ind] != '\0') {
        UART_ESP_PutString("Phone num not vaild.");
        return;
    }
    
    sprintf(full_phone_num, "+36%s", phone_num);
    
    uint8 message_length = readNumFromCommandUntilNonNum();
    if (message_length <= 0) {
        UART_ESP_PutString("Not valid message found");
        return;
    }
    message_length = message_length > 220 ? 220 : message_length; //max 220 karakter hosszu SMS mehet.
    for (uint8 i = 0; i < message_length; i++) {
        if (command_buffer[cmd_index] == '\0') {
            UART_ESP_PutString("SMS message is not the promised length long");
            return;
        }
        sms_message[i] = command_buffer[cmd_index++];
    }
    
    start_sms_sending_sequence(&gsm_fifo, sms_message, full_phone_num);
    
}

/* [] END OF FILE */

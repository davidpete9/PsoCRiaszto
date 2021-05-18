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
#include "Keypad.h"
#include "SettingsHelper.h"
#include "EEPROM_Helper.h"

char variable_to_be_displayed[16];

menu_item menu_system[] = {
    {{"A:Param. B: Kod","C: Tel. *: Ki"}, handle_main_menu_page},
    {{"A:Elesitesi ido","1:<- 2:-> *: Ki"}, choose_param_page1},
    {{"A:Csipog. hossza","1:<- 2:-> *: Ki"}, choose_param_page2},
    {{"A:Auto. bekapcs.","1:<- 2:-> *: Ki"}, choose_param_page3},
    {{"A:Deakt. ido","1:<- 2:-> *: Ki"}, choose_param_page4},
    {{"A:Ejszaka kezd.","1:<- 2:-> *: Ki"}, choose_param_page5},
    {{"A:Ejszaka vege","1:<- 2:-> *: Ki"}, choose_param_page6},
    {{"A:Fenyerz. nezze","1:<- 2:-> *: Ki"}, choose_param_page7},
    {{variable_to_be_displayed,"Uj:"}, change_16_num}, //8
    {{variable_to_be_displayed,"Uj:"}, change_8_num}, //9
    {{variable_to_be_displayed,"1 : be, 0: ki"}, ask_bool}, //10
    {{"Kerem a kodot:",""}, vertify_code}, //11
    {{"Uj kodot: ",""}, select_new_code}, //12
    {{variable_to_be_displayed,"+36"}, edit_phone_number} //13
};

menu_item actual_menu_item;

uint8 actual_eeprom_addr = -1;


uint8 OUT_FROM_MENU = 0u;

//String to int.
uint16 toInt(char* str)
{
    uint16 mult = 1;
    uint16 re = 0;
    int len = strlen(str);
    for(int i = len -1 ; i >= 0 ; i--)
    {
        re = re + ((uint8)str[i] -48)*mult;
        mult = mult*10;
    }
    return re;
}

char num_buffer[6];
uint8 num_ind = 0;

char code_buffer[4];
uint8 code_buffer_ind = 0;

uint8 CODE[] = {'4','3','2','1'}; //Default valami. ugyis felul lesz irva.

char PHONE_NUM[] = "702668307";

uint8 NIGHT_FROM = 0;
uint8 NIGHT_TO = 24;

/*
Megnezi, hogy nincs e eppen a helyes kod beirva.
*/
uint8 try_to_read_code_from_keyboard() {
    uint8 pressed_key = get_pressed_key();
    if (pressed_key != 0) { //gombnyomas eseten -  ilyenkor feletelezem, hogy a kodot irom be
    
    if (!code_buffer_ind) {
        LCD_ClearDisplay();
        LCD_Position(1u, 0u);
    }    
        
    LCD_PutChar('*');
    code_buffer[code_buffer_ind++] = pressed_key;
    pressed_key = 0;                
    if (code_buffer_ind == 4) {
        uint8 result = 1u;
        for (uint8 i = 0; i < 4; i++) {
            if (code_buffer[i] != CODE[i]) {
                result = 0u;
                break;
            }
        }
        code_buffer_ind = 0;
        if (result == 0u) {
            LCD_Position(1u, 0u);
            LCD_PrintString("Hibas kod!");
            LCD_Position(1u, 0u);
        }
        return  result;
     }
   }
    return 0u;
}

int16 read_num_from_keypad(uint8 pressed) {
    
    
    if (pressed == '*')
        return -2;
    if (pressed == 0 || pressed == 'A' || pressed == 'B' || pressed == 'C' || pressed == 'D' || num_ind > 4) {
        return -1;
    } else if (pressed == '#'){
        if (num_ind == 0)
            return -1;
        num_buffer[num_ind] = '\0';
        uint16 result =  toInt(num_buffer);
        num_ind = 0;
        return result;
    }         
    num_buffer[num_ind++] = pressed;
    LCD_PutChar(pressed);
        
    return -1;      
}

void change_16_num() {
    int16 new_delay = read_num_from_keypad(get_pressed_key());
    if (new_delay >= 0) {
       StoreInt16(new_delay, actual_eeprom_addr);
       init_constants();
       change_page(&menu_system[1]);
    }
    else if (new_delay == -2) {
        num_ind = 0;
        change_page(&menu_system[1]);
    }
}

void change_8_num() {
    int16 new_delay = read_num_from_keypad(get_pressed_key());
    if (new_delay >= 0) {
       StoreInt8(new_delay, actual_eeprom_addr);
       init_constants();
       change_page(&menu_system[1]);
    }
    else if (new_delay == -2) {
        num_ind = 0;
        change_page(&menu_system[1]);
    }
}

void ask_bool() {
uint8 pressed = get_pressed_key();
if (pressed == '1') {
 StoreInt8(1, actual_eeprom_addr);
init_constants();
 change_page(&menu_system[0]);
}
else if (pressed == '0') {
 StoreInt8(0, actual_eeprom_addr);
 change_page(&menu_system[0]);
}
else if (pressed == '*') {
change_page(&menu_system[1]);
}

}

void print_lines_to_lcd(const char * line1, const char * line2) {
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString(line1);
    LCD_Position(1u,0u);
    LCD_PrintString(line2);
}

void print_page(page * p) {
    print_lines_to_lcd(p->line1, p->line2);
}

void change_page(menu_item * dest) {
    print_page(&(dest->page));
    actual_menu_item = *dest;
} 

void vertify_code() {
   if (try_to_read_code_from_keyboard()) {
        change_page(&menu_system[12]);
    }
}


uint8 new_code_buffer[4];
uint8 new_code_buffer_ind = 0;

uint8 read_new_code(uint8 pressed) {
    if (pressed != 0) {
        if (new_code_buffer_ind == 0) {
            LCD_Position(1u, 0u);
        }
        new_code_buffer[new_code_buffer_ind++] = pressed;
        LCD_PutChar('*');
        if (new_code_buffer_ind == 4) {
            return 1u; 
        } 
    }
    return 0u;
}

uint8 entered_code[4];
uint8 first_entered = 0u;

void select_new_code() {
    if (first_entered == 0u) {
         if (read_new_code(get_pressed_key()) != 0u) {
            for (uint8 i = 0; i < 4; i++)
                entered_code[i] = new_code_buffer[i];    
                
            first_entered = 1u;    
            new_code_buffer_ind = 0;
            print_lines_to_lcd("Uj kod ujra:","             ");    
        }
    }
    else {
        if (read_new_code(get_pressed_key()) != 0u) {
           
             for (uint8 i = 0; i < 4; i++) {
                if (entered_code[i] != new_code_buffer[i]) {
                    print_lines_to_lcd("Nem egyeznek meg!","             ");
                    first_entered = 0u;
                    new_code_buffer_ind = 0;
                    return;
                }   
             }
            StoreInt8ArrayToRow(new_code_buffer, CODE_ADDR);
            first_entered = 0u;
            new_code_buffer_ind = 0;
            init_code();
            change_page(&menu_system[0]);
        }
    }
}

void init_constants() {
NIGHT_FROM = ReadInt8(NIGHT_FROM_ADDR);
NIGHT_TO = ReadInt8(NIGHT_UNITL_ADDR);
}

void init_code() {
ReadInt8ArrayFromRow(CODE, 4, CODE_ADDR);
}

uint8 phone_num_temp_buffer[10];
void init_phone_num() {
    
ReadInt8ArrayFromRow(phone_num_temp_buffer, 9, PHONE_NUM_1_ADDR);   
for (uint8 i = 0; i < 9; i++) {
PHONE_NUM[i] = phone_num_temp_buffer[i];
}
PHONE_NUM[9] = '\0';


}

/*
Le kell futtatni mindig, a menurenszerbe lepes elott.
*/
void init_settings() {
OUT_FROM_MENU = 0u;
change_page(&menu_system[0]);

}

uint8 phone_num_buffer[9];
int phone_num_buffer_ind = 0;

int8 read_phone_num(uint8 pressed) {

    if (pressed == '*')
        return -2;
    
   if (pressed == '#'){
        if (phone_num_buffer_ind == 0)
            return -1;
        phone_num_buffer_ind = 0;
        return 1;
    } 

    if (pressed == 0 || pressed == 'A' || pressed == 'B' || pressed == 'C' || pressed == 'D' || phone_num_buffer_ind > 9) {
        return -1;
    }         
    phone_num_buffer[phone_num_buffer_ind++] = pressed;
    LCD_PutChar(pressed);
        
    return -1;      
    
}

void edit_phone_number() {
    int8 res = read_phone_num(get_pressed_key());
   if (res == 1) {
        StoreInt8ArrayToRow(phone_num_buffer, PHONE_NUM_1_ADDR);
        init_phone_num();
        change_page(&menu_system[0]);
        return;
    }
    else if (res == -2) {
         phone_num_buffer_ind = 0;
         change_page(&menu_system[0]);
    } 

}




//{"A: Param. B: Kod","C: Tel. *: Ki"}
void handle_main_menu_page() {
    uint8 pressed = get_pressed_key();
    if (pressed == 'A') {
        change_page(&menu_system[1]); //Param
    }
    else if (pressed == 'B') {
        change_page(&menu_system[11]); //Kod valoztatas
    }
    else if (pressed == 'C') {
        actual_eeprom_addr = PHONE_NUM_1_ADDR;
        sprintf(variable_to_be_displayed, "%s #/*", PHONE_NUM);
        LCD_Position(4u, 0u);
        change_page(&menu_system[13]); //Tel. szam
    }
    else if (pressed == '*') {
       OUT_FROM_MENU = 1u;
    }
}

//{"A: Elesitesi ido","#:<- D:-> *: Ki"}
void choose_param_page1() {
    uint8 pressed = get_pressed_key();
    
    if (pressed == 'A') {        
        actual_eeprom_addr = WATCH_DELAY_ADDR;
        uint16 current_watch_time = ReadInt16(actual_eeprom_addr);
        sprintf(variable_to_be_displayed, "%d s #:Ok *:X", current_watch_time);
        LCD_Position(1u, 4u);
        change_page(&menu_system[8]);
    }
    else if (pressed == '1') {
       change_page(&menu_system[7]); 
    }
    else if (pressed == '2') {
        change_page(&menu_system[2]);
    
    }else if (pressed == '*') {
        change_page(&menu_system[0]);
    }
}

//{"A:Csipog. hossza","1:<- 2:-> *: Ki"}
void choose_param_page2() {
    uint8 pressed = get_pressed_key();
    
    if (pressed == 'A') {        
        actual_eeprom_addr = BEEP_LENGTH_ADDR;
        uint16 beep_len = ReadInt16(actual_eeprom_addr);
        sprintf(variable_to_be_displayed, "%d s #:Ok *:X", beep_len);
        LCD_Position(1u, 4u);
        change_page(&menu_system[8]);
    }
    else if (pressed == '1') {
       change_page(&menu_system[1]); 
    }
    else if (pressed == '2') {
        change_page(&menu_system[3]);
    
    }else if (pressed == '*') {
        change_page(&menu_system[0]);
    }
}

//{"A:Auto. bekapcs.","1:<- 2:-> *: Ki"}
void choose_param_page3() {
    uint8 pressed = get_pressed_key();
    
    if (pressed == 'A') {        
        actual_eeprom_addr = AUTO_WATCH_ACTIVATED_ADDR;
        uint8 stat_now = ReadInt8(actual_eeprom_addr);
        sprintf(variable_to_be_displayed, "%s van kapcsolva", stat_now == 1 ? "BE" : "KI");
        LCD_Position(1u, 4u);
        menu_system[10].page.line2 = "1: Be 0: Ki *:X";
        change_page(&menu_system[10]);
    }
    else if (pressed == '1') {
       change_page(&menu_system[2]); 
    }
    else if (pressed == '2') {
        change_page(&menu_system[4]);
    
    }else if (pressed == '*') {
        change_page(&menu_system[0]);
    }
}

//{"A:Deakt. ido","1:<- 2:-> *: Ki"}
void choose_param_page4() {
    uint8 pressed = get_pressed_key();
    
    if (pressed == 'A') {        
        actual_eeprom_addr = DEACT_DELAY_ADDR;
        uint16 deact_delay = ReadInt16(actual_eeprom_addr);
        sprintf(variable_to_be_displayed, "%d s #:Ok *:X", deact_delay);
        LCD_Position(1u, 4u);
        change_page(&menu_system[8]);
    }
    else if (pressed == '1') {
       change_page(&menu_system[3]); 
    }
    else if (pressed == '2') {
        change_page(&menu_system[5]);
    
    }else if (pressed == '*') {
        change_page(&menu_system[0]);
    }
}

//{"A:Ejszaka kezd.","1:<- 2:-> *: Ki"}
void choose_param_page5() {
    uint8 pressed = get_pressed_key();
    
    if (pressed == 'A') {        
        actual_eeprom_addr = NIGHT_FROM_ADDR;
        uint8 night_start = ReadInt8(actual_eeprom_addr);
        sprintf(variable_to_be_displayed, "%d h #:Ok *:X", night_start);
        LCD_Position(1u, 4u);
        menu_system[9].page.line2 = "Uj (0-24):";
        change_page(&menu_system[9]);
    }
    else if (pressed == '1') {
       change_page(&menu_system[4]); 
    }
    else if (pressed == '2') {
        change_page(&menu_system[6]);
    
    }else if (pressed == '*') {
        change_page(&menu_system[0]);
    }
}

//{"A:Ejszaka vege","1:<- 2:-> *: Ki"}
void choose_param_page6() {
    uint8 pressed = get_pressed_key();
    
    if (pressed == 'A') {        
        actual_eeprom_addr = NIGHT_UNITL_ADDR;
        uint16 night_to = ReadInt8(actual_eeprom_addr);
        sprintf(variable_to_be_displayed, "%d h #:Ok *:X", night_to);
        LCD_Position(1u, 4u);
        menu_system[9].page.line2 = "Uj (0-24):";
        change_page(&menu_system[9]);
    }
    else if (pressed == '1') {
       change_page(&menu_system[5]); 
    }
    else if (pressed == '2') {
        change_page(&menu_system[7]);
    
    }else if (pressed == '*') {
        change_page(&menu_system[0]);
    }
}

//{"A:Fotorezisztor?","1:<- 2:-> *: Ki"}
void choose_param_page7() {
    uint8 pressed = get_pressed_key();
    
    if (pressed == 'A') {        
        actual_eeprom_addr = USE_PHOTO_RES_ADDR;
        uint8 stat_now = ReadInt8(actual_eeprom_addr);
        sprintf(variable_to_be_displayed, "%s van kapcsolva", stat_now == 1 ? "BE" : "KI");
        LCD_Position(1u, 4u);
        menu_system[10].page.line2 = "1: Be 0: Ki *:X";
        change_page(&menu_system[10]);
    }
    else if (pressed == '1') {
       change_page(&menu_system[6]); 
    }
    else if (pressed == '2') {
        change_page(&menu_system[1]);
    
    }else if (pressed == '*') {
        change_page(&menu_system[0]);
    }
}

void process_settings() {
    actual_menu_item.handler();
}





/* [] END OF FILE */

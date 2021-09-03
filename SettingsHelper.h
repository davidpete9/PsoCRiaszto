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
#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H
    
#define LCD_COMMUNICATION 0
#define COMPUTER_COMMUNICATION 1  
#define ESP_COMMUNICATION 2  
    
#include "project.h"

typedef struct page {
char * line1;
char * line2;
}page; 

typedef struct menu_item {
  page page;   
  void (*handler)(); 
} menu_item; 

extern uint8 OUT_FROM_MENU;

extern uint8 CODE[];

extern char PHONE_NUM[];

extern uint8 NIGHT_FROM;
extern uint8 NIGHT_TO;
extern uint8 PH_RES_ON;
extern uint8 AUTO_POWERON;
extern uint16 WATCH_DELAY;
extern uint16 DEACT_DELAY;
extern uint16 BEEPING_DELAY;

extern uint8 COMMUNICATION_MODE;

void printChar(char8 c);

void printString(const char8* str);

uint8 try_to_read_code_from_keyboard();

void change_16_num();

void change_8_num();

void ask_bool();

void print_lines_to_lcd(const char * line1, const char * line2);

void print_page(page * p);
void change_page(menu_item * dest);

void select_new_code();

void vertify_code();

void handle_code_change();

//Betolt mindenfel parameter az eeprombol a memoriaba. Ez hivodik minden modositas utan
void init_constants();

//A kodot betolti az EEPROM-bol a memoriaba
void init_code();

//A tel.szamot
void init_phone_num();

/*
Le kell futtatni mindig, a menurenszerbe lepes elott.
*/
void init_settings();

void edit_phone_number();

//{"A: Param. B: Kod","C: Tel. *: Ki"}
void handle_main_menu_page();
void change_watch_param();

//{"A: Elesitesi ido","#:<- D:-> *: Ki"}
void choose_param_page1();
void choose_param_page2();
void choose_param_page3();
void choose_param_page4();
void choose_param_page5();
void choose_param_page6();
void choose_param_page7();

void process_settings();

uint16 toInt(char* str);
    
  
    
#endif


/* [] END OF FILE */

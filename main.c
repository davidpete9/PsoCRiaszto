/* ========================================
 *
Riaszto kodja  - kesz verzio
 *
 * ========================================
*/
#include "project.h"
#include "ds1302.h"
#include "PeriodicTask.h"
#include "TaskFIFO.h"
#include "Keypad.h"
#include "hb100movement.h"
#include "GSMHandler.h"
#include "EEPROM_Helper.h"
#include "SettingsHelper.h"
#include "ESP_API.h"

//allapotok
typedef enum state {figyelo, passziv, riasztas} state;
typedef enum substate {figyelo_varakozo,figylo_deaktivalo, passziv_felkeszules, passziv_varakozo,passziv_beallitasok, riasztas_idle, riasztas_init, passziv_felodott} substate;


/*
Csipogok egy adott frekvencian, egy adott ideig.
Olyat probalok csinalni, mint az arduinos tone()
*/
void start_tone(uint16 freq) {
    //beallitom egy olyan periodust a PWM-nek, ami a freq
    uint16 clk = 20000; 
    // period*freq = 20000 
    uint16 period = clk/freq;
    PWM_BEEPER_STATUS;
    PWM_BEEPER_WritePeriod(period);
    PWM_BEEPER_WriteCompare(period/2);
    PWM_BEEPER_Start();
}

void noTone() {
    PWM_BEEPER_WriteCompare(0);
    PWM_BEEPER_Stop();
}

periodic_task_list p_tasks;
TaskFIFO tone_fifo;
TaskFIFO gsm_fifo;
TaskFIFO timing_fifo;

//1 ms-onkent
CY_ISR(TIMER_ISR)
{
    
  run_permitted_tasks_and_decrement_counters(&p_tasks); 
  run_next_task_when_available(&tone_fifo);
  run_next_task_when_available(&gsm_fifo);
  run_next_task_when_available(&timing_fifo);

  Timer_ReadStatusRegister();
}


void check_photoresistor(uint8 * need_alert){
    
    volatile uint16 light = ADC_SAR_PR_GetResult16();
    //ha ejszaka van, viszont vilagos, akkor riasztok
    volatile uint8 h = get_hour();
    if (light < 300 && h > NIGHT_FROM && h < NIGHT_TO && PH_RES_ON == 1u) {
       *need_alert = 1u;         
    }
}

void change_backlight_with_light() {
  volatile uint16 light = ADC_SAR_PR_GetResult16();
  PWM_BACKLIGHT_STATUS; 
  
  if (light >= 500) {
         //350 a max
         uint16 dutycycle = 0;
        if (light < 1000) {
            dutycycle = 0.25*350;
        }
        else if (light < 1500) {
           dutycycle = 0.5*350;
        }
        else if (light < 2000) {
           dutycycle = 0.75*350;
        }
        else {
            dutycycle = 350;
        }
        
       
        PWM_BACKLIGHT_WriteCompare(dutycycle);
    } 
    else {    
        PWM_BACKLIGHT_WriteCompare(0.1*350u);
    }
}

void phres_command() {
    volatile uint16 ll = ADC_SAR_PR_GetResult16();
    char str[30];
    sprintf(str, "Phres value: %d", ll);
    UART_ESP_PutString(str); 
}


void init() {
    //Periferiak.
    isr_Timer_StartEx(TIMER_ISR);
    ADC_SAR_PR_Start();
    ADC_SAR_PR_StartConvert();
    Timer_Start();
    LCD_Start();
    LCD_Position(0u, 0u);
    SPIM_Start();
    SPIM_TxDisable();
    PWM_BACKLIGHT_Start();
    UART_Start();
    UART_GSM_Start();
    UART_ESP_Start();
    VDAC8_Start();
    VDAC8_ESP_Start();
    EEPROM_Start();
}

void tone(uint16 freq, uint16 duration) {
      pushTask(&tone_fifo, create_new_onetimetask(start_tone, 0, freq));
      pushTask(&tone_fifo, create_new_noparam_onetimetask(noTone, duration));
}

void print_time_to_lcd() {
    char str[16];
    print_time_string(str);
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString(str);
}


void play_alert() {
     tone(400, 300);
     tone(500, 300);
     tone(600, 300);
     tone(700, 300);
}

uint16 visual_counter_value = 255;

void visualized_counter() {
    LCD_Position(1u, 0u);
    LCD_PrintString("     "); //Nem akarom kitorolni az egesz kepernyot
    char str[8];
    sprintf(str, "%d s", visual_counter_value);
    LCD_Position(1u, 0u);
    LCD_PrintString(str);
    visual_counter_value -= 1;
    
    if (COMMUNICATION_MODE == ESP_COMMUNICATION) {
        UART_ESP_PutString(str);
    }
}

void finish_counting() {
    disable_task(&p_tasks,5);
}

state current_state = passziv;
substate actual_substate = passziv_varakozo;
uint8 is_light_alert = 0u;
uint8 is_movement_alert = 0u;
uint8 is_reed_relay_alert = 0u;

void switch_to_watching_mode() {
    current_state = figyelo;
    actual_substate = figyelo_varakozo;
    print_lines_to_lcd("figyelo","modban");
    tone(400,400);
    tone(800,400);
    enable_task(&p_tasks,3); //mozgaserzekelesi riasztas figyelese
    enable_task(&p_tasks,6); //reed rele figyelese
    
    if (PH_RES_ON == 1u) {
       enable_task(&p_tasks,2); //fotorez kell
    }
    disable_task(&p_tasks, 7); //LCD hattervilagitas ilyenkor nem kell
    
    is_light_alert = 0u;
    is_movement_alert = 0u;
    is_reed_relay_alert = 0u;
    
    PWM_BACKLIGHT_STATUS;
    PWM_BACKLIGHT_WriteCompare(0u);
}

void is_reed_alert(uint8 * is_reed_alert) {
    if (REED_RELAY_Read() == 0u) {
        *is_reed_alert = 1u;
    }
    else {
        *is_reed_alert = 0u;
    }
}

char alert_message[16];

void go_to_alarm_idle() {
    disable_task(&p_tasks, 100); //Csipogas ki
    actual_substate = riasztas_idle;
    char time_str[7];
    print_time_string(time_str);  
   
    print_lines_to_lcd(time_str,"RIASZTAS!");
    
}

char alert_sms[35];
char full_phone_num[13];
//Ez fut le riasztaskor
void create_alarm() {
    current_state = riasztas;
    actual_substate = riasztas_init;
    
    char time_str[7];
    print_time_string(time_str);
    
    
    
    sprintf(alert_sms, "%s - ekkor: %s", alert_message, time_str);
    
    enable_task(&p_tasks, 7); //LCD hattervil.
    
    print_lines_to_lcd("RIASZTASI MOD",time_str);
    
    

    sprintf(full_phone_num, "+36%s", PHONE_NUM);
    full_phone_num[12] = '\0';
    
    
    if (AUTO_POWERON == 1u) {
    //1. SMS-ek kuldese
    start_sms_sending_sequence(&gsm_fifo, alert_sms, full_phone_num);
    }
    
    //2. Csipogas kezdese
    enable_task(&p_tasks, 100);
    
    pushTask(&timing_fifo, create_new_noparam_onetimetask(go_to_alarm_idle, BEEPING_DELAY*1000));
    
}

void to_passiv_move() {
    disable_task(&p_tasks, 100); //Csipogas riasztashoz
    disable_task(&p_tasks,5);  //visszaszamolo
    disable_task(&p_tasks,6); //reed rele
    disable_task(&p_tasks,3); //mozgaserzekelo
    disable_task(&p_tasks, 2); //fozorez.
    enable_task(&p_tasks, 7); //Hattervil.
    enable_task(&p_tasks, 4); //ora.
    is_light_alert = 0u;
    is_movement_alert = 0u;
    is_reed_relay_alert = 0u;
    actual_substate = passziv_varakozo;
    current_state = passziv;
    
}

char command_buffer[255];
uint8 command_buffer_ind = 0;
uint8 COMMAND_READING = 0;
void (*command_processor)();

int main(void)
{
    init();
    
    //Kod es tel. szam be a memoriaba az eeprombol.
    //Annyiszor hasznalom, hogy nem erne meg mindig az EEPROM-ot piszkalni.
    init_code();
    init_phone_num();
    init_constants();
    
    tone_fifo.start = NULL;
    tone_fifo.end = NULL;
    
    
    gsm_fifo.start = NULL;
    gsm_fifo.end = NULL;
    
    p_tasks.start = NULL;
    p_tasks.end = NULL;
    
    timing_fifo.start = NULL;
    timing_fifo.end = NULL;
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    
    add_to_ptask_list(&p_tasks, create_new_ptask_noparam(readKey, 30, 1));
    add_to_ptask_list(&p_tasks, create_new_ptask_parametered(check_photoresistor, &is_light_alert, 200, 2));
    add_to_ptask_list(&p_tasks, create_new_ptask_parametered(check_movement, &is_movement_alert, 100, 3));
    add_to_ptask_list(&p_tasks, create_new_ptask_parametered(is_reed_alert, &is_reed_relay_alert, 100, 6));
    add_to_ptask_list(&p_tasks, create_new_ptask_noparam(print_time_to_lcd, 50000, 4));
    add_to_ptask_list(&p_tasks, create_new_ptask_noparam(change_backlight_with_light, 300, 7));
    add_to_ptask_list(&p_tasks, create_new_ptask_noparam(play_alert, 3000, 100));
    
    //Visszaszamlalo. Sokszor jol fog jonni. Alapbol disabled
    add_to_ptask_list(&p_tasks, create_new_ptask_noparam(visualized_counter, 1000, 5));
    
    //Akapbol passziv mod, sokminden nem kell

    to_passiv_move();

    volatile uint8 pressed = 0;
   
    char time_str[7];
    //GSM_DTR_Write(1u);
    
    for(;;)
    {   
         switch (current_state) {
            case passziv:
                switch (actual_substate) {
                    case passziv_felkeszules:
                    /* SEMMI TEENDO, AUTOMATA INNEN AZ ALLAPOTVALTAS*/
                        break;
                    case passziv_varakozo:
                        if (try_to_read_code_from_keyboard()) {
                            disable_task(&p_tasks, 4); // Az orat mar nem irom ki a kepernyore
                            print_lines_to_lcd("# - figyelo mod","* - beallitasok");
                            actual_substate = passziv_felodott;
                            pushTask(&timing_fifo, create_new_noparam_onetimetask(to_passiv_move, 30000)); 
                        }
                        break;
                    case passziv_felodott:
                        pressed = get_pressed_key();
                        if (pressed != 0) {
                        get_and_remove_first(&timing_fifo);
                        }
                        if (pressed == '#') {
                            actual_substate = passziv_felkeszules;
                            visual_counter_value = WATCH_DELAY;
                            print_lines_to_lcd("Elesites...","");
                            enable_task(&p_tasks, 5);
                            pushTask(&timing_fifo, create_new_noparam_onetimetask(finish_counting, visual_counter_value*1000));
                            pushTask(&timing_fifo, create_new_noparam_onetimetask(switch_to_watching_mode, 0));
                        }
                        else if (pressed == '*') {
                            actual_substate = passziv_beallitasok;
                            disable_task(&p_tasks, 4);
                            init_settings();
                            
                        }
                        break;
                    case passziv_beallitasok:
                        if (OUT_FROM_MENU == 1u) {
                            to_passiv_move();
                            print_time_to_lcd();
                        } else {
                            process_settings();
                        }
                        break;
                    default:
                        break;
                }
                break;
            case figyelo:
                switch (actual_substate) {
                    case figyelo_varakozo:
                        if (is_light_alert || is_movement_alert || is_reed_relay_alert) {
                           
                           if (is_light_alert) {
                                sprintf(alert_message, "R: Fenyerzekelo");
                           } 
                            else if (is_movement_alert) {
                               sprintf(alert_message, "R: Mozgaserz.");
                           }
                           else if (is_reed_relay_alert) {
                            sprintf(alert_message, "R: Reed-rele.");
                           }
                            enable_task(&p_tasks, 7);
                           print_lines_to_lcd(alert_message, "Irja be a kodot!"); 
                           
                           actual_substate = figylo_deaktivalo; 
                           pushTask(&timing_fifo, create_new_noparam_onetimetask(create_alarm, DEACT_DELAY*1000));
                            
                        }
                        if (try_to_read_code_from_keyboard() == 1u) {
                            to_passiv_move();
        
                        }
                        break;
                    case figylo_deaktivalo:
                        if (try_to_read_code_from_keyboard() == 1u) {
                            get_and_remove_first(&timing_fifo);
                            to_passiv_move();  
                            print_time_to_lcd();
        
                        }
                        break;
                    default:
                        break;
                }
                break;
            case riasztas:
                switch (actual_substate) {
                    case riasztas_idle:
                     if (try_to_read_code_from_keyboard() == 1u) {
                            to_passiv_move();   
                            print_time_to_lcd();
                            free_taskfifo(&gsm_fifo);
                            free_taskfifo(&timing_fifo);
                            GSM_DTR_Write(0u);
                      
                        }
                        break;
                    case riasztas_init:
                     if (try_to_read_code_from_keyboard() == 1u) {
                            get_and_remove_first(&timing_fifo);
                            to_passiv_move();  
                            print_time_to_lcd();
                            free_taskfifo(&gsm_fifo);
                            free_taskfifo(&timing_fifo);
                            GSM_DTR_Write(0u);
                        }
                        break;
                    default:
                        break;
                }
                break;
        }
        
        if (UART_GSM_GetRxBufferSize()) {
            
            if (COMMUNICATION_MODE == COMPUTER_COMMUNICATION) {
                UART_PutChar(UART_GSM_GetChar());
            }
            else {
            UART_ESP_PutChar(UART_GSM_GetChar());
            }
        }
        
         //Kulonallo interface, az ESP-nek API-t biztositok, hogy ki tudja hasznalni a riaszto lehetosegeit
         if (UART_ESP_GetRxBufferSize() && COMMUNICATION_MODE != ESP_COMMUNICATION && COMMAND_READING == 0) {
            char arrived = UART_ESP_GetChar();
            if (arrived == '@') {
                COMMUNICATION_MODE = ESP_COMMUNICATION;
                printString("ESP UART interface hasznalatban. Vissza $ beirasaval.\r\n");
                printString("Ird be a kodot a folytatashoz.\r\n");
                to_passiv_move();
            }
            else {
                switch (arrived) {      
                        /* GET */
                       case 'T': //Rendszerido lekerdezese
                        getSysTime();
                        break;
                       case 'A': // Visszater azzal,hogy most epp a mozgase
                        if (current_state == riasztas) {
                           UART_ESP_PutString("Riasztasi modban van.");
                           UART_ESP_PutString(alert_message);   
                        }
                        else if (current_state == figyelo){
                            UART_ESP_PutString("Figyelo modban van.");
                        }
                        else {
                            UART_ESP_PutString("Passziv modban van.");
                        }
                        break;
                       case 'F': //Fenyerzekelo ertekenek a lekerdezes
                        phres_command();
                        break;
                       case 'R': //Mozgasrzekelo statusa
                            UART_ESP_PutString(RADAR_Read() == 1u ? "Mozgas van most" : "Nincs mozgas.");
                        break;
                       case 'J':
                            UART_ESP_PutString(REED_RELAY_Read() == 0u ? "Reed rele ossze van erve" : "Nincs osszerve");
                        break;
                       case 'O':
                            disable_task(&p_tasks, 7);
                            PWM_BACKLIGHT_STATUS;
                            PWM_BACKLIGHT_WriteCompare(0u);
                            break;
                       case 'P':
                            enable_task(&p_tasks, 7);
                            break;
                            
                        /* ACTIONS */
                        /* Belefer-e blokkolas ha egy uzenet tovabbi reszeire varunk? */
                        
                        case 'K': //SMS kuldes - K702668307 16 Szep napunk van. / szam 9 jegy + uzenet hossza + uzenet
                        COMMAND_READING = 1;
                        command_processor = smsCommand;
                        break;
                        case 'C': //Csipogas - C400 1000 / frekvencia hossz
                        COMMAND_READING = 1;
                        command_processor = beepCommand;
                        break;
                        case 'L': //LCD-re iras - L16 Szep napunk van ma / hossz + uzenet tordel, ha nem eleg
                        COMMAND_READING = 1;
                        command_processor = lcdCommand;
                        break;
                        case 'I': //Ora beallitas - I2021 08 28 22 56 12
                        break;
                        case 'G': //GSM modul teszteles - G2 AT / kuldendo parancs hossza + parancs
                        break;
                        default:
                        break;
                    }
            }
        } else if (COMMAND_READING == 1 && UART_ESP_GetRxBufferSize()) {
            uint8 arrived = UART_ESP_GetByte();
            //dec. 212 Veget a parancsnak ezzel a koddal jelzem. Vagy kufutnank a tombbol.
            if (arrived == 0xD4 || command_buffer_ind >= 254) { 
                command_buffer[command_buffer_ind] = '\0';
                command_buffer_ind = 0;
                COMMAND_READING = 0;
                command_processor();
            }
            else {
                command_buffer[command_buffer_ind++] = arrived;
            }
        }
        
         if (UART_GetRxBufferSize() && COMMUNICATION_MODE != COMPUTER_COMMUNICATION) {
            char arrived = UART_GetChar();
            if (arrived == '@') {
                COMMUNICATION_MODE = COMPUTER_COMMUNICATION;
                printString("PC UART interface hasznalatban. Vissza $ beirasaval.\r\n");
                printString("Ird be a kodot a folytatashoz.\r\n");
                to_passiv_move();
            }
        }     
        
        
   
    }
}

/* [] END OF FILE */

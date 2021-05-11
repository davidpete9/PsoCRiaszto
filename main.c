/* ========================================
 *
Riaszto kodja  - Felkesz / Vaz eddig
 *
 * ========================================
*/
#include "project.h"
#include "ds1302.h"
#include "PeriodicTask.h"
#include "TaskFIFO.h"
#include "Keypad.h"
#include "hb100movement.h"

//KONTSTANSOK - Majd az EEPROM-ban tarolom, es lehet allitgatni
char* CODE = "4321";
uint8 NIGHT_FROM = 0;
uint8 NIGHT_TO = 24;


//allapotok
typedef enum state {figyelo, passziv, riasztas} state;
typedef enum substate {figyelo_varakozo,figylo_deaktivalo, passziv_felkeszules, passziv_varakozo, riasztas_idle, riasztas_init, passziv_felodott} substate;

//ezzel szamolok timer interruptban.
static uint32 counter = 0;


//ALLAPOTOK
 state actual_state = passziv;
 substate actual_substate = passziv_varakozo;

char alart_reason[16];


uint16 tone_deadline = 0;
uint8 beep_mode = 0u;

uint8 alarm_beep_mode = 0u;
uint32 alarm_until = 0;

uint8 TIMER_IT_MS = 50;



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
TaskFIFO fifo_tasks;

//1 ms-onkent
CY_ISR(TIMER_ISR)
{
   run_permitted_tasks_and_decrement_counters(&p_tasks);
    
   if (check_if_runnable_and_decrement_counter(&fifo_tasks) == 1u) {
        
        OneTimeTask * to_run = get_and_remove_first(&fifo_tasks);
        if (to_run !=  NULL) {
            if (to_run->func != NULL) {
                to_run->func(to_run->data);
                free(to_run);
            }
            if (to_run->funcnoparam != NULL) {
                to_run->funcnoparam();
                free(to_run);
            }
        }
    }
    
    Timer_ReadStatusRegister();
}


void check_photoresistor(uint8 * need_alert){
    volatile uint16 light = ADC_SAR_PR_GetResult16();
    
    //1. ha ejszaka van, viszont vilagos, akkor riasztok
    volatile uint8 h = get_hour();
    if (light < 1000 && h > NIGHT_FROM && h < NIGHT_TO) {
       *need_alert = 1u;         
    }
   
     PWM_BACKLIGHT_STATUS; 
    //2 .kijelzo hattervilagitasanak allitasa
    if (light > 1800) {
        uint16 test = 1+((light-1800)/100);
        uint16 dutycle = test*20 > 350 ? 350 : test*20;
        PWM_BACKLIGHT_WriteCompare(dutycle);
    } 
    else {    
        PWM_BACKLIGHT_WriteCompare(0u);
    }
}


void init() {
    
    isr_Timer_StartEx(TIMER_ISR);
    ADC_SAR_RADAR_IRQ_StartEx(ADC_SAR_RADAR_ISR);
    ADC_SAR_RADAR_Start();
    ADC_SAR_RADAR_StartConvert();
    ADC_SAR_PR_Start();
    ADC_SAR_PR_StartConvert();
    Timer_Start();
    LCD_Start();
    LCD_Position(0u, 0u);
    SPIM_Start();
    SPIM_TxDisable();
    PWM_BACKLIGHT_Start();
    UART_Start();
}

char code_buffer[4];
uint8 code_buffer_ind = 0;


uint8 currentlyPressedKey = 54;
/*
Megnezi, hogy nincs e eppen a helyes kod beirva.
*/
uint8 try_to_read_code_from_keyboard() {
    
    if (currentlyPressedKey != 0) { //gombnyomas eseten -  ilyenkor feletelezem, hogy a kodot irom be
    
    if (!code_buffer_ind) {
    LCD_ClearDisplay();
    LCD_Position(1u, 0u);
    }    
        
    LCD_PutChar('*');
    code_buffer[code_buffer_ind++] = currentlyPressedKey;
    currentlyPressedKey = 0;                
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
            LCD_ClearDisplay();
            LCD_Position(0u, 0u);
            LCD_PrintString("Hibas kod!");
            LCD_Position(1u, 0u);
        }
        return  result;
     }
   }
    return 0u;
}


void print_time_to_lcd() {
    char str[16];
    print_time_string(str);
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString(str);
}

void tone(uint16 freq, uint16 duration) {
      pushTask(&fifo_tasks, create_new_onetimetask(start_tone, 0, freq));
      pushTask(&fifo_tasks, create_new_noparam_onetimetask(noTone, duration));
}

void play_alert() {
    tone(1000, 200);
    tone(900, 300);
    tone(800, 400);
    tone(700, 500);
    tone(900, 600);
    tone(1400, 500);
    tone(2000, 400);
    tone(2400, 300);
   
}

int main(void)
{
    init();
    
    fifo_tasks.start = NULL;
    fifo_tasks.end = NULL;
    p_tasks.start = NULL;
    p_tasks.end = NULL;
    
    CyGlobalIntEnable; /* Enable global interrupts. */
     
   // char str[10];
   // sprintf(str, "%d", sizeof(int));
   //  LCD_PrintString(str);
    //OneTimeTask * test = (OneTimeTask*)malloc(10*sizeof(OneTimeTask));
  

    
    uint8 is_light_alert = 0u;
    uint8 is_movement_alert = 0u;
  
    
    add_to_ptask_list(&p_tasks, create_new_ptask_noparam(readKey, 30, 1));
    add_to_ptask_list(&p_tasks, create_new_ptask_parametered(check_photoresistor, &is_light_alert, 200, 2));
    add_to_ptask_list(&p_tasks, create_new_ptask_parametered(check_movement, &is_movement_alert, 100, 3));
 //   add_to_ptask_list(&p_tasks, create_new_ptask_noparam(print_time_to_lcd, 1000, 4));
    
   // LCD_ClearDisplay();
    
    for(;;)
    {   
        
        
   
      if (get_pressed_key() == '2') {
        play_alert();
      } 
      
        
       CyDelay(1000);
    }
}

/* [] END OF FILE */

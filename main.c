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

//ide mentem az aktualisan lenyomott gombokat
char currentlyPressedKey = 0;
uint8 waiting_for_relese = 0u;

uint16 tone_deadline = 0;
uint8 beep_mode = 0u;

uint8 alarm_beep_mode = 0u;
uint32 alarm_until = 0;

uint8 TIMER_IT_MS = 50;

//Megnezem, hogy van e lenyomott billentyu, ha nincs 0-val ter vissza,
// ha van akkor a billenyu ASCII kodjaval. 
//A ciklusokat a prell-ezes lekezelesere csinaltam.
int8 readKey() {
    
    ROW1_Write(1u);
    char result = 0;
    for (uint8 i = 0;i < 20; i++) {
        result = COL1_Read() != 0 ? '1' : result;
        result = COL2_Read() != 0 ? '2' : result;
        result = COL3_Read() != 0 ? '3' : result;
        result = COL4_Read() != 0 ? 'A' : result;
        if (result != 0) {
            ROW1_Write(0u);
            return result;
        }
        CyDelayUs(50);
    }
    ROW1_Write(0u);
    
    ROW2_Write(1u);
    for (uint8 i = 0;i < 20; i++) {
        result = COL1_Read() != 0 ? '4' : result;
        result = COL2_Read() != 0 ? '5' : result;
        result = COL3_Read() != 0 ? '6' : result;
        result = COL4_Read() != 0 ? 'B' : result;
        if (result != 0) {
            ROW2_Write(0u);
            return result;
        }
        CyDelayUs(50);
    }
    ROW2_Write(0u);
    
    ROW3_Write(1u);
    for (uint8 i = 0;i < 20; i++) {
        result = COL1_Read() != 0 ? '7' : result;
        result = COL2_Read() != 0 ? '8' : result;
        result = COL3_Read() != 0 ? '9' : result;
        result = COL4_Read() != 0 ? 'C' : result;
        if (result != 0) {
            ROW3_Write(0u);
            return result;
        }
        CyDelayUs(50);
    }
    ROW3_Write(0u);
    
    ROW4_Write(1u);
    for (uint8 i = 0;i < 20; i++) {
        result = COL1_Read() != 0 ? '*' : result;
        result = COL2_Read() != 0 ? '0' : result;
        result = COL3_Read() != 0 ? '#' : result;
        result = COL4_Read() != 0 ? 'D' : result;
        if (result != 0) {
            ROW4_Write(0u);
            return result;
        }
        CyDelayUs(50);
    }
    ROW4_Write(0u);
    
  
    return result;   
}


/*
Csipogok egy adott frekvencian, egy adott ideig.
Olyat probalok csinalni, mint az arduinos tone().
*/
void tone(uint16 freq, int duration) {
//beallitom egy olyan periodust a PWM-nek, ami a freq
uint16 clk = 20000; 
// period*freq = 20000 
uint16 period = clk/freq;
PWM_BEEPER_STATUS;
PWM_BEEPER_WritePeriod(period);
PWM_BEEPER_WriteCompare(period/2);
PWM_BEEPER_Start();

//timer counter tick every TIMER_IT_MS ms. (50)
tone_deadline = counter+duration/TIMER_IT_MS;
beep_mode = 1u;
}


//50 ms-onkent
CY_ISR(TIMER_ISR)
{
 
 //mindig megnezem, hogy van e lenyomott billentyu.
 char pressed = readKey();
 if (pressed != 0 && waiting_for_relese == 0u) {
     currentlyPressedKey = pressed;
     waiting_for_relese = 1u;
 }
if (waiting_for_relese == 1u && pressed == 0) {
    waiting_for_relese = 0u;
}

//Ha eppen csipogok a tone() hasznalataval, akkor azt lealittom amennyiben tulleptuk a beallitot hataridot
if (beep_mode == 1u && tone_deadline < counter) {
    PWM_BEEPER_WriteCompare(0);
    PWM_BEEPER_Stop();
    beep_mode = 0u;
}

//todo
if (alarm_beep_mode == 1u && alarm_until < counter) {
    alarm_beep_mode = 0u;
}


//Kb. Percenkent frissitem az orat
if (actual_state == passziv) {
    if (counter % 1024 == 0) {
     //   LCD_ClearDisplay();
      //  LCD_Position(0u, 0u);
        char str[16];
        print_time_string(str);
        //LCD_PrintString(str);
    }
}
    
 Timer_ReadStatusRegister();
 counter ++;
  
}


uint16 minAmp = 30000;
uint16 maxAmp = 0;
uint32 lastCountRadar = 0;
/*
A radar AD konverterenek az interruptja
*/
CY_ISR(ADC_SAR_RADAR_ISR ) {
 
volatile uint16 AD_result = ADC_SAR_RADAR_GetResult16();

if (AD_result < minAmp) {
    minAmp = AD_result;
}  
if (AD_result > maxAmp) {
    maxAmp = AD_result;
}

if (counter != lastCountRadar) { //minden timer interruptra osszesitem az eredmenyeket /50ms-onkent./
   
    volatile int16 amp = maxAmp-minAmp;
    
   // char str[15];
   // LCD_ClearDisplay();
   // sprintf(str,"%lu : M: %d",counter, amp); 
   // LCD_PrintString(str); 
    
    if (amp > 3000) {
        //Mozgas erzekelve.
        if (actual_state == figyelo) {
            actual_substate = figylo_deaktivalo;
            sprintf(alart_reason, "RADAR");
            //TODO: csak akkor riasszon be, ha nem csak pl. egyetlen ilyen eszleles volt, hanem tobb is
            //mivel van esely ra, hogy az csak egy valahonnan egy felszedett zavar.

        }   
    }
    
   
 
   maxAmp = 0;
   minAmp = 30000;
   lastCountRadar = counter;
}

}

uint16 minLight = 9999;
uint16 maxLight = 0;
uint32 lastCntPr = 0;
/*
A fenyellenallas AD konverterenetk az interruptja
*/
CY_ISR(ADC_SAR_PR_ISR) {
   
    volatile uint16 AD_result =  ADC_SAR_PR_GetResult16();
    
    if (AD_result < minLight) {
    minLight = AD_result;
    }  
    if (AD_result > maxLight) {
    maxLight = AD_result;
    }
    
    //800 ms-onkent a fenyerossegnek megfeleloen allitom a hattervilagoitast, es jelzek, ha riasztanom kell
    if (counter != lastCntPr && counter % 16 == 0) {
        char str[30];
        sprintf(str,"%d. : PR val: %d\r\n",maxLight, minLight); 
        UART_PutString(str);  
      
        
        if (actual_state == figyelo && minLight < 100) {
            uint8 h = get_hour();
            if (h > NIGHT_FROM && h < NIGHT_TO) {
               actual_substate = figylo_deaktivalo;
               sprintf(alart_reason, "Fotorezisztor");
            }
        }
      
        if (maxLight > 2000) {
           PWM_BACKLIGHT_STATUS;
           uint16 test = 1+((maxLight-2000)/50);
           uint16 dutycle = test*20 > 350 ? 350 : test*20;
           PWM_BACKLIGHT_WriteCompare(dutycle);
        } 
        else {
            PWM_BACKLIGHT_STATUS;
            PWM_BACKLIGHT_WriteCompare(0u);
        }
        
        minLight = 9999;
        maxLight = 0;
        lastCntPr = counter;
    }
    
}

void init() {
    
    isr_Timer_StartEx(TIMER_ISR);
    ADC_SAR_RADAR_IRQ_StartEx(ADC_SAR_RADAR_ISR);
    ADC_SAR_RADAR_Start();
    ADC_SAR_RADAR_StartConvert();
    
    
    ADC_SAR_PR_IRQ_StartEx(ADC_SAR_PR_ISR);
    ADC_SAR_PR_Start();
    ADC_SAR_PR_StartConvert();
    
    Timer_Start();
    
    LCD_Start();
    LCD_Position(0u, 0u);
    
    SPIM_Start();
    SPIM_TxDisable();
    
    UART_Start();
}

char code_buffer[4];
uint8 code_buffer_ind = 0;

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

int main(void)
{
    init();
    CyGlobalIntEnable; /* Enable global interrupts. */
    
  // char str[16];
  // print_time_string(str);
  // LCD_PrintString(str);
    
   
    PWM_BACKLIGHT_Start();
    
    LCD_Position(1u, 0u);
    

    
    uint32 code_deadline = 2^31; // csak hogy veletlenusl sem riasszon be azonnal. 
    
    uint8 already_printed = 0u;
    
    for(;;)
    {      
        //allapotgep
        switch (actual_state) {
            case passziv:
                if (actual_substate == passziv_varakozo) { //varakozok, hogy a kod beirasara.
                    if (try_to_read_code_from_keyboard() == 1u) {
                        already_printed = 0u;
                        actual_substate = passziv_felodott;   
                    }
                 }
                else if (actual_substate == passziv_felodott) {
                   if (already_printed == 0u) {
                         LCD_ClearDisplay();
                         LCD_Position(0u, 0u);
                         LCD_PrintString("# - figyelo mod");
                         LCD_Position(1u, 0u);
                         LCD_PrintString("* - beallitasok");
                        
                         already_printed = 1u;
                    } 
                   if (currentlyPressedKey == '#') {
                        LCD_ClearDisplay();
                        LCD_PrintString("Figyelo mod");
                        //todo: varakozo mod
                        CyDelay(1000);
                        actual_state = figyelo;
                        actual_substate = figyelo_varakozo;
                        already_printed = 0u;
                       
                    }
                    else if (currentlyPressedKey == '*') {
                      //todo
                    }
                    currentlyPressedKey = 0;
                }
                else {
                //todo
                }
                break;
            case figyelo:
                if (actual_substate == figyelo_varakozo) {
                    //Ellenorzom a REED RELET
                    if (REED_RELAY_Read() == 0u) {
                        actual_substate = figylo_deaktivalo;
                        sprintf(alart_reason, "Reed rele");
                        currentlyPressedKey = 0;
                    }
                }
                else if (actual_substate == figylo_deaktivalo) {
                    if (try_to_read_code_from_keyboard() == 1u) {
                        actual_state = passziv;
                        actual_substate = passziv_felodott;
                        already_printed = 0u;
                    }
                    else {
                        if (already_printed == 0u) {
                            LCD_ClearDisplay();
                            LCD_Position(0u, 0u);
                            char first_row[16];
                            char timestr[5];
                            print_time_string(timestr);
                            sprintf(first_row, "%s - RIASZTAS", timestr);
                            char second_row[16];
                            sprintf(second_row, "OK: %s", alart_reason);
                            LCD_PrintString(first_row);
                            LCD_Position(1u, 0u);
                            code_deadline = counter+10000/TIMER_IT_MS;
                            LCD_PrintString(second_row);
                            already_printed = 1u;
                        }
                    }
                    if (counter > code_deadline) {
                        actual_state = riasztas;
                        already_printed = 0u;
                    }
                }
                break;
            case riasztas:
                if (already_printed == 0u) {
                    LCD_ClearDisplay();
                    LCD_Position(0u, 0u);
                    LCD_PrintString("RIASZTAS!!");
                    already_printed = 1u;
                    tone(700, 1000); //todo, hosszabban csipogni.
                    
                }
                if (try_to_read_code_from_keyboard() == 1u) {
                    actual_state = passziv;
                    actual_substate = passziv_felodott;
                    already_printed = 0u;
                }
                
                //todo: sms-ek, alallapotok
                break;
            default:
                break;
        }
        
        
        
        
    }
}

/* [] END OF FILE */

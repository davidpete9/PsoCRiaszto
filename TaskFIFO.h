/* ========================================
 *
 * Egy lancolt lista megvalositasa, ahol sorban, bizonyos idoelteltevel
 * egymast koveto taskokat lehet definialni.
 * Pl. Csipogasi szekvencia megvalositasa 
 * (pl. legyen 1000 Hz es 1000 ms-ig, utana 500ms-ig csond, majd 1400 Hz 300 ms-ig...)
 * 
 * Akkor hajtja vegre a task-ot, majd torli a taskot a fifo-bol, ha a szamlalo 0, kulonben mindig 1-gyel csokkenti.   
 *
 * Implementalt fuggvenyek: 
 * Get first item
 * Remove first item
 * Add item to the end
 *
 * ========================================
*/
#include "project.h"

typedef struct OneTimeTask {
    void (*func)(uint16);
    void (*funcnoparam)();
    uint16 delay;
    uint16 data;  //A fuggveny parametere
    struct OneTimeTask *next;
} OneTimeTask;

typedef struct TaskFIFO {
 OneTimeTask * start, *end;
} TaskFIFO;


OneTimeTask * create_new_onetimetask( void (*func)(uint16), uint16 delay, uint16 data);
OneTimeTask * create_new_noparam_onetimetask( void (*func)(), uint16 delay);

//A vegere beteszek egy taskot.
void pushTask(TaskFIFO* fifo, OneTimeTask* task);
//kiveszi az elso elemet. A hivo felelossege felszabaditani a memoriat.
OneTimeTask * get_and_remove_first(TaskFIFO* fifo);

//felszabaditja a teljes listat
void free_taskfifo(TaskFIFO * list);


//megnezi, hogy futtathato e mar a taszk, ha nem, akkor csokkenti a szamlalot
uint8 check_if_runnable_and_decrement_counter(TaskFIFO * fifo);

//Ezt fogom futtatni a timer interruptban. Ha sorban az elso taszknak a delay erteke 0
//akkor lefuttatom a fuggvenyt, es kiveszem a fifobol.
void run_next_task_when_available(TaskFIFO * fifo_tasks);


/* [] END OF FILE */

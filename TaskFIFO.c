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
    void (*func)(uint16*);
    uint16 delay;
    uint16 * data;  //A fuggveny parametere
    struct OneTimeTask *next;
} OneTimeTask;

typedef struct TaskFIFO {
 OneTimeTask * start, *end;
} TaskFIFO;


OneTimeTask * create_new_onetimetask( void (*func)(uint16*), uint16 delay, uint16 * data) {
    OneTimeTask * new_task;
    new_task = (OneTimeTask*)malloc(sizeof(OneTimeTask));
    new_task->delay = delay;
    new_task->func = func;
    new_task->data = data;
    return new_task;
}


//A vegere beteszek egy taskot.
void push(TaskFIFO* fifo, OneTimeTask* task) {
    if (fifo->end == NULL) {
        fifo->end = task;
        fifo->start = task;
    }
    else {
        fifo->end->next = task;
        fifo->end = task;
    }
}

//kiveszi az elso elemet. A hivo felelossege felszabaditani a memoriat.
void get_and_remove_first(TaskFIFO* fifo) {
    OneTimeTask* first = fifo->start;
    if (fifo->start == fifo->end) {
        fifo->start = NULL;
        fifo->end = NULL;
    } else {
        fifo->start = fifo->start->next;
    }
    return;
}

//felszabaditja a teljes listat
void free_taskfifo(TaskFIFO * list) {
    OneTimeTask * iter = list->start;
    while (iter != NULL) {
        OneTimeTask *nex = iter->next;
        free(iter);
        iter = nex;
    }
}

//megnezi, hogy futtathato e mar a taszk, ha nem, akkor csokkenti a szamlalot
uint8 check_if_runnable_and_decrement_counter(TaskFIFO * fifo) {
    if (fifo->start->delay == 0u)
        return 1u;
    
    fifo->start->delay--;
    return 0u;
}



/* [] END OF FILE */

/* ========================================
 *
 * Periodikusan futo, engedelyezheto taszkokat kezelo lancolt listat valositok meg.
 * Ezeken fogik mindig vegiegiteralni a timer IT-ben.
 *
 * ========================================
*/

#include "project.h"


//Periodikusan lefuto folyamat.
typedef struct periodic_task {
   void (*paramtask)(uint8*); //Lehet olyan, aminek egy flag parametere van
   void (*noparamtask)(); //Lehet olyan, aminek nincs parametere.
   uint16 period; //Ennyi idonkent fut le
   uint16 counter; //Ezt csokkenti minden iteracioban
   uint8 enabled; 
   uint8 pid; //azonosito
   uint8 * flag; //Ami a parameteres fuggvenynek a parametere lehet
   struct periodic_task * next; //lancolt lista kovetkezo elemet
} periodic_task;


typedef struct periodic_task_list {
    periodic_task * start;
    periodic_task * end;
} periodic_task_list;



periodic_task * create_new_ptask_parametered( void (*paramtask)(uint8*), uint8 * flag, uint16 period, uint8 pid) {
    periodic_task * new_task;
    new_task = (periodic_task*)malloc(sizeof(periodic_task));
    new_task->counter = 0;
    new_task->period = period;
    new_task->paramtask = paramtask;
    new_task->noparamtask = NULL;
    new_task->flag = flag;
    new_task->pid = pid;
    new_task->enabled = 1u;
    new_task->next = NULL;
    return new_task;
}

periodic_task * create_new_ptask_noparam( void (*noparamfunc)(), uint16 period, uint8 pid) {
    periodic_task * new_task;
    new_task = (periodic_task*)malloc(sizeof(periodic_task));
    new_task->counter = 0;
    new_task->period = period;
    new_task->noparamtask = noparamfunc;
    new_task->paramtask = NULL;
    new_task->pid = pid;
    new_task->next = NULL;
    new_task->enabled = 1u;
    return new_task;
}

void add_to_ptask_list(periodic_task_list *list, periodic_task * task) {
    if (list->start == NULL) {
        list->start = task;
        list->end = task;
        return;
    }
    
    list->end->next = task;
    list->end = task;
}

periodic_task * find_task(periodic_task_list * tasks, uint8 pid) {
    periodic_task *iter;
 
    for (iter = tasks->start; iter != NULL; iter = iter->next) {
        if (iter->pid == pid)
            return iter;
    }
    return NULL;
}

void enable_task(periodic_task_list * tasks, uint8 pid) {
   periodic_task * task = find_task(tasks, pid);
    if (task != NULL)
        task->enabled = 1u;
}

void disable_task(periodic_task_list * tasks, uint8 pid) {
   periodic_task * task = find_task(tasks, pid);
    if (task != NULL) {
        task->enabled = 0u;
        task->counter = task->period;
    }
        
}

void free_list(periodic_task_list * list) {
    periodic_task * iter = list->start;
    while (iter != NULL) {
        periodic_task *nex = iter->next;
        free(iter);
        iter = nex;
    }
}

void run_permitted_tasks_and_decrement_counters(periodic_task_list *list) {

    periodic_task *iter;
    for (iter = list->start; iter != NULL; iter = iter->next) {
        if (iter->enabled == 1u) {
            if (iter->counter == 0) {
                iter->counter = iter->period;
                if (iter->noparamtask != NULL) {
                    iter->noparamtask();
                }
                if (iter->paramtask != NULL) {
                    iter->paramtask(iter->flag);
                }
            }
            else {
                iter->counter--;
            }
        }
    }   
}






/* [] END OF FILE */

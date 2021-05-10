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



periodic_task * create_new_ptask_parametered( void (*paramtask)(uint8*), uint8 * flag, uint16 period, uint8 pid);

periodic_task * create_new_ptask_noparam( void (*noparamfunc)(), uint16 period, uint8 pid);

void add_to_ptask_list(periodic_task_list *list, periodic_task * task);

periodic_task * find_task(periodic_task_list * tasks, uint8 pid);

void enable_task(periodic_task_list * tasks, uint8 pid);

void disable_task(periodic_task_list * tasks, uint8 pid);

void free_list(periodic_task_list * list);

void run_permitted_tasks_and_decrement_counters(periodic_task_list *list);




/* [] END OF FILE */

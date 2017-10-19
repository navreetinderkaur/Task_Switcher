
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "common.h"



#define MAX_TASKS 4

int32_t TaskAdd(void (*f)(void *data), void *data);
int32_t TaskKill(int32_t id);
int32_t TaskCurrent(void);
int32_t TaskSwitcher(void);

typedef struct Task_s 
{
	void (*f)(void *data);               /* Task function */
	void *data;                         /* Private data pointer for this task */
}Task_t;

Task_t tasks[MAX_TASKS];            /* process list*/
int32_t currentTask;


                                                         
int32_t TaskAdd(void (*f)(void *data), void *data)          /*Try to find an empty slot */
{
int j;
	for(j=0; j< MAX_TASKS; j++) 
	{
		if(tasks[j].f == NULL) 
		{
			tasks[j].f    = f;
			tasks[j].data = data;
			return j;
		}
	}                              
/* No slots available, return -1*/
	return -1;
}

int32_t TaskKill(int32_t id)          /* killing the task*/
{
	tasks[id].f    = NULL;
	tasks[id].data = NULL;
        return 1;
}

int32_t TaskCurrent(void)
{
	return currentTask;
}


static int32_t TaskNext(void)         /* Find next task to run*/
{
	int32_t i;
	uint32_t count=0;
	i = currentTask;
	do 
	{
		i = (i + 1) % MAX_TASKS;count++;
	}while((tasks[i].f == NULL)&& (count <= MAX_TASKS));
	return (count <= MAX_TASKS) ? i : -1;
}

int32_t TaskSwitcher(void)
{
	if(currentTask < 0) 
	{
		return -1;
	}
	tasks[currentTask].f(tasks[currentTask].data);
	currentTask = TaskNext();
	return 0;
}



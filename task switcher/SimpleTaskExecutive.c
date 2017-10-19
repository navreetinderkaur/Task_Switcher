/**
  ******************************************************************************
  * File Name          : SimpleTaskExecutive.c
  * Description        : To add tasks, switch the tasks, kill any task by call and kill current task self
  * Modified on	       : March 6, 2017     
  * User               : Navreetinder Kaur
  *****************************************************************************
*/
#include <stdio.h>
#include "task switch.c"

// prototypes
void task1(void* data);
void task2(void* data);
void task3(void* data);
void task4(void* data);
void gpio();
void gpioPattern();
void ledOnOff();

// global varible declaration
const uint32_t GPIOpin[] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

int i;

// FUNCTION      : gpioPattern
// DESCRIPTION   : To blink led in pattern
// PARAMETERS    : NOTHING
// RETURNS       : NOTHING

void gpioPattern()                                                                              
{	      
  	      for(int pin=0; pin < 7; pin++){
 		   	HAL_GPIO_WritePin(GPIOE, GPIOpin[pin], GPIO_PIN_SET);		        //set GPIO pin
		        HAL_Delay(50);	
  	 		HAL_GPIO_WritePin(GPIOE, GPIOpin[pin] , GPIO_PIN_RESET);		//reset GPIO pin
 		        HAL_Delay(50);	
  		}

}

// FUNCTION      : gpioBlink
// DESCRIPTION   : To blink one led 
// PARAMETERS    : NOTHING
// RETURNS       : NOTHING

void gpioBlink()
{
	i=0;
        while(i<3){        
    		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);		                         //set GPIO pin
  	        HAL_Delay(10);	
    		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10 , GPIO_PIN_RESET);		                 //reset GPIO pin
                HAL_Delay(10);	
		i++;
	}
}

// FUNCTION      : gpioAll
// DESCRIPTION   : To on all leds at same time and off all leds at same time
// PARAMETERS    : NOTHING
// RETURNS       : NOTHING

void gpioAll()
{
  	      for(int pin=0; pin < 8; pin++){
 		   	HAL_GPIO_WritePin(GPIOE, GPIOpin[pin], GPIO_PIN_SET);		         //set GPIO pin
		        HAL_Delay(10);	 	 			
  		}
		
		for(int pin=0; pin < 8; pin++){
 		   	HAL_GPIO_WritePin(GPIOE, GPIOpin[pin], GPIO_PIN_RESET);		         //Reset GPIO pin
		        HAL_Delay(10);	
  		}
}

// FUNCTION      : task0
// DESCRIPTION   : To add task 0
// PARAMETERS    : data
// RETURNS       : NOTHING

void task0(void* data)
{
	printf("The task0 is %s\n", (char*) data);
	gpioPattern();
}

// FUNCTION      : task1
// DESCRIPTION   : To add task 1
// PARAMETERS    : data
// RETURNS       : NOTHING

void task1(void* data)
{
	printf("The task1 is %s\n", (char*) data);
	gpioBlink();
}

// FUNCTION      : task2
// DESCRIPTION   : To add task 2
// PARAMETERS    : data
// RETURNS       : NOTHING

void task2(void* data)
{
	printf("The task2 is %s\n", (char*) data);
        gpioAll();
}

// FUNCTION      : task0
// DESCRIPTION   : To add task 3
// PARAMETERS    : data
// RETURNS       : NOTHING

void task3(void* data)
{
	printf("The task3 is %s\n", (char*) data);
}

// FUNCTION      : CmdTaskAdd
// DESCRIPTION   : To add the tasks
// PARAMETERS    : mode
// RETURNS       : NOTHING

void CmdTaskAdd(int mode)
{
       
	if(mode != CMD_INTERACTIVE) {
		return;
  	}

	
	if(TaskAdd(task0, (void*) "led pattern") ==-1){                                // to check error condition
		printf("Error from Task 0\n");
	}	
	
	if(TaskAdd(task1, (void*) "led blink") ==-1){
		printf("Error from Task 1\n");
	}
	
	if(TaskAdd(task2, (void*) "led all") == -1){
        	printf("Error from Task 2\n");
	} 
	
	if(TaskAdd(task3, (void*) "navreet") == -1){
		printf("Error from Task 3\n");
	}	
	
	else{
		printf("The tasks are added\n");
	}
}

// FUNCTION      : CmdTaskTest
// DESCRIPTION   : To check whether the tasks are switching
// PARAMETERS    : mode
// RETURNS       : NOTHING

void CmdTaskTest(int mode)
{
	if(mode != CMD_INTERACTIVE) {
		return;
  	}
	
	for(int i = 0; i < 50; i++){
		TaskSwitcher();
	}

}

// FUNCTION      : CmdTaskKill
// DESCRIPTION   : To kill any task by call
// PARAMETERS    : mode
// RETURNS       : NOTHING

void CmdTaskKill (int mode)
{
	int rc;
	uint32_t value;

 	if(mode != CMD_INTERACTIVE) {
		return;
  	}

	rc = fetch_uint32_arg(&value);

	if(rc) {
		printf("Missing id\n");
		return;
  	}
	
	if (value > (MAX_TASKS-1)){                                       // to check error condition
		printf("the task not found\n");
        }
	
	else if (TaskKill(value)){       
		printf("the task %u is killed\n",(unsigned)value);		
	}
}

// FUNCTION      : CmdKillSelf
// DESCRIPTION   : To kill the current task self
// PARAMETERS    : mode
// RETURNS       : NOTHING

void CmdKillSelf(int mode)
{	
	int selfKill;
	if(mode != CMD_INTERACTIVE) {
		return;
  	}
	
	selfKill = TaskCurrent();               	                   // the current task is called here
					
	if(TaskKill(selfKill)){						   // to check whether the task to be killde is current task			
		printf("the current task is killed\n");
	}
}

ADD_CMD("taskadd", CmdTaskAdd,"                To add the tasks ")
ADD_CMD("tasktest", CmdTaskTest,"                To test the switcher")
ADD_CMD("taskkill", CmdTaskKill,"<id>            The switcher function implement")
ADD_CMD("selfkill", CmdKillSelf,"                kill the current taskself")


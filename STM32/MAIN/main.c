
#include "stm32_config.h"
#include "led.h"
#include "includes.h"  //ucos

/* 起始任务相关设置 */
//任务优先级
#define Start_Task_PRIO          10  //优先级最低

//任务堆栈大小
#define Start_STK_Size           64

//任务堆栈空间大小
OS_STK Start_Task_STK[Start_STK_Size];

//任务接口函数
void start_task(void *pdata);

// 任务堆栈空间大小
#define USER_TASK_STK_SIZE         64
OS_STK Task1_STK[USER_TASK_STK_SIZE];
void user_task1(void *pdata);

OS_STK Task2_STK[USER_TASK_STK_SIZE];
void user_task2(void *pdata);

OS_STK Task3_STK[USER_TASK_STK_SIZE];
void user_task3(void *pdata);

OS_STK Task4_STK[USER_TASK_STK_SIZE];
void user_task4(void *pdata);

OS_STK Task5_STK[USER_TASK_STK_SIZE];
void user_task5(void *pdata);


int main(void)
{
	OS_Heart_Init();  //初始化ucos心跳
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);  //初始化中断优先级
	
	LED_Init();  //初始化LED硬件接口

	printf("\r\n system hardware are all OK!\r\n");

	OSInit();  //初始化ucos
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&Start_Task_STK[Start_STK_Size-1], Start_Task_PRIO);  //创建起始任务
	OSStart();  //ucos启动
}

/******************************************************************************
* Function Name --> 开始任务
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void start_task(void * pdata)
{
	OS_CPU_SR cpu_sr=0;

	pdata = pdata;
	OS_ENTER_CRITICAL();  //进入临界区，无法被中断打断
	
	printf("\r\n qitas system run all task\r\n");
	
	OSTaskCreate(user_task1, (void *)0, (OS_STK *)&Task1_STK[USER_TASK_STK_SIZE-1], 4);
	OSTaskCreate(user_task2, (void *)0, (OS_STK *)&Task2_STK[USER_TASK_STK_SIZE-1], 5);
	OSTaskCreate(user_task3, (void *)0, (OS_STK *)&Task3_STK[USER_TASK_STK_SIZE-1], 6);
	OSTaskCreate(user_task4, (void *)0, (OS_STK *)&Task4_STK[USER_TASK_STK_SIZE-1], 7);
	OSTaskCreate(user_task5, (void *)0, (OS_STK *)&Task5_STK[USER_TASK_STK_SIZE-1], 8);
	
	OSTaskSuspend(Start_Task_PRIO);  //挂起起始任务
	OS_EXIT_CRITICAL();  //退出临界区，可以被中断打断
}
/******************************************************************************
* Function Name --> 任务1
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void user_task1(void *pdata)
{
	while(1)
	{
		LED0 = 0;
		delay_ms(1800);
		LED0 = 1;
		delay_ms(2000);
		
		printf("user_task1\n");
	}
}
/******************************************************************************
* Function Name --> 任务2
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void user_task2(void * pdata)
{
	while(1)
	{
		LED1 = 0;
		delay_ms(3000);
		LED1 = 1;
		delay_ms(2000);
		printf("user_task2\n");
	}
}

/******************************************************************************
* Function Name --> 任务3
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void user_task3(void * pdata)
{
	while(1)
	{
		LED1 = 0;
		delay_ms(6000);
		LED1 = 1;
		delay_ms(3000);
		printf("user_task3\n");
	}
}


/******************************************************************************
* Function Name --> 任务4
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void user_task4(void * pdata)
{
	while(1)
	{
		LED1 = 0;
		delay_ms(2000);
		LED1 = 1;
		delay_ms(5000);
		printf("user_task4\n");
	}
}

/******************************************************************************
* Function Name --> 任务5
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void user_task5(void * pdata)
{
	while(1)
	{
		LED1 = 0;
		delay_ms(3000);
		LED1 = 1;
		delay_ms(3000);
		printf("user_task5\n");
	}
}






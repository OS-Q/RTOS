
#include "stm32_config.h"
#include "led.h"
#include "includes.h"  //ucos

/* ��ʼ����������� */
//�������ȼ�
#define Start_Task_PRIO          10  //���ȼ����

//�����ջ��С
#define Start_STK_Size           64

//�����ջ�ռ��С
OS_STK Start_Task_STK[Start_STK_Size];

//����ӿں���
void start_task(void *pdata);

// �����ջ�ռ��С
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
	OS_Heart_Init();  //��ʼ��ucos����
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);  //��ʼ���ж����ȼ�
	
	LED_Init();  //��ʼ��LEDӲ���ӿ�

	printf("\r\n system hardware are all OK!\r\n");

	OSInit();  //��ʼ��ucos
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&Start_Task_STK[Start_STK_Size-1], Start_Task_PRIO);  //������ʼ����
	OSStart();  //ucos����
}

/******************************************************************************
* Function Name --> ��ʼ����
* Description   --> none
* Input         --> *pdata: ����ָ��
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void start_task(void * pdata)
{
	OS_CPU_SR cpu_sr=0;

	pdata = pdata;
	OS_ENTER_CRITICAL();  //�����ٽ������޷����жϴ��
	
	printf("\r\n qitas system run all task\r\n");
	
	OSTaskCreate(user_task1, (void *)0, (OS_STK *)&Task1_STK[USER_TASK_STK_SIZE-1], 4);
	OSTaskCreate(user_task2, (void *)0, (OS_STK *)&Task2_STK[USER_TASK_STK_SIZE-1], 5);
	OSTaskCreate(user_task3, (void *)0, (OS_STK *)&Task3_STK[USER_TASK_STK_SIZE-1], 6);
	OSTaskCreate(user_task4, (void *)0, (OS_STK *)&Task4_STK[USER_TASK_STK_SIZE-1], 7);
	OSTaskCreate(user_task5, (void *)0, (OS_STK *)&Task5_STK[USER_TASK_STK_SIZE-1], 8);
	
	OSTaskSuspend(Start_Task_PRIO);  //������ʼ����
	OS_EXIT_CRITICAL();  //�˳��ٽ��������Ա��жϴ��
}
/******************************************************************************
* Function Name --> ����1
* Description   --> none
* Input         --> *pdata: ����ָ��
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
* Function Name --> ����2
* Description   --> none
* Input         --> *pdata: ����ָ��
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
* Function Name --> ����3
* Description   --> none
* Input         --> *pdata: ����ָ��
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
* Function Name --> ����4
* Description   --> none
* Input         --> *pdata: ����ָ��
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
* Function Name --> ����5
* Description   --> none
* Input         --> *pdata: ����ָ��
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






//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "Key.h"
#include "Delay.h"
#include "LED.h"



/*
************************************************************
*	�������ƣ�	Key_Init
*
*	�������ܣ�	��������ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
u8 Flag;

void Key_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//��GPIOB��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//��ֹJTAG����
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;				//����Ϊ���
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	GPIO_Init(GPIOB, &gpio_initstruct);							//��ʼ��GPIO

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

void  EXTI1_IRQHandler(void)
{
	DelayXms(10);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		if(Flag == 0) Flag = 1;
		else Flag = 0;
		if(LED_info.LED_Status == LED_ON) LED_Set(LED_OFF);
		else LED_Set(LED_ON);

	}
	
	EXTI_ClearITPendingBit(EXTI_Line1);
}

//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "Key.h"
#include "Delay.h"
#include "LED.h"



/*
************************************************************
*	函数名称：	Key_Init
*
*	函数功能：	蜂鸣器初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
u8 Flag;

void Key_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//打开GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//禁止JTAG功能
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;				//设置为输出
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;						//将初始化的Pin脚
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//可承载的最大频率
	GPIO_Init(GPIOB, &gpio_initstruct);							//初始化GPIO

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

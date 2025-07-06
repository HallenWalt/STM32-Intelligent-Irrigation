#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "AD.h"

void AD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode 				 = ADC_Mode_Independent; 		//��ADģʽ
	ADC_InitStructure.ADC_DataAlign       	 = ADC_DataAlign_Right;			//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv	 = ADC_ExternalTrigConv_None;	//���ⲿ�ж�ת��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//����ת��
	ADC_InitStructure.ADC_ScanConvMode 		 = DISABLE;						//��ɨ��
	ADC_InitStructure.ADC_NbrOfChannel  	 = 1;							//ͨ����Ϊ1
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); 
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);

}

uint16_t AD_GetValue(void)                                                   //��ȡ����ʪ�ȴ���������ģ���ѹ
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}


uint8_t AD_GetSmcValue(void)                                                   //��ȡ����ʪ��
{
	u8 i;
	u32 Smcv;
	for(i=0;i<10; i++)
	{
		Smcv += AD_GetValue();
	}
	if(Smcv > 40000) Smcv = 40000;
	return (u8)(100-(Smcv/400));
}

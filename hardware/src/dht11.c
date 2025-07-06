#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"

#define DT GPIO_Pin_0
//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11_DQ_OUT(Bit_RESET); 	//����DQ

	DelayMs(20);    	//��������18ms
	DHT11_DQ_OUT(Bit_SET); 	//DQ=1 
	DelayUs(30);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//����DHT11�ӿ�����Ϊ����ģʽ	 
    while (!DHT11_DQ_IN&&retry<80)//DHT11������80us
	{
		retry++;
		DelayUs(1);
	} 
	if(retry >= 80)return 1;
	else retry=0;
    while (DHT11_DQ_IN&&retry<80)//DHT11���ͺ���ٴ�����80us
	{
		retry++;
		DelayUs(1);
	}
	if(retry >= 80)return 1;	    
	else return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(!DHT11_DQ_IN);//�ȴ��͵�ƽ50us
	
	while(DHT11_DQ_IN)//�жϸߵ�ƽʱ��
	{
		retry++;
		DelayUs(1);
	}
	if(retry < 27)return 0;
	else return 1;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else return 1;
	return 0;	    
}

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
 	GPIO_InitStructure.GPIO_Pin = DT;				 //PA0�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);				 //��ʼ��IO��
 	GPIO_SetBits(GPIOA,DT);						 //PA0 �����
			    
	DHT11_Rst();  //��λDHT11
	return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
} 


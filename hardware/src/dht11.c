#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"

#define DT GPIO_Pin_0
//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11_DQ_OUT(Bit_RESET); 	//拉低DQ

	DelayMs(20);    	//拉低至少18ms
	DHT11_DQ_OUT(Bit_SET); 	//DQ=1 
	DelayUs(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//设置DHT11接口引脚为输入模式	 
    while (!DHT11_DQ_IN&&retry<80)//DHT11会拉低80us
	{
		retry++;
		DelayUs(1);
	} 
	if(retry >= 80)return 1;
	else retry=0;
    while (DHT11_DQ_IN&&retry<80)//DHT11拉低后会再次拉高80us
	{
		retry++;
		DelayUs(1);
	}
	if(retry >= 80)return 1;	    
	else return 0;
}

//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(!DHT11_DQ_IN);//等待低电平50us
	
	while(DHT11_DQ_IN)//判断高电平时间
	{
		retry++;
		DelayUs(1);
	}
	if(retry < 27)return 0;
	else return 1;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
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

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
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

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
 	GPIO_InitStructure.GPIO_Pin = DT;				 //PA0端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);				 //初始化IO口
 	GPIO_SetBits(GPIOA,DT);						 //PA0 输出高
			    
	DHT11_Rst();  //复位DHT11
	return DHT11_Check();//等待DHT11的回应
} 


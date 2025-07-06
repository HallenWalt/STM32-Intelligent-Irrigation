//单片机头文件
#include "stm32f10x.h"
#include "stdio.h"
//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "LED.h"
#include "Key.h"
#include "dht11.h"
#include "OLED.h"
#include "AD.h"

//C库
#include <string.h>


#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

void Hardware_Init(void);
void Display_Init(void);
void Refresh_Data(void);
u8 humi,temp,Smc;
extern u8 Flag;
/*
************************************************************
*	函数名称：	main
*
*	函数功能：	
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/
int main(void)
{
	unsigned short timeCount = 0;	//发送间隔变量
	unsigned char *dataPtr = NULL;

	Hardware_Init();				//初始化外围硬件
	ESP8266_Init();					//初始化ESP8266
	
	OLED_Clear();					//OLED清屏
	OLED_ShowString(1,1,"Connect MQTTs Server..");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
	OLED_ShowString(3,1,"Connect MQTTs Success");
	DelayXms(500);	
	OLED_Clear();					//OLED清屏
	OLED_ShowString(1,1,"login onenet...");
	while(OneNet_DevLink())			//接入OneNET
	DelayXms(500);

	OneNET_Subscribe();				//Onenet下发订阅
	
	Display_Init();					//OLED屏显示初始化信息								
	
	while(1)
	{
		
		if(++timeCount >= 50)									//发送间隔5s
		{
			DHT11_Read_Data(&temp,&humi);                       //读取温湿度
			Smc= AD_GetSmcValue();                              //读取土壤湿度
			
			DelayMs(10);
		
			if(Flag == 0)										//判断是否有上位机或按键控制
			{
				if(Smc < 20) LED_Set(LED_ON);
				else LED_Set(LED_OFF);
			}
	
			OneNet_SendData();									//ONENET发送数据
			timeCount = 0;
			ESP8266_Clear();							        //ONENET清除数据
		}

		DelayXms(10);

			dataPtr = ESP8266_GetIPD(10);						//获取平台返回的数据
			if(dataPtr != NULL) OneNet_RevPro(dataPtr);			//平台返回数据检测                             

		Refresh_Data();											//更新OLED屏数据
		DelayMs(10);
	}

}	

/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

	Delay_Init();									//延时初始化
	
	Usart1_Init(115200);							//串口1，打印信息用
	
	Usart2_Init(115200);							//串口2，驱动ESP8266用
		
	LED_Init();										//蜂鸣器初始化
	
	Key_Init();                                     //初始化按键
	
	OLED_Init();									//初始化OLED  
	
	AD_Init();                                      //AD初始化（土壤湿度传感器）
	
	
	while(DHT11_Init())                             //DHT11初始化
	{
		OLED_ShowString(1,1,"DHT11 Error");
		DelayMs(1000);
	}
	
		OLED_Clear();
		OLED_ShowString(1,1,"Hardware init OK");
		DelayMs(1000);
}

void Display_Init(void)
{
		OLED_Clear();
		OLED_ShowChinese(1,1,1);//温
		OLED_ShowChinese(1,2,2);//度
		OLED_ShowChinese(1,3,0);//：
		OLED_ShowChinese(1,5,3);//℃
	              
			      
		OLED_ShowChinese(2,1,4);//湿
		OLED_ShowChinese(2,2,5);//度
	  	OLED_ShowChinese(2,3,0);//：
		OLED_ShowChinese(2,5,6);//%
                  
    	OLED_ShowChinese(3,1,10);//土
		OLED_ShowChinese(3,2,11);//壤
		OLED_ShowChinese(3,3,4);//湿
		OLED_ShowChinese(3,4,5);//度
	  	OLED_ShowChinese(3,5,0);//：
		OLED_ShowChinese(3,7,6);//%
	
		OLED_ShowChinese(4,1,12);//水
		OLED_ShowChinese(4,2,13);//泵
		OLED_ShowChinese(4,3,0);//：

}

void Refresh_Data(void)
{
	OLED_ShowNum(1,7,temp,2);//温度	
	OLED_ShowNum(2,7,humi,2);//湿度	
	OLED_ShowNum(3,11,Smc,2); //土壤湿度
	if(LED_info.LED_Status == Bit_SET)
	OLED_ShowChinese(4,4,8);//开
	else
	OLED_ShowChinese(4,4,9);//关		

}



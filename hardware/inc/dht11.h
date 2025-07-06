#ifndef DHT11_H
#define DHT11_H

extern int X;

/*PA0 -- DHT11 DATA_COM*/
#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8;} //设置GPIOA_Pin0为上拉/下拉输入模式
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3;} //设置GPIOA_Pin0为通用推挽输出模式，引脚最大传输速度为50MHz
//IO操作函数   
#define	DHT11_DQ_OUT(X)  GPIO_WriteBit(GPIOA, DT, X)           //设置DHT11输出高低电平
#define	DHT11_DQ_IN  GPIO_ReadInputDataBit(GPIOA, DT)          //读取GPIOA_Pin0引脚的电压

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取数据
u8 DHT11_Read_Byte(void);//读取一个字节
u8 DHT11_Read_Bit(void);//读取一位
u8 DHT11_Check(void);//检测DHT11
void DHT11_Rst(void);//复位DHT11   

#endif

#ifndef DHT11_H
#define DHT11_H

extern int X;

/*PA0 -- DHT11 DATA_COM*/
#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8;} //����GPIOA_Pin0Ϊ����/��������ģʽ
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3;} //����GPIOA_Pin0Ϊͨ���������ģʽ������������ٶ�Ϊ50MHz
//IO��������   
#define	DHT11_DQ_OUT(X)  GPIO_WriteBit(GPIOA, DT, X)           //����DHT11����ߵ͵�ƽ
#define	DHT11_DQ_IN  GPIO_ReadInputDataBit(GPIOA, DT)          //��ȡGPIOA_Pin0���ŵĵ�ѹ

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ����
u8 DHT11_Read_Byte(void);//��ȡһ���ֽ�
u8 DHT11_Read_Bit(void);//��ȡһλ
u8 DHT11_Check(void);//���DHT11
void DHT11_Rst(void);//��λDHT11   

#endif

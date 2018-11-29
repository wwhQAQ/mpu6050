#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f10x.h"

void IIC_Init(void);//IIC初始化
void IIC_Start(void);//IIC起始信号
void IIC_Stop(void);//IIC结束信号
u8 IIC_Wait_Ack(void);//读取应答信号
void IIC_Send_Byte(u8 txd);//通过IIC写一字节数据
u8 IIC_Read_Byte(u8 ack);    //读取IIC一个字节的数据
void IIC_NAck(void);
void IIC_Ack(void);

//IO方向设置
void SDA_OUT(void); //配置sda线为输出
void SDA_IN(void);//配置sda线为输入

//IO操作设置
void WRITE_SDA(u8 dat);		//控制SDA输出高低电平
void WRITE_SCL(u8 dat);		//控制SCL输出高低电平
u8 	 READ_SDA(void);		//读取SDA的电平


#endif


#include "stm32f10x.h"
#include "usart.h"
#include "mpu6050.h"
#include "delay.h"
#include "inv_mpu.h"

void niming_report(u8 fun,u8 *data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	send_buf[0]=0x88;			//帧头
	send_buf[1]=fun;			//功能字
	send_buf[2]=len;			//有效数据长度
	send_buf[len+3]=0;
	for(i=0;i<len;i++)
	{
		send_buf[3+i]=data[i];
	}
	for(i=0;i<len+3;i++)
	{
		send_buf[len+3]+=send_buf[i];
	}
	for(i=0;i<len+4;i++)
	{
		usart1_send(send_buf[i]);
	}	
}

void data_change_nimingfly(int x,int y,int z,u8 *dat0)
{
	u8 i;
	for(i=0;i<28;i++)
	{
		dat0[i]=0;
	}
	dat0[18]=x>>8;
	dat0[19]=x;
	dat0[20]=y>>8;
	dat0[21]=y;
	dat0[22]=z>>8;
	dat0[23]=z;
}
	

int main(void)
{
	u8 dat[28];
	float roll,pitch,yaw;
	delay_init();
	usart_init(115200);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_ms(500);
	usart1_send(0x01);
	MPU6050_Init();	
	usart1_send(0x02);
	delay_ms(500);
	while(mpu_dmp_init())
	{
		usart1_send(0x00);
	}
	while(1)
	{
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		data_change_nimingfly((int)(roll*100),(int)(pitch*100),(int)(yaw*10),dat);
		niming_report(0xAF,dat,28);		
	}
}

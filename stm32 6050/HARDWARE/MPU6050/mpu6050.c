#include "mpu6050.h"
#include "delay.h"

void MPU6050_Init(void)
{
	u8 res;
	IIC_Init();							//1.初始化IIC。
	
	delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG1ADDR,0x80);  //将mpu6050复位。
	delay_ms(100);
	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG1ADDR,0x00);  //将mpu6050唤醒。
	delay_ms(100);
	delay_ms(100);
	
	GYRO_Range(3);						
		delay_ms(100);
	
	ACC_Range(0);
		delay_ms(100);
	
	MPU6050_Frequency(50);								//4.其他设置 ，设置陀螺仪采样频率为50HZ
		delay_ms(100);
	


	
	MPU6050_Write_OneByte(MPU6050ADDR,INTREGADDR,0x00);//设置为关闭所有中断。
		delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,AUXIICREGADDR,0X00);//关闭AUX2寄存器的IIC2通道
		delay_ms(100);

MPU6050_Write_OneByte(MPU6050ADDR,FIFOREGADDR,0x00);//设置为关闭所有FIFO。
		delay_ms(100);


	
	MPU6050_Write_OneByte(MPU6050ADDR,INTBPREGADDR,0x80);//设置INT低电平有效
		delay_ms(100);
		
	res=MPU6050_Read_OneByte(MPU6050ADDR,DEVICEID);
	if(res==MPU6050ADDR)
	{

	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG1ADDR,0x01);					//5.配置系统时钟源,使用PLL，X轴陀螺仪做参考。关闭温度传感器.
			delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG2ADDR,0x00);					//使能ACC和GYRO传感器
			delay_ms(100);
		
			MPU6050_Frequency(50);
		delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,DIGREGADDR,0x04);	//数字低通滤波器
		delay_ms(100);
	
	}
	

}










/*设置MPU6050的采样频率*/
void MPU6050_Frequency(u8 xx)
{	
	u8 data;
	data=1000/xx-1;
	MPU6050_Write_OneByte(MPU6050ADDR,FREQREGADDR,data);
}


/*设置陀螺仪的量程。xx可以为0,1,2,3。分别对应+-250,500,1000,2000的量程*/
void GYRO_Range(u8 xx)
{
	MPU6050_Write_OneByte(MPU6050ADDR,GYROREGADDR,xx<<3);
}

/*设置加速度计的量程。xx可以为0,1,2,3。分别对应+-2g,4g,8g,16g的量程*/
void ACC_Range(u8 xx)
{
	MPU6050_Write_OneByte(MPU6050ADDR,ACCREGADDR,xx<<3);
}

/*MPU6050写入一个字节*/
void MPU6050_Write_OneByte(u8 addr,u8 regaddr,u8 dat)
{
	IIC_Start();
	IIC_Send_Byte(addr<<1|0);
	IIC_Wait_Ack();
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*MPU6050读取一个字节*/
u8 MPU6050_Read_OneByte(u8 addr,u8 regaddr)
{
	u8 tempdat;
	IIC_Start();
	IIC_Send_Byte(addr<<1|0);
	IIC_Wait_Ack();
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((addr<<1)|1);
	IIC_Wait_Ack();
	tempdat=IIC_Read_Byte(0);
	IIC_Stop();
	return tempdat;
}
















u8 MPU6050_Read_Len(u8 addr,u8 regaddr,u8 len,u8 *buf)
{	
	IIC_Start();
	IIC_Send_Byte(addr<<1);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}	
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((addr<<1)+1);
	IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
		{
			*buf=IIC_Read_Byte(0);			//读数据，并且不再响应
		}
		else
		{
			*buf=IIC_Read_Byte(1);
		}
		len--;
		buf++;
	}
	IIC_Stop();
	return 0;
}


u8 MPU6050_Write_Len(u8 addr,u8 regaddr,u8 len,u8 *buf)
{
	u8 i;
	IIC_Start();
	IIC_Send_Byte(addr<<1);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}	
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);		
		if(IIC_Wait_Ack())
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_Stop();
	return 0;
}




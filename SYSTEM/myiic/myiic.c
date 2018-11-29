#include "myiic.h"
#include "delay.h"

/*IIC初始化函数*/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );//PB 时钟使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);   //PB10,PB11 输出高
}

/*IIC起始信号*/
void IIC_Start(void)									//好像改动过
{
	SDA_OUT();				//配置SDA为输出；
	WRITE_SDA(1);			//SDA输出高电平
	WRITE_SCL(1);			//SCL输出高电平
	delay_us(2);			//延迟2微秒
	WRITE_SDA(0);			//SDA向低电平跳变
	delay_us(2);			//延迟2微秒
	WRITE_SCL(0);			//SCL变为低电平，准备发送数据	
}

/*IIC结束信号*/
void IIC_Stop(void)												//好像改动过
{
	SDA_OUT();				//配置SDA为输出
	WRITE_SCL(0);			//SCL为低电平
	WRITE_SDA(0);			//SDA为低电平
	delay_us(2);			
	WRITE_SCL(1);
	WRITE_SDA (1);			//SDA向高电平跳变，完成数据传送
	delay_us(2);
}

/*IIC等待应答信号*/
u8 IIC_Wait_Ack(void)
{
	u8 error=255;					//配置等待超时时间
	SDA_OUT();
	WRITE_SCL(1);
	delay_us(2);
	WRITE_SDA(1);
	delay_us(2);
	SDA_IN();						//配置SDA为输入，准备接受低电平
	while(READ_SDA()&(error>0))		//判断有无应答信号
	{
		error--;
	}
	if(error==0)					//超时，返回1
	{
		IIC_Stop();
		return 1;
	}
	else							//有应答信号，返回0
	{	
		WRITE_SCL(0);					//时钟输出0	
		return 0;
	}
}

void IIC_Send_Byte(u8 txd)
{
	u8 i=0;
	SDA_OUT();
	WRITE_SCL(0);				//拉低时钟，开始传输数据
	for(i=0;i<8;i++)
	{
		WRITE_SDA((txd&0x80)>>7);
		txd<<=1;
		WRITE_SCL(1);
		delay_us(2);
		WRITE_SCL(0);
		delay_us(2);			
	}
}


u8 IIC_Read_Byte(u8 ack)			//ack为0 不回应，为1回应
{
	u8 i,receivedat=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		WRITE_SCL(0);			//拉低时钟
		delay_us(2);
		WRITE_SCL(1);			//拉高时钟
		receivedat<<=1;
		if(READ_SDA()==1)
		{
			receivedat++;
		}
	}
	if(!ack)								//ack为0 不应带
		IIC_NAck();
	else
		IIC_Ack();							//为1，应答。
	return receivedat;
}

/*不产生应答信号*/
void IIC_NAck(void)
{
	WRITE_SCL(0);
	SDA_OUT();
	WRITE_SDA(1);
	delay_us(2);
	WRITE_SCL(1);
	delay_us(2);
	WRITE_SCL(0);
}

/*产生应答信号*/
void IIC_Ack(void)
{
	WRITE_SCL(0);
	SDA_OUT();
	WRITE_SDA(0);
	delay_us(2);
	WRITE_SCL(1);
	delay_us(2);
	WRITE_SCL(0);
}

/*设置SDA为输出*/
void SDA_OUT(void)										//将SDA设置为输出；
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 				//初始化 GPIO;
}

/*设置SDA为输入*/
void SDA_IN(void)										//将SDA设置为输入；
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 				//初始化 GPIO;
}

/*设置SDA为输出高低电平*/
void WRITE_SDA(u8 dat)
{
	switch(dat)
	{
		case 1:
		GPIO_SetBits(GPIOB,GPIO_Pin_7);break;
		case 0:	
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);break;
	}
}

/*设置SCL为输出高低电平*/
void WRITE_SCL(u8 dat)
{
	switch(dat)
	{
		case 1:
		GPIO_SetBits(GPIOB,GPIO_Pin_6);break;
		case 0:	
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);break;
	}
}

/*读取SCL电平*/
u8 READ_SDA(void)
{	
	return 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
}






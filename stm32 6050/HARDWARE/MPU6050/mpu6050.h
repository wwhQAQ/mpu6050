#ifndef __MPU6050_H
#define __MPU6050_H
#include "myiic.h"

/*MPU6050µØÖ·*/
#define MPU6050ADDR 	0x68

/*¿ØÖÆ¼Ä´æÆ÷*/
#define GYROREGADDR 	0x1B
#define ACCREGADDR  	0x1C
#define POWERREG1ADDR   0x6B
#define POWERREG2ADDR   0x6C
#define AUXIICREGADDR   0x6A
#define INTREGADDR		0x38
#define FIFOREGADDR		0x23
#define FREQREGADDR		0x19
#define DIGREGADDR 		0x1A
#define INTBPREGADDR	0x37

/*Êý¾Ý¶ÁÈ¡¼Ä´æÆ÷*/
#define ACCDATXH		0x3B	
#define ACCDATXL		0x3C	
#define ACCDATYH		0x3D	
#define ACCDATYL		0x3E	
#define ACCDATZH		0x3F	
#define ACCDATZL		0x40	
#define TDATH			0x41
#define TDATL			0x42
#define GYRODATXH       0x43
#define GYRODATXL       0x44
#define GYRODATYH       0x45
#define GYRODATYL       0x46
#define GYRODATZH       0x47
#define GYRODATZL       0x48

#define DEVICEID        0X75	

void MPU6050_Init(void);
void GYRO_Range(u8 xx);
void ACC_Range(u8 xx);
void MPU6050_Frequency(u8 xx);


u8 MPU6050_Read_OneByte(u8 addr,u8 regaddr);
void MPU6050_Write_OneByte(u8 addr,u8 regaddr,u8 dat);
u8 MPU6050_Read_Len(u8 addr,u8 regaddr,u8 len,u8 *buf);
u8 MPU6050_Write_Len(u8 addr,u8 regaddr,u8 len,u8 *buf);
u8 MPU6050_Get_GyroX(void);

				


#endif


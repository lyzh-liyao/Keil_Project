#include <Prohead.h>
#include <Driver.h>
#include <stm32f10x.h>
#include <Type.h>  
#include <NRF24L01_Driver.h>
#include <mpu6050_driver.h>
#include <System_Init.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>
#include <NRF24L01_Driver_send.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include"math.h"
/*
********************************************************************************
** 函数名称 ： main(void)
** 函数功能 ： 主函数
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
端口占用：

定时器： A0|A1|A2|A3
串口1：	 A9|A10
模拟spi：B6|B7
LED：    B10

NRF24L01_SPI2_SCK:	PB13
NRF24L01_SPI2_MISO:	PB14
NRF24L01_SPI2_MOSI:	PB15
NRF24L01_IRQ:				PC4
NRF24L01_CE:				PC5
NRF24L01_CSN:				PC6


2_NRF24L01_SPI1_SCK:	PA5
2_NRF24L01_SPI1_MISO:	PA6
2_NRF24L01_SPI1_MOSI:	PA7
2_NRF24L01_IRQ:				PC7
2_NRF24L01_CE:				PC8
2_NRF24L01_CSN:				PC9


*********************************************************************************/
#define q30  1073741824.0f
char* runState = "同步";
static signed char gyro_orientation[9] = {-1,0,0,0,-1,0,0,0,1};
//四元数 begin
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
char num[50];
float Pitch,Roll,Yaw;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
//四元数 end

//DMP begin
#define DEFAULT_MPU_HZ  (100)
int result;     
static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}
static  unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}
static void run_self_test(void)
{
    int result;
//    char test_packet[4] = {0};
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		printf("setting bias succesfully ......\n");
    }
	else
	{
		printf("bias has not been modified ......\n");
	}
}
//DMP end
int main(void)
{	
	system_init();
	//u8 Tx_Buf[32]={"亲爱的 我爱你！"};
	//RX_Mode(0);//配置接收通道0
	//RX_Mode(4);
	while(1){
		
		Delay_MS(1000);				 //延时
// 		if(equals((u8*)"同步",(u8*)runState,4)){
// 			printf("接收工作正常；");
// 			runState="";
// 		}else{
// 			printf("接收工作失败；");
// 		}
		//
		//nRF24L01_TxPacket_send(Tx_Buf);
		
		//printf("产生开始");
		//EXTI_GenerateSWInterrupt(EXTI_Line4);//软件产生一个中断，中断线4
		//printf("产生完成");
		//RX_Mode(0);//配置接收通道0
		//RX_Mode(4);//配置接收通道4
	
		//Build_Data();
		//printf("\n我的main123");
		//printf("%s",temData2);
		//USART1_SendData(direct);
		//printf("2710:%d",0x2710);

	result = mpu_init();
	printf("result:%d",result);
	if(!result)
  {
	  //mpu_init();
	  printf("mpu initialization complete......\n ");
	  //mpu_set_sensor
	  if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	  {
	  	 printf("mpu_set_sensor complete ......\n");
	  }
	  else
	  {
	  	 printf("mpu_set_sensor come across error ......\n");
	  }
	  //mpu_configure_fifo
	  if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	  {
	  	 printf("mpu_configure_fifo complete ......\n");
	  }
	  else
	  {
	  	 printf("mpu_configure_fifo come across error ......\n");
	  }
	  //mpu_set_sample_rate
	  if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))
	  {
	  	 printf("mpu_set_sample_rate complete ......\n");
	  }
	  else
	  {
	  	 printf("mpu_set_sample_rate error ......\n");
	  }
	  //dmp_load_motion_driver_firmvare
	  if(!dmp_load_motion_driver_firmware())
	  {
	  	printf("dmp_load_motion_driver_firmware complete ......\n");
	  }
	  else
	  {
	  	printf("dmp_load_motion_driver_firmware come across error ......\n");
	  }
	  //dmp_set_orientation
	  if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
	  {
	  	 printf("dmp_set_orientation complete ......\n");
	  }
	  else
	  {
	  	 printf("dmp_set_orientation come across error ......\n");
	  }
	  //dmp_enable_feature
	  if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
	        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
	        DMP_FEATURE_GYRO_CAL))
	  {
	  	 printf("dmp_enable_feature complete ......\n");
	  }
	  else
	  {
	  	 printf("dmp_enable_feature come across error ......\n");
	  }
	  //dmp_set_fifo_rate
	  if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
	  {
	  	 printf("dmp_set_fifo_rate complete ......\n");
	  }
	  else
	  {
	  	 printf("dmp_set_fifo_rate come across error ......\n");
	  }
	  run_self_test();
	  if(!mpu_set_dmp_state(1))
	  {
	  	 printf("mpu_set_dmp_state complete ......\n");
	  }
	  else
	  {
	  	 printf("mpu_set_dmp_state come across error ......\n");
	  }
  }
  	while(1) 
	{
	
	 dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,
                &more);	 
	 /* Gyro and accel data are written to the FIFO by the DMP in chip
     * frame and hardware units. This behavior is convenient because it
     * keeps the gyro and accel outputs of dmp_read_fifo and
     * mpu_read_fifo consistent.
     */
/*     if (sensors & INV_XYZ_GYRO )
       send_packet(PACKET_TYPE_GYRO, gyro);
     if (sensors & INV_XYZ_ACCEL)
        send_packet(PACKET_TYPE_ACCEL, accel); */
     /* Unlike gyro and accel, quaternions are written to the FIFO in
     * the body frame, q30. The orientation is set by the scalar passed
     * to dmp_set_orientation during initialization.
     */
     if (sensors & INV_WXYZ_QUAT )
	 {
	 	 q0=quat[0] / q30;	
		 q1=quat[1] / q30;
		 q2=quat[2] / q30;
		 q3=quat[3] / q30;
		 Pitch  = asin(2 * q1 * q3 - 2 * q0* q2)* 57.3; // pitch
    	 Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
		 Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
//		sprintf(num,"%f",q0);//	in Calculating quaternion steps.....
//		sprintf(num,"%f",q1);
		 printf("%f,%f,%f",Pitch,Roll,Yaw);	
	    printf(num);
	    printf("\n");
//		UART1_ReportIMU(Yaw*10, Pitch*10, Roll*10,0,0,0,100);
	 }
	 



//		delay_ms(500);	
//		LCD_ShowNum(30,70,sca610(0,5),6,16); 
//		Hanzi(50,110,BLUE,0,4,1);
	}






	}
}

/*************结束***************/


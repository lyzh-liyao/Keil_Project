#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__
#include "stm32f10x.h"
#include "STM32_DevInit.h"
#include "protocol.h"

#define MAX_ENC	160	//µ¥Î»Ö®¼äÄÚ±àÂëÆ÷×î´ó±ä»¯Á¿
#define ENC_MM 1
#define INIT_ENC 0x7FFF
#define RING_ENC 3000

typedef enum{ MOTOR_L, MOTOR_R }MOTOR_ID;	//×óÓÒµç»úID
typedef enum{ MOTOR_STOP = 0x0 ,MOTOR_UP = 0x1, MOTOR_DOWN = 0x2 }MOTOR_DIR;//µç»úÔË¶¯·½Ï
//Ö¸ÁîÀàĞÍ:¿ÕÖ¸Áî£¬Ö±ÏßÔË¶¯£¬Ö¸¶¨ÔË¶¯£¬½Ç¶ÈÔË¶¯£¬»¡ÏßÔË¶¯
typedef enum{ NONE, STRAIGHT, CUSTOM, ANGLE, ARC}CMD_TYPE;

typedef struct _Motor_T Motor_T;
struct _Motor_T{
	MOTOR_ID Id;
	TIM_TypeDef* PWM_TIMx;	//PWMÊä³ö¶¨Ê±Æ÷
	TIM_TypeDef* ENC_TIMx;	//±àÂëÆ÷²¶»ñ¶¨Ê±Æ÷
	uint8_t TIM_Channel;		//PWM¶¨Ê±Æ÷Í¨µÀ
	GPIO_TypeDef* GPIOx;		//±àÂëÆ÷GPIO×é
	uint16_t INA_GPIO_Pinx;	//±àÂëÆ÷AÏîIO Pin
	uint16_t INB_GPIO_Pinx;	//±àÂëÆ÷BÏîIO Pin
	uint8_t Overflow_Flag;	//±àÂëÒç³ö±êÖ¾Î»£¨¶¨Ê±Æ÷¼ÆÊıÖĞ¶Ï£©
	PROTOCOL_INFO_T 	Exec_Protocol;//ÕıÔÚÖ´ĞĞµÄĞ­Òé
	CMD_TYPE Cmd_Type;			//Ö¸ÁîÀàĞÍ   Ö±Ïß£¬Ô­µØ×ª£¬»¡ÏßÔË¶¯
	
	float Speed_Enc;				//×î´óÔÊĞíËÙ¶È·Ö³É0-100µÈ·Ö   *£¨0-100£©=ÕæÊµËÙ¶ÈÖ¸Áî
	PID_T PID_Speed;				//ËÙ¶È»·
	PID_T PID_Location;			//Î»ÖÃ»·
	uint16_t Encoder_Cur;		//µ±Ç°±àÂëÆ÷Öµ
	uint16_t Encoder_Last;	//ÉÏ´Î±àÂëÆ÷Öµ
	int32_t Encoder_Diff;		//Ä¿±ê±àÂëÖµÓëµ±Ç°±àÂëÖµµÄ²î
	int32_t Location_Diff;	//×óÓÒµç»ú±àÂëÖµµÄ²î
	
	int16_t Speed;					//¼´Ê±ËÙ¶È
	int16_t SpeedCMD;				//ÃüÁîËÙ¶È
	//uint16_t Encoder_Begin;
	uint16_t Encoder_CMD;
	//uint16_t Encoder_CMD_Diff;
	float Enc_Angle_Code;
	int16_t SpeedBAK;				//ÓÃÓÚÔİÍ£ÔË¶¯Ê±»Ö¸´ÔË¶¯ËÙ¶È
	int16_t Out_PWM;				//×îÖÕÊä³öPWM
	MOTOR_DIR Dir;					//µç»úµ±Ç°×ªÏò£¨±àÂëÆ÷¶¯Ì¬Öµ£©
	MOTOR_DIR DirCMD;				//ÃüÁî·½Ïò
	MOTOR_DIR DirBAK;				//ÓÃÓÚÔİÍ£ÔË¶¯Ê±»Ö¸´ÔË¶¯·½Ïò
	uint8_t IsOpposite;			//ÊÇ·ñ·´Ïò

	
	void (*Motor_Run)(Motor_T* motor, MOTOR_DIR dir, uint16_t speed);
	void (*Motor_Custom)(Motor_T* motor, MOTOR_DIR dir, uint16_t speed, int16_t Encoder);
	uint16_t (*Motor_Get_Encoder)(Motor_T* motor);
	
};

extern Motor_T *motor_L, *motor_R;
extern void Motor_Init(void); 
extern void Motor_GPIO_Configuration(void);
extern void Motor_TIM_Configuration(void);
extern void Motor_PID(void);

#endif


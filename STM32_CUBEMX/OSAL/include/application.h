#ifndef APPLICATION_H
#define APPLICATION_H

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "osal.h"
#include "osal_timer.h"
#include "osal_event.h"
#include "osal_memory.h"
#include "osal_msg.h"

//全局变量声明
/*****************************************************************************/

typedef struct
{
	osal_event_hdr_t  hdr;          //操作系统事件结构
	unsigned char *Data;			//命令帧操作数
} General_SerialData_t;             //OSAL中通用串口打印消息的格式结构体

/**********************ADC CDD field start*************************************/
typedef enum{
  BATTERY = 0,
  DIR_MOTOR,
  REAR_MOTOR,
  FRONT_MOTOR,
  ADC_CHANNEL_NUM,
}ADC_CHANNEL_n;

typedef enum{
  INVALID = 0,
  VALID,
}ADCSTATUS_t;

typedef  struct{
  uint32_t adcvalue[ADC_CHANNEL_NUM];
  uint16_t Device_iv[ADC_CHANNEL_NUM];
  ADCSTATUS_t ADC_status;
}ADC_t;

extern ADC_t ADCData;
extern HAL_StatusTypeDef Start_ADC_Scan(void);
extern uint8_t ADC2Voltage_Handle(ADC_t *data);
/**********************ADC CDD field end*************************************/

/**********************MOTOR CDD field start*************************************/
typedef enum{
  NORMAL = 0,
  OVERLOAD ,
  LOCK,
  MERROR,
  MSTATUS_NUM,
}MOTOR_STATUS_n;

typedef enum{
  INIT = 0,
  POSITIVE ,
  NAGETIVE ,
  MDIR_NUM,
}MOTOR_DIR_n;

typedef enum{
  DINIT = 0,
  CENTER ,
  RIGHT ,
  LEFT ,
  DMDIR_NUM,
}DMOTOR_DIR_n;

typedef struct{
  MOTOR_STATUS_n status;
  MOTOR_DIR_n direction[2];
  uint8 MSpeed;
  uint8_t waitflag;
}Driver_Motor_t;

typedef struct{
  MOTOR_STATUS_n status;
  DMOTOR_DIR_n direction[2];
  uint8_t waitflag;
}Dir_Motor_t;

extern Driver_Motor_t FrontMotor;
extern Driver_Motor_t RearMotor;
extern Dir_Motor_t  DirMotor;

extern void Motor_Control_Init(void);
extern uint8_t Motor_Control_mainfunction(void);
extern uint8_t Set_Driver_M(MOTOR_DIR_n direction,uint8_t speed);
extern uint8_t Set_Dir_M(DMOTOR_DIR_n direction);
extern void Test_Motor_Driver(void);
/**********************MOTOR CDD field end*************************************/

/**********************WIRELESS CDD field start*************************************/
#define MESLENGTH   13
#define RXBUFSIZE   (MESLENGTH)
#define HEADMASK1   0xAA
#define HEADMASK2   0xBB
#define TAILMASK1   0xCC
#define TAILMASK2   0xDD
#define ROCKERMES   0x01
#define KEYMES      0x02
//HB HB ROCKERMES R_rocker_X R_rocker_Y L_rocker_X L_rocker_Y TB TB
//HB HB KEYMES R_UP_UP R_UP_DOWN L_UP_UP L_UP_DOWN C_R_UP C_R_DOWN C_L_UP C_L_DOWN TB TB

typedef struct{
  uint16_t R_rocker_X;
  uint16_t R_rocker_Y;
  uint16_t L_rocker_X;
  uint16_t L_rocker_Y;
}command_rocker_t;

typedef struct{
  uint8_t R_UP_UP;
  uint8_t R_UP_DOWN;
  uint8_t L_UP_UP;
  uint8_t L_UP_DOWN;
  uint8_t C_R_UP;
  uint8_t C_R_DOWN;
  uint8_t C_L_UP;
  uint8_t C_L_DOWN;
}command_key_t;

typedef struct{
  uint8_t rxdata[RXBUFSIZE];
  uint8_t commanddata[MESLENGTH-4];
}Wirless_Data_t;

extern Wirless_Data_t Wireless_data;
extern uint8 Wireless_MainFunction();
extern void Wireless_Init(void);

/**********************WIRELESS CDD field end*************************************/

/*****************************************************************************/

//所有任务的任务ID、初始化函数、事件处理函数、任务事件都统一在此文件声明或定义
/*****************************************************************************/
extern void System_Startup(void);

extern void DPrint ( const char * format, ... );
//任务ID声明
extern uint8 Serial_TaskID;

//任务初始化函数声明
extern void Serial_Task_Init(uint8 task_id);

//任务事件处理函数声明
extern uint16 Serial_Task_EventProcess(uint8 task_id,uint16 task_event);

extern void osal_printf(char *format, ...);		//打印通用串口数据

extern uint8 AppPeriod_TaskID;
//任务初始化函数声明
extern void Period10ms_Task_Init(uint8 task_id);

//任务事件处理函数声明
extern uint16 Period10ms_Task_EventProcess(uint8 task_id,uint16 task_event);
//任务事件定义
//系统消息事件
#define SYS_EVENT_MSG		0x8000

//串口通信任务事件定义
#define	PRINTF_STR			0X0001 		//打印字符串事件
//串口通信任务系统消息事件定义
#define OSAL_PRINTF			0X01		//系统打印

#define LED_FLASH           0x0001

#define ADC_HANDLE          0x0002

#define COMMAND_HANDLE      0x0004

#define MOTOR_HANDLE        0x0008

/*****************************************************************************/

#endif

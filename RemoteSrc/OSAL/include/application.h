#ifndef APPLICATION_H
#define APPLICATION_H

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "gd32f1x0_libopt.h"
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"

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

/*****************************************************************************/

/**********************WIRELESS CDD field start*************************************/
#define MESLENGTH   13
#define RXBUFSIZE   (MESLENGTH)
#define TXBUFSIZE   (13)

#define HEADMASK1   0xFD
#define HEADMASK2   0xFE
#define TAILMASK1   0xFE
#define TAILMASK2   0xFF
#define ROCKERMES   0x01
#define KEYMES      0x02
//HB HB ROCKERMES R_rocker_X R_rocker_Y L_rocker_X L_rocker_Y TB TB
//HB HB KEYMES R_UP_UP R_UP_DOWN L_UP_UP L_UP_DOWN C_R_UP C_R_DOWN C_L_UP C_L_DOWN TB TB

typedef enum{
  DIRINIT = 0,
  UP_RIGHT ,
  FREE ,
  DOWN_LEFT 
}rocker_dir_n;

typedef enum{
  KEYFREE = 0,
  KEYPUSH ,
  KEYKEEP ,
  KEYREL
}keystate_n;

typedef struct{
  uint8_t R_rocker_X_D;
  uint8_t R_rocker_X;
  uint8_t R_rocker_Y_D;
  uint8_t R_rocker_Y;
  uint8_t L_rocker_X_D;
  uint8_t L_rocker_X;
  uint8_t L_rocker_Y_D;
  uint8_t L_rocker_Y;
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
  uint8_t txdata[TXBUFSIZE];
  uint8_t inuse_flag;
}Wirless_Data_t;

extern Wirless_Data_t Wireless_data;
extern uint8 Wireless_MainFunction();
extern void Wireless_Init(void);

/**********************WIRELESS CDD field end*************************************/

/**********************Debug uart CDD field start*************************************/
#define ENABLE_Debug    1

#if (ENABLE_Debug == 1)
  #define Dprintf(format, ...)   printf(format, ##__VA_ARGS__)
#else
  #define Dprintf(format, ...)
#endif

typedef enum{
  BUFIDLE = 0,
  RECEIVING ,
  ANALYSING,
  RECEIVED ,
  BUFSTATE_NUM,
}RXBUF_STATUS_n;

extern void Start_Debug_Uart(void);
extern void Start_Wireless_Uart(void);
extern uint8_t Start_Send_Wireless(uint8_t *data,uint8_t length);
extern void Debug_Uart_IRQHandler(void);
extern void Wireless_Uart_IRQHandler(void);

/**********************Debug uart CDD field end*************************************/

/**********************ADC field start*************************************/

#define ADC_CHANNELn      7

typedef struct{
  uint16_t adcdata[ADC_CHANNELn];
  uint8_t ready_flag;
}ADC_Data_t;

typedef enum{
  AD_ROCKER_R_Y = 0,
  AD_KEY_R ,
  AD_ROCKER_L_X ,
  AD_ROCKER_L_Y ,
  AD_KEY_L ,
  AD_BAT_VOL ,
  AD_ROCKER_R_X ,
}ADCBUF_ID_n;

extern ADC_Data_t adc_value;

/**********************ADC field end*************************************/

/**********************PowerM field start*************************************/
#define BAT_VOL_MAX     7200
#define BAT_VOL_MIN     4000
#define BAT_VOL_LOW     5000

typedef enum{
  POW_INIT = 0,
  POW_NOR,
  POW_MAX,
  POW_LOW,
  POW_MIN,
}POW_STATE_n;

typedef struct{
  uint16_t batvoltage;  //mV
  POW_STATE_n power_state;
}POW_Data_t;

extern POW_Data_t power_data;

extern void PowerManager_Init(void);
extern void PowerManager_Mainfunction(void);

/**********************PowerM field end*************************************/

/**********************KeyM field start*************************************/

extern command_key_t commandkey;

extern void KeyManager_Mainfunction(void);

/**********************KeyM field end*************************************/
extern void KeyManager_Init(void);
extern void KeyManager_Mainfunction(void);
/**********************RockerM field start*************************************/

extern command_rocker_t rockerinf;

extern void RockerManager_Init(void);
extern void RockerManager_Manifunction(void);

/**********************RockerM field end*************************************/

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

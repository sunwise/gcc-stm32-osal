#ifndef APPLICATION_H
#define APPLICATION_H

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "gd32f1x0_libopt.h"
#include "systick.h"
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

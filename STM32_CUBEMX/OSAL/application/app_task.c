/****************************************************************************************
 * 文件名  ：serial_task.c
 * 描述    ：系统串口通信任务
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

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

uint8 Serial_TaskID; //系统串口通信任务ID
uint8 Period10ms_TaskID;

enum{
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
  ADCSTATUS_t ADC_status;
}ADC_t;
ADC_t ADCData;
/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
#define PRINT_MAX   50
void DPrint ( const char * format, ... )
{
  uint32_t length;
  static char buffer[PRINT_MAX];

  va_list args;
  va_start (args, format);
  length = vsnprintf (&buffer[0],PRINT_MAX,format, args);
  va_end (args);
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&buffer[0], length);
}

//串口通信任务初始化
void Serial_Task_Init(uint8 task_id)
{
  Serial_TaskID = task_id;

}

//串口通信任务事件处理
uint16 Serial_Task_EventProcess(uint8 task_id, uint16 task_event)
{
  if ( task_event & SYS_EVENT_MSG )   	//判断系统消息事件
    {
      osal_sys_msg_t *MSGpkt;    			//定义一个指向接受系统消息结构体的指针
      //从消息队列获取消息
      MSGpkt = (osal_sys_msg_t *) osal_msg_receive(task_id);

      while (MSGpkt)
        {
          switch (MSGpkt->hdr.event)
          //判断消息事件
          {
            case OSAL_PRINTF:
              break;

            default:
              break;
          }

// Release the memory
          osal_msg_deallocate((uint8 *) MSGpkt);

// Next  获取下一个消息
          MSGpkt = (osal_sys_msg_t *) osal_msg_receive(task_id);
        }

// return unprocessed events
      return (task_event ^ SYS_EVENT_MSG);
    }

  if ( task_event & PRINTF_STR )
    {

      return task_event ^ PRINTF_STR;
    }
  return 0;
}

void Period10ms_Task_Init(uint8 task_id)
{
  Period10ms_TaskID = task_id;
  HAL_ADC_Start_DMA(&hadc, ADCData.adcvalue, ADC_CHANNEL_NUM);
}

uint16 Period10ms_Task_EventProcess(uint8 task_id, uint16 task_event)
{
  if ( task_event & SYS_EVENT_MSG )   	//判断系统消息事件
    {
      osal_sys_msg_t *MSGpkt;    			//定义一个指向接受系统消息结构体的指针
//从消息队列获取消息
      MSGpkt = (osal_sys_msg_t *) osal_msg_receive(task_id);

      while (MSGpkt)
        {
          switch (MSGpkt->hdr.event)
//判断消息事件
          {
            case OSAL_PRINTF:
              break;

            default:
              break;
          }

// Release the memory
          osal_msg_deallocate((uint8 *) MSGpkt);

// Next  获取下一个消息
          MSGpkt = (osal_sys_msg_t *) osal_msg_receive(task_id);
        }

// return unprocessed events
      return (task_event ^ SYS_EVENT_MSG);
    }
  if ( task_event & LED_FLASH )
    {
      static uint8 dir = 1;

      if ( dir )
        {
          dir = 0;
        }
      else
        {
          dir = 1;
        }

      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState) dir);
      DPrint("adcvaue = %d %d %d %d !\r\n",ADCData.adcvalue[0],ADCData.adcvalue[1],ADCData.adcvalue[2],ADCData.adcvalue[3]);
      return task_event ^ LED_FLASH;
    }
  if ( task_event & ADC_HANDLE )
    {
      HAL_ADC_Start_DMA(&hadc, ADCData.adcvalue, ADC_CHANNEL_NUM);
      return task_event ^ ADC_HANDLE;
    }
  if ( task_event & COMMAND_HANDLE )
    {
      return task_event ^ COMMAND_HANDLE;
    }
  if ( task_event & MOTOR_HANDLE )
    {
      return task_event ^ MOTOR_HANDLE;
    }

  return 0;
}

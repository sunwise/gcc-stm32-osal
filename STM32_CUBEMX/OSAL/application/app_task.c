/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

uint8 Serial_TaskID; //系统串口通信任务ID
uint8 AppPeriod_TaskID;

void Serial_Task_Init(uint8 task_id);
uint16 Serial_Task_EventProcess(uint8 task_id, uint16 task_event);
void AppPeriod_Task_Init(uint8 task_id);
uint16 AppPeriod_Task_EventProcess(uint8 task_id, uint16 task_event);
void System_Startup(void);

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
#define ADC_EVENT_PRIEOD     117
#define LED_EVENT_PRIEOD     503
#define COMMAND_EVENT_PRIEOD 36
#define MOTOR_EVENT_PRIEOD   59

void System_Startup(void)
{
  HAL_DISABLE_INTERRUPTS();
  osal_init_system();
  osal_add_Task(Serial_Task_Init,Serial_Task_EventProcess,1);
  osal_add_Task(AppPeriod_Task_Init,AppPeriod_Task_EventProcess,2);
  osal_Task_init();
  osal_mem_kick();
  HAL_ENABLE_INTERRUPTS();
  osal_start_reload_timer( Serial_TaskID, PRINTF_STR, 1000);
  osal_start_reload_timer( AppPeriod_TaskID, LED_FLASH, LED_EVENT_PRIEOD);
  osal_start_reload_timer( AppPeriod_TaskID, ADC_HANDLE, ADC_EVENT_PRIEOD);
  osal_start_reload_timer( AppPeriod_TaskID, COMMAND_HANDLE, COMMAND_EVENT_PRIEOD);
  osal_start_reload_timer( AppPeriod_TaskID, MOTOR_HANDLE, MOTOR_EVENT_PRIEOD);
  
  osal_start_system();
  Dprintf("OSAL System init finish! \r\n");
}

//串口通信任务初始化
void Serial_Task_Init(uint8 task_id)
{
  Serial_TaskID = task_id;
  Start_Debug_Uart();
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

void AppPeriod_Task_Init(uint8 task_id)
{
  AppPeriod_TaskID = task_id;
  Start_ADC_Scan();
  Motor_Control_Init();
  Wireless_Init();
}

uint16 AppPeriod_Task_EventProcess(uint8 task_id, uint16 task_event)
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
//          Dprintf("500ms second event! \r\n");
          
          dir = 1;
        }

      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState) dir);
      
//      Test_Motor_Driver();
      
      return task_event ^ LED_FLASH;
    }
  if ( task_event & ADC_HANDLE )
    {
      Start_ADC_Scan();
      ADC2Voltage_Handle(&ADCData);
      
      return task_event ^ ADC_HANDLE;
    }
  if ( task_event & COMMAND_HANDLE )
    {
      Wireless_MainFunction();
      
      return task_event ^ COMMAND_HANDLE;
    }
  if ( task_event & MOTOR_HANDLE )
    {
      Motor_Control_mainfunction();
      
      return task_event ^ MOTOR_HANDLE;
    }

  return 0;
}

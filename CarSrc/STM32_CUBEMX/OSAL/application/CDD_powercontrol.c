/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define POW_ON_DELAY   2
#define POW_OFF_DELAY  10
#define POW_INIT 1
#define POW_HOLD 2
#define LOW_POW_VOL   6000 //mV
static uint8_t powerstatus = POW_INIT;
   
void hold_power(POWER_ACT_n act)
{
  if(act == HOLD_POW)
  {
    HAL_GPIO_WritePin(POWER_HOLD_GPIO_Port, POWER_HOLD_Pin, GPIO_PIN_SET);
  }
  
  if(act == CLOSE_POW)
  {
    HAL_GPIO_WritePin(POWER_HOLD_GPIO_Port, POWER_HOLD_Pin, GPIO_PIN_RESET);
  }
  
}

void power_control(void)
{
  static uint8_t timecount = 0;
  
  switch(powerstatus){
    case POW_INIT:
      
      if(timecount < POW_ON_DELAY)
      {
        timecount ++;
        if(timecount == POW_ON_DELAY)
        {
          hold_power(HOLD_POW);
          powerstatus = POW_HOLD;
          timecount = 0;
        }
      }
      
      break;
    case POW_HOLD:
      if(ADCData.ADC_status == VALID)
      {
        if(ADCData.Device_iv[BATTERY] < LOW_POW_VOL)
        {
          timecount ++;
          if(timecount == POW_OFF_DELAY)
          {
            hold_power(CLOSE_POW);
          }
        }
        else
        {
          timecount = 0;
        }
      }
      break;
    default:
      break;
  }
}
/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define HOLDPOW_PIN                         GPIO_PIN_15
#define HOLDPOW_GPIO_PORT                   GPIOA
#define HOLDPOW_GPIO_CLK                    RCU_GPIOA

#define POWKEY_PIN                         GPIO_PIN_12
#define POWKEY_GPIO_PORT                   GPIOA
#define POWKEY_GPIO_CLK                    RCU_GPIOA
   
POW_Data_t power_data={
  6000,
  POW_INIT,
};

uint8_t CalculateBatVoltage(void);

void PowerManager_Init(void)
{
  /* enable the led clock */
  rcu_periph_clock_enable(HOLDPOW_GPIO_CLK);
  /* configure led GPIO port */ 
  gpio_mode_set(HOLDPOW_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,HOLDPOW_PIN);
  gpio_output_options_set(HOLDPOW_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,HOLDPOW_PIN);
  
  GPIO_BC(HOLDPOW_GPIO_PORT) = HOLDPOW_PIN;
}
/***********************************
*  input 0 close 1 hold
***********************************/
void power_hold_control(uint8_t sw)
{
  if(sw)
  {
    GPIO_BOP(HOLDPOW_GPIO_PORT) = HOLDPOW_PIN;
  }
  else
  {
    GPIO_BC(HOLDPOW_GPIO_PORT) = HOLDPOW_PIN;
  }
}

FlagStatus read_power_key(void)
{
  return gpio_input_bit_get(POWKEY_GPIO_PORT,POWKEY_PIN);
}

void PowerManager_Mainfunction(void)
{
  static uint8_t keepcount = 0;
  
  switch(power_data.power_state){
    case POW_INIT:
        keepcount = 0;
        power_data.power_state = POW_NOR;
        power_hold_control(TRUE);
      break;
    case POW_MAX:
      power_hold_control(FALSE);
      break;
    case POW_NOR:
      CalculateBatVoltage();
      if((commandkey.L_UP_DOWN == KEYPUSH)&&(commandkey.R_UP_DOWN == KEYPUSH))
      {
        keepcount ++;
      }
      else
      {
        keepcount = 0;
      }
      if(keepcount > 30)
      {
        keepcount = 0;
        power_data.power_state = POW_MIN;
        power_hold_control(FALSE);
      }
      break;
    case POW_LOW:
      CalculateBatVoltage();
      if((commandkey.L_UP_DOWN == KEYPUSH)&&(commandkey.R_UP_DOWN == KEYPUSH))
      {
        keepcount ++;
      }
      else
      {
        keepcount = 0;
      }
      if(keepcount > 30)
      {
        keepcount = 0;
        power_data.power_state = POW_MIN;
        power_hold_control(FALSE);
      }
      break;
    case POW_MIN:
      power_hold_control(FALSE);
      break;
    default:
      break;
  }
  
}
/***********************************

***********************************/
uint8_t CalculateBatVoltage(void)
{
  uint8_t res = FALSE;
  
  if(adc_value.ready_flag == TRUE)
  {
    power_data.batvoltage  = (uint16_t)(adc_value.adcdata[AD_BAT_VOL]*2.4);
    
    if(power_data.batvoltage > BAT_VOL_MAX)
    {
      power_data.power_state = POW_MAX;
      return (res);
    }
    if(power_data.batvoltage < BAT_VOL_MIN)
    {
      power_data.power_state = POW_MIN;
      return (res);
    }
    if(power_data.batvoltage < BAT_VOL_LOW)
    {
      power_data.power_state = POW_LOW;
      return (res);
    }
    power_data.power_state = POW_NOR;
  }
  
  return (res);
}
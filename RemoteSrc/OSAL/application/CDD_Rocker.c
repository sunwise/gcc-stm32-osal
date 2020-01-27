/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define ROCKER_FREE_MIN    1800
#define ROCKER_FREE_MAX    2200

command_rocker_t rockerinf;

void Right_Rocker(void);
void Left_Rocker(void);


void RockerManager_Init(void)
{
  rockerinf.R_rocker_X = 0;
  rockerinf.R_rocker_X_D = FREE;
  rockerinf.R_rocker_Y = 0;
  rockerinf.R_rocker_Y_D = FREE;
  rockerinf.L_rocker_X = 0;
  rockerinf.L_rocker_X_D = FREE;
  rockerinf.L_rocker_Y = 0;
  rockerinf.L_rocker_Y_D = FREE;
}

void RockerManager_Manifunction(void)
{
  if(adc_value.ready_flag)
  {
    Right_Rocker();
    Left_Rocker();

  }
}

void Right_Rocker(void)
{
  if((adc_value.adcdata[AD_ROCKER_R_X] > ROCKER_FREE_MIN) && (adc_value.adcdata[AD_ROCKER_R_X] < ROCKER_FREE_MAX))
  {
    rockerinf.R_rocker_X = 0;
    rockerinf.R_rocker_X_D = FREE;
  }
  else
  {
    if(adc_value.adcdata[AD_ROCKER_R_X] < ROCKER_FREE_MIN)
    {
      rockerinf.R_rocker_X = (ROCKER_FREE_MIN-adc_value.adcdata[AD_ROCKER_R_X]) >> 3;
      rockerinf.R_rocker_X_D = DOWN_LEFT;
    }
    if(adc_value.adcdata[AD_ROCKER_R_X] > ROCKER_FREE_MAX)
    {
      rockerinf.R_rocker_X = (adc_value.adcdata[AD_ROCKER_R_X]-ROCKER_FREE_MAX) >> 3;
      rockerinf.R_rocker_X_D = UP_RIGHT;
    }
  }
  
  if((adc_value.adcdata[AD_ROCKER_R_Y] > ROCKER_FREE_MIN) && (adc_value.adcdata[AD_ROCKER_R_Y] < ROCKER_FREE_MAX))
  {
    rockerinf.R_rocker_Y = 0;
    rockerinf.R_rocker_Y_D = FREE;
  }
  else
  {
    if(adc_value.adcdata[AD_ROCKER_R_Y] < ROCKER_FREE_MIN)
    {
      rockerinf.R_rocker_Y = (ROCKER_FREE_MIN-adc_value.adcdata[AD_ROCKER_R_Y]) >> 3;
      rockerinf.R_rocker_Y_D = UP_RIGHT;
    }
    if(adc_value.adcdata[AD_ROCKER_R_Y] > ROCKER_FREE_MAX)
    {
      rockerinf.R_rocker_Y = (adc_value.adcdata[AD_ROCKER_R_Y]-ROCKER_FREE_MAX) >> 3;
      rockerinf.R_rocker_Y_D = DOWN_LEFT;
    }
  }
  
}

void Left_Rocker(void)
{
  if((adc_value.adcdata[AD_ROCKER_L_X] > ROCKER_FREE_MIN) && (adc_value.adcdata[AD_ROCKER_L_X] < ROCKER_FREE_MAX))
  {
    rockerinf.L_rocker_X = 0;
    rockerinf.L_rocker_X_D = FREE;
  }
  else
  {
    if(adc_value.adcdata[AD_ROCKER_L_X] < ROCKER_FREE_MIN)
    {
      rockerinf.L_rocker_X = (ROCKER_FREE_MIN-adc_value.adcdata[AD_ROCKER_L_X]) >> 3;
      rockerinf.L_rocker_X_D = UP_RIGHT;
    }
    if(adc_value.adcdata[AD_ROCKER_L_X] > ROCKER_FREE_MAX)
    {
      rockerinf.L_rocker_X = (adc_value.adcdata[AD_ROCKER_L_X]-ROCKER_FREE_MAX) >> 3;
      rockerinf.L_rocker_X_D = DOWN_LEFT;
    }
  }
  
  if((adc_value.adcdata[AD_ROCKER_L_Y] > ROCKER_FREE_MIN) && (adc_value.adcdata[AD_ROCKER_L_Y] < ROCKER_FREE_MAX))
  {
    rockerinf.L_rocker_Y = 0;
    rockerinf.L_rocker_Y_D = FREE;
  }
  else
  {
    if(adc_value.adcdata[AD_ROCKER_L_Y] < ROCKER_FREE_MIN)
    {
      rockerinf.L_rocker_Y = (ROCKER_FREE_MIN-adc_value.adcdata[AD_ROCKER_L_Y]) >> 3;
      rockerinf.L_rocker_Y_D = DOWN_LEFT;
    }
    if(adc_value.adcdata[AD_ROCKER_L_Y] > ROCKER_FREE_MAX)
    {
      rockerinf.L_rocker_Y = (adc_value.adcdata[AD_ROCKER_L_Y]-ROCKER_FREE_MAX) >> 3;
      rockerinf.L_rocker_Y_D = UP_RIGHT;
    }
  }
  
}


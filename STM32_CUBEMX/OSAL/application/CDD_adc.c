/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define ADC_CDD_DEBUG    0
ADC_t ADCData ={
  {0,0,0,0},
  {0,0,0,0},
  INVALID,
};

HAL_StatusTypeDef Start_ADC_Scan(void)
{
  HAL_StatusTypeDef res;
  res = HAL_ADC_Start_DMA(&hadc, ADCData.adcvalue, ADC_CHANNEL_NUM);
  if(res != HAL_OK)
  {
    Dprintf("adc start error = %d !\r\n",res);
  }
#if(ADC_CDD_DEBUG)
  Dprintf("adcvaue = %d %d %d %d !\r\n",ADCData.adcvalue[0],ADCData.adcvalue[1],ADCData.adcvalue[2],ADCData.adcvalue[3]);
#endif
  return res ;
}
/***************************************
*battery voltage is xxx (mv)
*device current is xxx  (ma)
***************************************/
uint8_t ADC2Voltage_Handle(ADC_t *data)
{
  if(data->adcvalue[BATTERY] > 500)//battery voltage up 3.0V
  {
    data->Device_iv[BATTERY] = (float)(data->adcvalue[BATTERY] * 4.91 );
    data->Device_iv[DIR_MOTOR] = (float)(data->adcvalue[DIR_MOTOR] * 1.44 );
    data->Device_iv[REAR_MOTOR] = (float)(data->adcvalue[REAR_MOTOR] * 1.44 );
    data->Device_iv[FRONT_MOTOR] = (float)(data->adcvalue[FRONT_MOTOR] * 1.44 );
    
    data->ADC_status = VALID;
    
    return 0;
  }
  
  return 1;
}
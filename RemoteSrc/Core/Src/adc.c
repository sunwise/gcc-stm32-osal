/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

ADC_Data_t adc_value;

void Start_ADC_Scan(void)
{
  adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}
void Check_ADC_State(void)
{
  if(adc_value.adcdata[AD_BAT_VOL] > 1000)
  {
    adc_value.ready_flag = TRUE;
  }
}
/* ADC init function */
void ADC_Init(void)
{
  /* enable DMA clock */
  rcu_periph_clock_enable(RCU_DMA);
  /* enable ADC clock */
  rcu_periph_clock_enable(RCU_ADC);
  /* config ADC clock */
  rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
  /* enable the clock */
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
    
  gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
  gpio_mode_set(GPIOB,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_1);
  
  /* ADC_DMA_channel configuration */
  dma_deinit(DMA_CH0);
  dma_periph_address_config(DMA_CH0,(uint32_t)(&ADC_RDATA));
  dma_memory_address_config(DMA_CH0,(uint32_t)(&adc_value.adcdata[0]));
  dma_transfer_direction_config(DMA_CH0,DMA_PERIPHERAL_TO_MEMORY);
  dma_memory_width_config(DMA_CH0,DMA_MEMORY_WIDTH_16BIT);
  dma_periph_width_config(DMA_CH0,DMA_PERIPHERAL_WIDTH_16BIT);
  dma_priority_config(DMA_CH0,DMA_PRIORITY_HIGH);
  dma_transfer_number_config(DMA_CH0,ADC_CHANNELn);
  dma_periph_increase_disable(DMA_CH0);
  dma_memory_increase_enable(DMA_CH0);
  dma_circulation_enable(DMA_CH0);
  dma_channel_enable(DMA_CH0);
  
  /* ADC channel length config */
    adc_channel_length_config(ADC_REGULAR_CHANNEL,ADC_CHANNELn);

  /* ADC regular channel config */
  adc_regular_channel_config(0,ADC_CHANNEL_0,ADC_SAMPLETIME_239POINT5);
  adc_regular_channel_config(1,ADC_CHANNEL_1,ADC_SAMPLETIME_239POINT5);
  adc_regular_channel_config(2,ADC_CHANNEL_4,ADC_SAMPLETIME_239POINT5);
  adc_regular_channel_config(3,ADC_CHANNEL_5,ADC_SAMPLETIME_239POINT5);
  adc_regular_channel_config(4,ADC_CHANNEL_6,ADC_SAMPLETIME_239POINT5);
  adc_regular_channel_config(5,ADC_CHANNEL_7,ADC_SAMPLETIME_239POINT5);
  adc_regular_channel_config(6,ADC_CHANNEL_9,ADC_SAMPLETIME_239POINT5);
    
  /* ADC external trigger enable */
  adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
  /* ADC external trigger source config */
  adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_SWRCST);
  /* ADC data alignment config */
  adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
  adc_discontinuous_mode_config(ADC_REGULAR_CHANNEL,8);
  /* enable ADC interface */
  adc_enable();
  /* ADC calibration and reset calibration */
  adc_calibration_enable();
  /* ADC DMA function enable */
  adc_dma_mode_enable();
  /* ADC contineous function enable */
//  adc_special_function_config(ADC_CONTINUOUS_MODE,ENABLE);
  /* ADC software trigger enable */
  adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

  adc_value.ready_flag = FALSE;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

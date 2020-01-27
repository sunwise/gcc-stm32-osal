/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
void nvic_Irq_init(void);
void rcu_init(void);

int main(void)
{
  systick_config();
  
  nvic_Irq_init();
  
  gd_eval_led_init (LED1);
  gd_eval_led_init (LED2);
  gd_eval_led_init (LED3);
  gd_eval_led_init (LED_GROUP1);
  gd_eval_led_init (LED_GROUP2);
  
  gd_eval_key_init(KEY_LOCK, KEY_MODE_GPIO);
  gd_eval_key_init(KEY_POWER, KEY_MODE_GPIO);

  gd_eval_com1_init();
  gd_eval_com2_init();
  ADC_Init();
  
  gd_eval_led_on (LED_GROUP1);
  gd_eval_led_off (LED_GROUP2);
  gd_eval_led_off (LED1);
  gd_eval_led_off (LED2);
  gd_eval_led_off (LED3);
  
  System_Startup();
  
  while (1)
  {

  }
  
}

void nvic_Irq_init(void)
{
  /* USART interrupt configuration */
  nvic_irq_enable(USART0_IRQn, 0, 0);
}



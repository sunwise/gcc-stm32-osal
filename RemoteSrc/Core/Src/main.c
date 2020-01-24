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

int main(void)
{
  systick_config();
  
  gd_eval_led_init (LED1);
  gd_eval_led_init (LED2);
  gd_eval_led_init (LED3);
  gd_eval_led_init (LED_GROUP1);
  gd_eval_led_init (LED_GROUP2);

  gd_eval_com_init(EVAL_COM1);
  
  gd_eval_led_on (LED_GROUP1);
  gd_eval_led_on (LED_GROUP2);
  gd_eval_led_on (LED1);
  gd_eval_led_on (LED2);
  gd_eval_led_on (LED3);
  
  System_Startup();
  
  while (1)
  {

  }
  
}


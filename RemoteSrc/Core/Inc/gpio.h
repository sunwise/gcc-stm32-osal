/*!
    \file  gd32f1x0_eval.h
    \brief definitions for GD32f1x0_EVAL's Leds, Keys and COM ports hardware resources
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GPIO_H
#define GPIO_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f1x0.h"

/* exported types */
typedef enum 
{
    LED1 = 0,
    LED2,
    LED3,
    LED4,
    LED_GROUP1,
    LED_GROUP2,
    LEDn
} led_typedef_enum;

typedef enum 
{
    KEY_LOCK = 0,
    KEY_POWER = 1
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

/* GD32130_150 eval low layer led */
//#define LEDn                             4

#define LED1_PIN                         GPIO_PIN_13
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCU_GPIOC
  
#define LED2_PIN                         GPIO_PIN_15
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCU_GPIOC
  
#define LED3_PIN                         GPIO_PIN_14
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCU_GPIOC
  
#define LED4_PIN                         GPIO_PIN_2
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK                    RCU_GPIOD

#define LED_GROUP1_PIN                         GPIO_PIN_9
#define LED_GROUP1_GPIO_PORT                   GPIOB
#define LED_GROUP1_GPIO_CLK                    RCU_GPIOB

#define LED_GROUP2_PIN                         GPIO_PIN_8
#define LED_GROUP2_GPIO_PORT                   GPIOB
#define LED_GROUP2_GPIO_CLK                    RCU_GPIOB

/* GD32130_150 eval low layer button */  
#define KEYn                             2

/* lock push-button */
#define LOCK_KEY_PIN                     GPIO_PIN_2
#define LOCK_KEY_GPIO_PORT               GPIOB
#define LOCK_KEY_GPIO_CLK                RCU_GPIOB
#define LOCK_KEY_EXTI_LINE               EXTI_2
#define LOCK_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOB
#define LOCK_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN7
#define LOCK_KEY_EXTI_IRQn               EXTI4_15_IRQn

/* power push-button */
#define POWER_KEY_PIN                     GPIO_PIN_12
#define POWER_KEY_GPIO_PORT               GPIOA
#define POWER_KEY_GPIO_CLK                RCU_GPIOA
#define POWER_KEY_EXTI_LINE               EXTI_12
#define POWER_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define POWER_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN12
#define POWER_KEY_EXTI_IRQn               EXTI4_15_IRQn


/* function declarations */
/* configures led GPIO */
void gd_eval_led_init          (led_typedef_enum lednum);
/* turn on selected led */
void gd_eval_led_on            (led_typedef_enum lednum);
/* turn off selected led */
void gd_eval_led_off           (led_typedef_enum lednum);
/* toggle the selected led */
void gd_eval_led_toggle        (led_typedef_enum lednum);
/* configure key */
void gd_eval_key_init          (key_typedef_enum keynum, keymode_typedef_enum key_mode);
/* return the selected button state */
uint8_t gd_eval_key_state_get   (key_typedef_enum keynum);


#ifdef cplusplus
}
#endif

#endif /* GD32F1x0_EVAL_H */


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

#ifndef UART_H
#define UART_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f1x0.h"
   
/* GD32130_150 eval low layer COM */
#define COMn                             2

/* definition for COM 1, connected to USART0 */
#define EVAL_COM1                        USART0
#define EVAL_COM1_CLK                    RCU_USART0

#define EVAL_COM1_TX_PIN                 GPIO_PIN_9
#define EVAL_COM1_RX_PIN                 GPIO_PIN_10

#define EVAL_COM1_GPIO_PORT               GPIOA
#define EVAL_COM1_GPIO_CLK                RCU_GPIOA
#define EVAL_COM1_AF                      GPIO_AF_1
   
/* definition for COM 2, connected to USART1 */
#define EVAL_COM2                        USART1
#define EVAL_COM2_CLK                    RCU_USART1

#define EVAL_COM2_TX_PIN                 GPIO_PIN_8
#define EVAL_COM2_RX_PIN                 GPIO_PIN_0

#define EVAL_COM2_TX_GPIO_PORT               GPIOA
#define EVAL_COM2_TX_GPIO_CLK                RCU_GPIOA
#define EVAL_COM2_TX_AF                      GPIO_AF_4
   
#define EVAL_COM2_RX_GPIO_PORT               GPIOB
#define EVAL_COM2_RX_GPIO_CLK                RCU_GPIOB
#define EVAL_COM2_RX_AF                      GPIO_AF_4


#define EVAL_DEBUG_COM         EVAL_COM2


/* configure COM port */
void gd_eval_com1_init(void);
void gd_eval_com2_init(void);

#ifdef cplusplus
}
#endif

#endif
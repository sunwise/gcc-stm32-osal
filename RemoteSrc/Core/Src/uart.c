 /*!
    \file  gd32f1x0_eval.c
    \brief firmware functions to manage leds, keys, COM ports
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "uart.h"

/*!
    \brief      configure COM port
    \param[in]  COM: COM on the board
      \arg        EVAL_COM1: COM1 on the board
      \arg        EVAL_COM2: COM2 on the board
    \param[out] none
    \retval     none
*/
void gd_eval_com1_init(void)
{
    /* enable GPIO clock */

    rcu_periph_clock_enable( EVAL_COM1_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(EVAL_COM1_CLK);

    /* connect port to USARTx_Tx */
    gpio_af_set(EVAL_COM1_GPIO_PORT, EVAL_COM1_AF, EVAL_COM1_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_af_set(EVAL_COM1_GPIO_PORT, EVAL_COM1_AF, EVAL_COM1_RX_PIN);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(EVAL_COM1_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,EVAL_COM1_TX_PIN);
    gpio_output_options_set(EVAL_COM1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,EVAL_COM1_TX_PIN);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(EVAL_COM1_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,EVAL_COM1_RX_PIN);
    gpio_output_options_set(EVAL_COM1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,EVAL_COM1_RX_PIN);

    /* USART configure */
    usart_deinit(EVAL_COM1);
    usart_baudrate_set(EVAL_COM1,115200U);
    usart_transmit_config(EVAL_COM1, USART_TRANSMIT_ENABLE);
    usart_receive_config(EVAL_COM1, USART_RECEIVE_ENABLE);
    usart_enable(EVAL_COM1);

}

/*!
    \brief      configure COM port
    \param[in]  COM: COM on the board
      \arg        EVAL_COM1: COM1 on the board
      \arg        EVAL_COM2: COM2 on the board
    \param[out] none
    \retval     none
*/
void gd_eval_com2_init(void)
{
    /* enable GPIO clock */

    rcu_periph_clock_enable( EVAL_COM2_TX_GPIO_CLK);
    rcu_periph_clock_enable( EVAL_COM2_RX_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(EVAL_COM2_CLK);

    /* connect port to USARTx_Tx */
    gpio_af_set(EVAL_COM2_TX_GPIO_PORT, EVAL_COM2_TX_AF, EVAL_COM2_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_af_set(EVAL_COM2_RX_GPIO_PORT, EVAL_COM2_RX_AF, EVAL_COM2_RX_PIN);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(EVAL_COM2_TX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,EVAL_COM2_TX_PIN);
    gpio_output_options_set(EVAL_COM2_TX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,EVAL_COM2_TX_PIN);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(EVAL_COM2_RX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,EVAL_COM2_RX_PIN);
    gpio_output_options_set(EVAL_COM2_RX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,EVAL_COM2_RX_PIN);

    /* USART configure */
    usart_deinit(EVAL_COM2);
    usart_baudrate_set(EVAL_COM2,115200U);
    usart_transmit_config(EVAL_COM2, USART_TRANSMIT_ENABLE);
    usart_receive_config(EVAL_COM2, USART_RECEIVE_ENABLE);
    usart_enable(EVAL_COM2);

}

/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define USARTx_Debug               EVAL_COM2
#define USARTx_Wireless            EVAL_COM1

#define RX_BUFFER_SIZE   12
#define Wirless_BUFFER_SIZE   MESLENGTH
/**
  * @brief RX buffers for storing received data
  */
uint8_t DebugUartRXBuffer[RX_BUFFER_SIZE];
__IO uint8_t BufferReadyIndication;

uint8_t *PWirelessData;
uint8_t *BufferInUse;
__IO uint8_t WirelessBufferIndication;

uint8_t *PresponseData;
uint8_t Txdatalength;
uint8_t TxBufferIndication;

int fputc(int ch, FILE *f)
{
  usart_data_transmit(USARTx_Debug, ch);
  while(RESET == usart_flag_get(USARTx_Debug, USART_FLAG_TBE));
  
  return ch;
}

void Start_Debug_Uart(void)
{
  BufferReadyIndication = 0;
  
  usart_interrupt_enable(USARTx_Debug, USART_INT_RBNE);
}

void Start_Wireless_Uart(void)
{
  WirelessBufferIndication = 0;
  PWirelessData  = &Wireless_data.rxdata[0];
  BufferInUse = &Wireless_data.inuse_flag;
  TxBufferIndication = 0;
  
  usart_interrupt_enable(USARTx_Wireless, USART_INT_RBNE);
}

uint8_t Start_Send_Wireless(uint8_t *data,uint8_t length)
{
  PresponseData = data;
  Txdatalength = length;
  TxBufferIndication = 0;
  /* Start USART transmission : Will initiate TXE interrupt after DR register is empty */
  usart_data_transmit(USARTx_Wireless, PresponseData[TxBufferIndication]);

  /* Enable TXE interrupt */
  usart_interrupt_enable(USARTx_Wireless, USART_INT_TBE);
  
  return 0;
}


static void Debug_USART_CharReception_Callback(void)
{

  /* Read Received character. RXNE flag is cleared by reading of DR register */
  DebugUartRXBuffer[BufferReadyIndication++] = usart_data_receive(USARTx_Debug);

  /* Checks if Buffer full indication has been set */
  if (BufferReadyIndication >= RX_BUFFER_SIZE)
  {
    /* Set Buffer swap indication */
    BufferReadyIndication = 0;
  }
}

static void Wireless_USART_CharReception_Callback(void)
{
  if(*BufferInUse != ANALYSING)
  {

    if(usart_data_receive(USARTx_Wireless) == HEADMASK1)
    {
      WirelessBufferIndication = 0;
      *BufferInUse = RECEIVING;
    }
    /* Read Received character. RXNE flag is cleared by reading of DR register */
    PWirelessData[WirelessBufferIndication++] = usart_data_receive(USARTx_Wireless);

    /* Checks if Buffer full indication has been set */
    if (WirelessBufferIndication >= Wirless_BUFFER_SIZE)
    {
      /* Set Buffer swap indication */
      WirelessBufferIndication = 0;
      *BufferInUse = RECEIVED;
    }
  }
}

/**
  * @brief  Function called for achieving next TX Byte sending
  * @param  None
  * @retval None
  */
void Wireless_USART_TXEmpty_Callback(void)
{
  if(TxBufferIndication == (Txdatalength - 1))
  {
    /* Disable TXE interrupt */
    usart_interrupt_disable(USARTx_Wireless, USART_INT_TBE);
    
    /* Enable TC interrupt */
    usart_interrupt_enable(USARTx_Wireless, USART_INT_TC);
  }

  /* Fill DR with a new char */
  usart_data_transmit(USARTx_Wireless, PresponseData[TxBufferIndication++]);
}
/**
  * @brief  Function called at completion of last byte transmission
  * @param  None
  * @retval None
  */
void Wireless_USART_CharTransmitComplete_Callback(void)
{
  if(TxBufferIndication == Txdatalength)
  {
    TxBufferIndication = 0;
    
    /* Disable TC interrupt */
    usart_interrupt_disable(USARTx_Wireless, USART_INT_TC);
  }
}

void Wireless_Uart_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  /* Check RXNE flag value in SR register */
  if(usart_interrupt_flag_get(USARTx_Wireless, USART_INT_FLAG_RBNE))
  {
    /* RXNE flag will be cleared by reading of DR register (done in call) */
    /* Call function in charge of handling Character reception */
    Wireless_USART_CharReception_Callback();
  }
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
  else if(usart_interrupt_flag_get(USARTx_Wireless, USART_INT_FLAG_TBE))
  {
    /* TXE flag will be automatically cleared when writing new data in DR register */

    /* Call function in charge of handling empty DR => will lead to transmission of next character */
    Wireless_USART_TXEmpty_Callback();
  }

  if(usart_interrupt_flag_get(USARTx_Wireless, USART_INT_TC))
  {
    /* Clear TC flag */
    usart_interrupt_flag_clear(USARTx_Wireless, USART_INT_TC);
    /* Call function in charge of handling end of transmission of sent character
       and prepare next charcater transmission */
    Wireless_USART_CharTransmitComplete_Callback();
  }
}

void Debug_Uart_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  /* Check RXNE flag value in SR register */
  if(usart_interrupt_flag_get(USARTx_Debug, USART_INT_FLAG_RBNE))
  {
    /* RXNE flag will be cleared by reading of DR register (done in call) */
    /* Call function in charge of handling Character reception */
    Debug_USART_CharReception_Callback();
  }
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
  else if(usart_interrupt_flag_get(USARTx_Debug, USART_INT_FLAG_TBE))
  {
    /* TXE flag will be automatically cleared when writing new data in DR register */
  }

  if(usart_interrupt_flag_get(USARTx_Debug, USART_INT_TC))
  {
    /* Clear TC flag */
    usart_interrupt_flag_clear(USARTx_Debug, USART_INT_TC);
  }
}



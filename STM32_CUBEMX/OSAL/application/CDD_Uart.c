/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define USARTx_Debug               USART2
#define USARTx_Wireless            USART1

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
  LL_USART_TransmitData8(USARTx_Debug, ch);
  while(!LL_USART_IsActiveFlag_TXE(USARTx_Debug));
  
  return ch;
}

void Start_Debug_Uart(void)
{
  BufferReadyIndication = 0;
  
  LL_USART_EnableIT_RXNE(USARTx_Debug);
//  LL_USART_EnableIT_ERROR(USARTx_Debug);
  /****************disable TX interrupt**************/
//  LL_USART_EnableIT_TXE(USARTx_Debug);
//  LL_USART_EnableIT_TC(USARTx_Debug);
}

void Start_Wireless_Uart(void)
{
  WirelessBufferIndication = 0;
  PWirelessData  = &Wireless_data.rxdata[0];
  BufferInUse = &Wireless_data.inuse_flag;
  TxBufferIndication = 0;
  
  LL_USART_EnableIT_RXNE(USARTx_Wireless);
  LL_USART_EnableIT_ERROR(USARTx_Wireless);
  /****************disable TX interrupt**************/
//  LL_USART_EnableIT_TXE(USARTx_Wireless);
//  LL_USART_EnableIT_TC(USARTx_Wireless);
}

uint8_t Start_Send_Wireless(uint8_t *data,uint8_t length)
{
  PresponseData = data;
  Txdatalength = length;
  TxBufferIndication = 0;
  /* Start USART transmission : Will initiate TXE interrupt after DR register is empty */
  LL_USART_TransmitData8(USARTx_Wireless, PresponseData[TxBufferIndication]); 

  /* Enable TXE interrupt */
  LL_USART_EnableIT_TXE(USARTx_Wireless); 
  
  return 0;
}


static void Debug_USART_CharReception_Callback(void)
{

  /* Read Received character. RXNE flag is cleared by reading of DR register */
  DebugUartRXBuffer[BufferReadyIndication++] = LL_USART_ReceiveData8(USARTx_Debug);

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

    if(LL_USART_ReceiveData8(USARTx_Wireless) == HEADMASK1)
    {
      WirelessBufferIndication = 0;
      *BufferInUse = RECEIVING;
    }
    /* Read Received character. RXNE flag is cleared by reading of DR register */
    PWirelessData[WirelessBufferIndication++] = LL_USART_ReceiveData8(USARTx_Wireless);

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
    LL_USART_DisableIT_TXE(USARTx_Wireless);
    
    /* Enable TC interrupt */
    LL_USART_EnableIT_TC(USARTx_Wireless);
  }

  /* Fill DR with a new char */
  LL_USART_TransmitData8(USARTx_Wireless, PresponseData[TxBufferIndication++]);
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
    LL_USART_DisableIT_TC(USARTx_Wireless);
  }
}

void Wireless_Uart_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  /* Check RXNE flag value in SR register */
  if(LL_USART_IsActiveFlag_RXNE(USARTx_Wireless) && LL_USART_IsEnabledIT_RXNE(USARTx_Wireless))
  {
    /* RXNE flag will be cleared by reading of DR register (done in call) */
    /* Call function in charge of handling Character reception */
    Wireless_USART_CharReception_Callback();
  }
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
  else if(LL_USART_IsEnabledIT_TXE(USARTx_Wireless) && LL_USART_IsActiveFlag_TXE(USARTx_Wireless))
  {
    /* TXE flag will be automatically cleared when writing new data in DR register */

    /* Call function in charge of handling empty DR => will lead to transmission of next character */
    Wireless_USART_TXEmpty_Callback();
  }

  if(LL_USART_IsEnabledIT_TC(USARTx_Wireless) && LL_USART_IsActiveFlag_TC(USARTx_Wireless))
  {
    /* Clear TC flag */
    LL_USART_ClearFlag_TC(USARTx_Wireless);
    /* Call function in charge of handling end of transmission of sent character
       and prepare next charcater transmission */
    Wireless_USART_CharTransmitComplete_Callback();
  }
}

void Debug_Uart_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  /* Check RXNE flag value in SR register */
  if(LL_USART_IsActiveFlag_RXNE(USARTx_Debug) && LL_USART_IsEnabledIT_RXNE(USARTx_Debug))
  {
    /* RXNE flag will be cleared by reading of DR register (done in call) */
    /* Call function in charge of handling Character reception */
    Debug_USART_CharReception_Callback();
  }
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
  else if(LL_USART_IsEnabledIT_TXE(USARTx_Debug) && LL_USART_IsActiveFlag_TXE(USARTx_Debug))
  {
    /* TXE flag will be automatically cleared when writing new data in DR register */
  }

  if(LL_USART_IsEnabledIT_TC(USARTx_Debug) && LL_USART_IsActiveFlag_TC(USARTx_Debug))
  {
    /* Clear TC flag */
    LL_USART_ClearFlag_TC(USARTx_Debug);
  }
}



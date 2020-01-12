/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define WIRELESS_CDD_DEBUG    1
Wirless_Data_t Wireless_data;

void Wireless_Init(void)
{
  HAL_UART_Receive_DMA(&huart1, Wireless_data.rxdata, RXBUFSIZE);
  
}

uint8 Get_CommandData(void)
{
  uint8 i;
  for(i=0;i < (RXBUFSIZE-1);i ++)
  {
    if((Wireless_data.rxdata[i] == HEADMASK1) && (Wireless_data.rxdata[i+1] == HEADMASK2))
    {
      if(i > 1)
      {
        if((Wireless_data.rxdata[i-2] == TAILMASK1) && (Wireless_data.rxdata[i-1] == TAILMASK2))
        {
          return i;
        }
      }
      else if(i == 0)
      {
        if((Wireless_data.rxdata[MESLENGTH-2] == TAILMASK1) && (Wireless_data.rxdata[MESLENGTH-1] == TAILMASK2))
        {
          return i;
        }
        
      }
      memset(Wireless_data.rxdata,0,MESLENGTH);
#if(WIRELESS_CDD_DEBUG)
      DPrint("command error!\r\n");
#endif
    }
  }

  return 0xFF;
}

uint8 Analysis_Command(uint8 datan)
{
  if(datan == 0)
  {
    memcpy(Wireless_data.commanddata,&Wireless_data.rxdata[2],(MESLENGTH-4));
  }
  else if(datan < (MESLENGTH-2))
  {
    memcpy(Wireless_data.commanddata,&Wireless_data.rxdata[datan+2],(RXBUFSIZE-2-datan));
    memcpy(&Wireless_data.commanddata[RXBUFSIZE-2-datan],&Wireless_data.rxdata[0],(datan-1));
  }
  else
  {
    return 0;
  }
  if(Wireless_data.commanddata[0] == ROCKERMES)
  {
#if(WIRELESS_CDD_DEBUG)
    DPrint("rocker message %d !\r\n",datan);
#endif
    //HB HB ROCKERMES R_rocker_X R_rocker_Y L_rocker_X L_rocker_Y TB TB
  }
  if(Wireless_data.commanddata[0] == KEYMES)
  {
#if(WIRELESS_CDD_DEBUG)
    DPrint("key message %d !\r\n",datan);
#endif
    //HB HB KEYMES R_UP_UP R_UP_DOWN L_UP_UP L_UP_DOWN C_R_UP C_R_DOWN C_L_UP C_L_DOWN TB TB
  }
  
  return 0;
}

uint8 Wireless_MainFunction()
{
  uint8 datanum;
  datanum = Get_CommandData();
  if(datanum != 0xFF)
  {
    Analysis_Command(datanum);
    memset(Wireless_data.rxdata,0,MESLENGTH);
  }
  return 0;
}


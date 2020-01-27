/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

#define WIRELESS_CDD_DEBUG    1
Wirless_Data_t Wireless_data;

#define C_MESSAGE_TYPE   2
#define C_RIGHT_X_DIR    3
#define C_RIGHT_X_VAL    4
#define C_RIGHT_Y_DIR    5
#define C_RIGHT_Y_VAL    6
#define C_LEFT_X_DIR    7
#define C_LEFT_X_VAL    8
#define C_LEFT_Y_DIR    9
#define C_LEFT_Y_VAL    10

void Wireless_Init(void)
{
  Start_Wireless_Uart();
  
}

uint8 Check_CommandData(void)
{
  uint8 res = 1;
  
  if((Wireless_data.rxdata[0] == HEADMASK1) && (Wireless_data.rxdata[1] == HEADMASK2))
  {
    if((Wireless_data.rxdata[MESLENGTH-2] == TAILMASK1) && (Wireless_data.rxdata[MESLENGTH-1] == TAILMASK2))
    {
      res = 0;
      return res;
    }
  }
  Dprintf("command error!\r\n");
  
  return res;
}

void Handle_Rocker_Command(void)
{
  
}

uint8 Analysis_Command(void)
{
  
  if(Wireless_data.rxdata[C_MESSAGE_TYPE] == ROCKERMES)
  {
#if(WIRELESS_CDD_DEBUG)
    Dprintf("rocker message !\r\n");
#endif
    //HB HB ROCKERMES R_rocker_X R_rocker_Y L_rocker_X L_rocker_Y TB TB
    Handle_Rocker_Command();
  }
  if(Wireless_data.rxdata[C_MESSAGE_TYPE] == KEYMES)
  {
#if(WIRELESS_CDD_DEBUG)
    Dprintf("key message !\r\n");
#endif
    //HB HB KEYMES R_UP_UP R_UP_DOWN L_UP_UP L_UP_DOWN C_R_UP C_R_DOWN C_L_UP C_L_DOWN TB TB
    
  }
  
  return 0;
}

uint8 Fill_TXBuffer(void)
{
  Wireless_data.txdata[0] = HEADMASK1;
  Wireless_data.txdata[1] = HEADMASK2;
  Wireless_data.txdata[2] = ROCKERMES;
  
  memcpy(&Wireless_data.txdata[3],(uint8_t*)(&rockerinf),8);
  
  Wireless_data.txdata[TXBUFSIZE-2] = TAILMASK1;
  Wireless_data.txdata[TXBUFSIZE-1] = TAILMASK2;
  
  return 0;
}

uint8 Wireless_MainFunction()
{
  #define Send_Command_P    0x03
  uint8 rest;
  static uint8_t PrieodCount = 0;
  static uint16_t responseactive = 0;
  
  PrieodCount ++;
  responseactive ++;
  
  if(Wireless_data.inuse_flag == RECEIVED)
  {
    Wireless_data.inuse_flag = ANALYSING;
    rest = Check_CommandData();
    if(!rest)
    {
      Analysis_Command();
    }
    Wireless_data.rxdata[0] = 0x00;
    Wireless_data.rxdata[MESLENGTH-2] = 0x00;
    
    Wireless_data.inuse_flag = BUFIDLE;
    responseactive = 0;
  }
  
  if(PrieodCount >= Send_Command_P)
  {
    PrieodCount = 0;
    Fill_TXBuffer();
    Start_Send_Wireless(&Wireless_data.txdata[0],TXBUFSIZE);
  }
  
  if(responseactive > 1666)
  {
    RequestShutdown();
  }
  
  return 0;
}


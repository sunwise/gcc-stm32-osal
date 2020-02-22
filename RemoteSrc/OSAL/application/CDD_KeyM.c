/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

enum{
  UP_UP = 0,
  UP_DOWN,
  C_UP,
  C_DOWN,
};

#define KEYBIT(x)    (0x01U << x)

static uint8_t rkeystate[2]={0x00,0x00};
static uint8_t lkeystate[2]={0x00,0x00};

command_key_t commandkey;

void RightSide_Key(void);
void LeftSide_Key(void);
void Process_Key(void);

void KeyManager_Init(void)
{
  commandkey.R_UP_UP = KEYFREE;
  commandkey.R_UP_DOWN = KEYFREE;
  commandkey.L_UP_UP = KEYFREE;
  commandkey.L_UP_DOWN = KEYFREE;
  commandkey.C_R_UP = KEYFREE;
  commandkey.C_R_DOWN = KEYFREE;
  commandkey.C_L_UP = KEYFREE;
  commandkey.C_L_DOWN = KEYFREE;
}

void KeyManager_Mainfunction(void)
{
  if(adc_value.ready_flag)
  {
    RightSide_Key();
    LeftSide_Key();
    
  }
  Process_Key();
}

void RightSide_Key(void)
{
  uint8_t rkeyadc;
  
  rkeyadc = adc_value.adcdata[AD_KEY_R] / 100 ;
  
  rkeystate[1] = rkeystate[0];
  
  if(rkeyadc > 35)
  {
    rkeystate[0] = 0x00;
  }
  else if(rkeyadc > 25)
  {
    rkeystate[0] = KEYBIT(UP_UP);
  }
  else if(rkeyadc > 15)
  {
    rkeystate[0] = KEYBIT(UP_DOWN);
  }
  else if(rkeyadc > 5)
  {
    rkeystate[0] = KEYBIT(C_UP);
  }
  else
  {
    rkeystate[0] = KEYBIT(C_DOWN);
  }
  
}

void LeftSide_Key(void)
{
  uint8_t lkeyadc;
  
  lkeyadc = adc_value.adcdata[AD_KEY_L] / 100;
  
  lkeystate[1] = lkeystate[0];
  
  if(lkeyadc > 35)
  {
    lkeystate[0] = 0x00;
  }
  else if(lkeyadc > 25)
  {
    lkeystate[0] = KEYBIT(UP_DOWN);
  }
  else if(lkeyadc > 15)
  {
    lkeystate[0] = KEYBIT(UP_UP);
  }
  else if(lkeyadc > 5)
  {
    lkeystate[0] = KEYBIT(C_UP);
  }
  else
  {
    lkeystate[0] = KEYBIT(C_DOWN);
  }
  
}

void Process_Key(void)
{
  if((rkeystate[0] & KEYBIT(UP_UP))&& (!(rkeystate[1] & KEYBIT(UP_UP))))
  {
    commandkey.R_UP_UP = KEYPUSH;
  }
  if(!(rkeystate[1] & KEYBIT(UP_UP))&& (!(rkeystate[0] & KEYBIT(UP_UP))))
  {
    commandkey.R_UP_UP = KEYFREE;
  }
  
  if((rkeystate[0] & KEYBIT(UP_DOWN))&& (!(rkeystate[1] & KEYBIT(UP_DOWN))))
  {
    commandkey.R_UP_DOWN = KEYPUSH;
  }
  if(!(rkeystate[0] & KEYBIT(UP_DOWN))&& (!(rkeystate[1] & KEYBIT(UP_DOWN))))
  {
    commandkey.R_UP_DOWN = KEYFREE;
  }
  
  if(!(rkeystate[0] & KEYBIT(C_UP))&& (!(rkeystate[1] & KEYBIT(C_UP))))
  {
    commandkey.C_R_UP = KEYFREE;
  }
  if((rkeystate[0] & KEYBIT(C_UP))&& (!(rkeystate[1] & KEYBIT(C_UP))))
  {
    commandkey.C_R_UP = KEYPUSH;
  }
  
  if((rkeystate[0] & KEYBIT(C_DOWN))&& (!(rkeystate[1] & KEYBIT(C_DOWN))))
  {
    commandkey.C_R_DOWN = KEYPUSH;
  }
  if(!(rkeystate[0] & KEYBIT(C_DOWN))&& (!(rkeystate[1] & KEYBIT(C_DOWN))))
  {
    commandkey.C_R_DOWN = KEYFREE;
  }
  
  
  
  if((lkeystate[0] & KEYBIT(UP_UP))&& (!(lkeystate[1] & KEYBIT(UP_UP))))
  {
    commandkey.L_UP_UP = KEYPUSH;
  }
  if(!(lkeystate[0] & KEYBIT(UP_UP))&& (!(lkeystate[1] & KEYBIT(UP_UP))))
  {
    commandkey.L_UP_UP = KEYFREE;
  }
  
  if((lkeystate[0] & KEYBIT(UP_DOWN))&& (!(lkeystate[1] & KEYBIT(UP_DOWN))))
  {
    commandkey.L_UP_DOWN = KEYPUSH;
  }
  if(!(lkeystate[0] & KEYBIT(UP_DOWN))&& (!(lkeystate[1] & KEYBIT(UP_DOWN))))
  {
    commandkey.L_UP_DOWN = KEYFREE;
  }
  
  if((lkeystate[0] & KEYBIT(C_UP))&& (!(lkeystate[1] & KEYBIT(C_UP))))
  {
    commandkey.C_L_UP = KEYPUSH;
  }
  if(!(lkeystate[0] & KEYBIT(C_UP))&& (!(lkeystate[1] & KEYBIT(C_UP))))
  {
    commandkey.C_L_UP = KEYFREE;
  }
  
  if((lkeystate[0] & KEYBIT(C_DOWN))&& (!(lkeystate[1] & KEYBIT(C_DOWN))))
  {
    commandkey.C_L_DOWN = KEYPUSH;
  }
  if(!(lkeystate[0] & KEYBIT(C_DOWN))&& (!(lkeystate[1] & KEYBIT(C_DOWN))))
  {
    commandkey.C_L_DOWN = KEYFREE;
  }
}


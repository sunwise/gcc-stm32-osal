/****************************************************************************************
 * �ļ���  ��
 * ����    
 * ����ƽ̨��
 * ��汾  ��
 ***************************************************************************************/
#include "application.h"

typedef struct{
  uint8_t lockkeystate;

}Key_Data_t;

Key_Data_t  keydata;


void KeyManager_Mainfunction(void)
{
  keydata.lockkeystate = gd_eval_key_state_get(KEY_LOCK);
}
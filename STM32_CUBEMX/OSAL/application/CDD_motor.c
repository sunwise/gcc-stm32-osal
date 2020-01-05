/****************************************************************************************
 * 文件名  ：
 * 描述    
 * 开发平台：
 * 库版本  ：
 ***************************************************************************************/
#include "application.h"

Driver_Motor_t FrontMotor;
Driver_Motor_t RearMotor;
Dir_Motor_t  DirMotor;

static uint8_t Dir_Motor_Control(void);
static uint8_t Driver_Motor_Control(void);
static void SetFrontM(MOTOR_DIR_n direction,uint8_t speed);
static void SetRearM(MOTOR_DIR_n direction,uint8_t speed);
static void SetDirM(DMOTOR_DIR_n direction,uint8_t speed);

#define OLD_D  0
#define NEW_D  1

void Motor_Control_Init(void)
{
  FrontMotor.direction[OLD_D] = INIT;
  FrontMotor.direction[NEW_D] = POSITIVE;
  FrontMotor.MSpeed = 0;
  FrontMotor.status = NORMAL;
  FrontMotor.waitflag = FALSE;
  
  RearMotor.direction[OLD_D] = INIT;
  RearMotor.direction[NEW_D] = POSITIVE;
  RearMotor.MSpeed = 0;
  RearMotor.status = NORMAL;
  RearMotor.waitflag = FALSE;
  
  DirMotor.direction[OLD_D] = DINIT;
  DirMotor.direction[NEW_D] = CENTER;
  DirMotor.status = NORMAL;
  DirMotor.waitflag = FALSE;
}

uint8_t Motor_Control_mainfunction(void)
{
  Dir_Motor_Control();
  Driver_Motor_Control();
  
  return 0;
}

uint8_t Set_Driver_M(MOTOR_DIR_n direction,uint8_t speed)
{
  return 0;
}

uint8_t Set_Dir_M(DMOTOR_DIR_n direction)
{
  return 0;
}

static uint8_t Driver_Motor_Control(void)
{
  if(FrontMotor.direction[OLD_D] != FrontMotor.direction[NEW_D])
  {
    SetFrontM(FrontMotor.direction[OLD_D],0);
    FrontMotor.direction[OLD_D] = FrontMotor.direction[NEW_D];
  }
  else
  {
    SetFrontM(FrontMotor.direction[NEW_D],FrontMotor.MSpeed);
  }
  
  if(RearMotor.direction[OLD_D] != RearMotor.direction[NEW_D])
  {
    SetRearM(RearMotor.direction[OLD_D],0);
    RearMotor.direction[OLD_D] = RearMotor.direction[NEW_D];
  }
  else
  {
    SetRearM(RearMotor.direction[NEW_D],RearMotor.MSpeed);
  }
  
  return 0;
}

static uint8_t Dir_Motor_Control(void)
{
  #define LOCK_P   10
  #define RUN_P    50
  #define FREE_P   0
  
  uint8_t Speed_t = FREE_P;
  
  if(DirMotor.direction[OLD_D] != DirMotor.direction[NEW_D])
  {
    SetDirM(DirMotor.direction[OLD_D],FREE_P);
    DirMotor.direction[OLD_D] = DirMotor.direction[NEW_D];
  }
  else
  {
    if(DirMotor.direction[NEW_D] == CENTER)
    {
      Speed_t = FREE_P;
    }
    else
    {
      Speed_t = RUN_P;
    }
    SetDirM(DirMotor.direction[NEW_D],Speed_t);
  }
  
  return 0;
}

static void SetFrontM(MOTOR_DIR_n direction,uint8_t speed)
{
}

static void SetRearM(MOTOR_DIR_n direction,uint8_t speed)
{
}

static void SetDirM(DMOTOR_DIR_n direction,uint8_t speed)
{
}




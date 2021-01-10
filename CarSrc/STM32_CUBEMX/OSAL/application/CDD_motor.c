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

TIM_OC_InitTypeDef sConfigOC = {0};

static uint8_t Dir_Motor_Control(void);
static uint8_t Driver_Motor_Control(void);
static void SetFrontM(MOTOR_DIR_n direction,uint8_t speed);
static void SetRearM(MOTOR_DIR_n direction,uint8_t speed);
static void SetDirM(DMOTOR_DIR_n direction,uint8_t speed);

#define OLD_D  0
#define NEW_D  1

#define F_MOTOR_DEAD     F_MOTOR_DEAD_D
#define R_MOTOR_DEAD     R_MOTOR_DEAD_D

#define F_PWM_DUTY_MAX     F_MOTOR_MAX_DUTY
#define R_PWM_DUTY_MAX     R_MOTOR_MAX_DUTY

#define D_PWM_DUTY         D_MOTOR_DUTY

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
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  
}

uint8_t Motor_Control_mainfunction(void)
{
  Dir_Motor_Control();
  Driver_Motor_Control();
  
  return 0;
}

uint8_t Set_Driver_M(MOTOR_DIR_n direction,uint8_t speed)
{
  FrontMotor.direction[NEW_D] = direction;
  RearMotor.direction[NEW_D] = direction;
  
  if(speed)
  {
    FrontMotor.MSpeed = speed + F_MOTOR_DEAD;
    RearMotor.MSpeed = speed + R_MOTOR_DEAD;
  }
  
  if(FrontMotor.MSpeed > F_PWM_DUTY_MAX)
  {
    FrontMotor.MSpeed = F_PWM_DUTY_MAX;
  }
  
  if(RearMotor.MSpeed > R_PWM_DUTY_MAX)
  {
    RearMotor.MSpeed = R_PWM_DUTY_MAX;
  }

  return 0;
}

uint8_t Set_Dir_M(DMOTOR_DIR_n direction)
{
  DirMotor.direction[NEW_D] = direction;
  
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
  #define RUN_P    100
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
      Speed_t = D_PWM_DUTY;//RUN_P change dir motor PWM to MAX
    }
    SetDirM(DirMotor.direction[NEW_D],Speed_t);
  }
  
  return 0;
}

static void SetFrontM(MOTOR_DIR_n direction,uint8_t speed)
{
  if(direction == POSITIVE)
  {
    HAL_GPIO_WritePin(MN1_GPIO_Port, MN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MN2_GPIO_Port, MN2_Pin, GPIO_PIN_RESET);
  }
  else if(direction == NAGETIVE)
  {
    HAL_GPIO_WritePin(MN1_GPIO_Port, MN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MN2_GPIO_Port, MN2_Pin, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(MN1_GPIO_Port, MN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MN2_GPIO_Port, MN2_Pin, GPIO_PIN_RESET);
  }
  
//  sConfigOC.Pulse = (uint32_t)speed * 2;
  sConfigOC.Pulse = (uint32_t)speed;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

static void SetRearM(MOTOR_DIR_n direction,uint8_t speed)
{
  if(direction == POSITIVE)
  {
    HAL_GPIO_WritePin(MN3_GPIO_Port, MN3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MN4_GPIO_Port, MN4_Pin, GPIO_PIN_RESET);
  }
  else if(direction == NAGETIVE)
  {
    HAL_GPIO_WritePin(MN3_GPIO_Port, MN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MN4_GPIO_Port, MN4_Pin, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(MN3_GPIO_Port, MN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MN4_GPIO_Port, MN4_Pin, GPIO_PIN_RESET);
  }
  
//  sConfigOC.Pulse = (uint32_t)speed * 2;
  sConfigOC.Pulse = (uint32_t)speed;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

static void SetDirM(DMOTOR_DIR_n direction,uint8_t speed)
{
  if(direction == RIGHT)
  {
    HAL_GPIO_WritePin(MN5_GPIO_Port, MN5_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MN6_GPIO_Port, MN6_Pin, GPIO_PIN_RESET);
  }
  else if(direction == LEFT)
  {
    HAL_GPIO_WritePin(MN5_GPIO_Port, MN5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MN6_GPIO_Port, MN6_Pin, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(MN5_GPIO_Port, MN5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MN6_GPIO_Port, MN6_Pin, GPIO_PIN_RESET);
  }
  
  sConfigOC.Pulse = speed;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void Test_Motor_Driver(void)
{
  static MOTOR_DIR_n dir_driver = POSITIVE;
  static DMOTOR_DIR_n dir_dir = CENTER;
  static uint8 speedt = 5;
  static uint8 timecount=0;
  
  timecount ++;
  
  if(timecount >  4)
  {
    timecount = 0;
    
    Set_Driver_M(dir_driver,speedt);
    Set_Dir_M(dir_dir);
    
    dir_driver ++;
    dir_dir ++;
    speedt += 10;
    
    if(dir_driver == MDIR_NUM)
    {
      dir_driver = POSITIVE;
    }
    if(dir_dir == DMDIR_NUM)
    {
      dir_dir = CENTER;
    }
    if(speedt > 100)
    {
      speedt = 0;
    }
  }

}


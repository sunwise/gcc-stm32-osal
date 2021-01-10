#ifndef DEVICE_CFG_H
#define DEVICE_CFG_H

//*********************************car configuration***************************************/
#define BAT_VOL     12   //battery voltage

#define F_MOTOR_MODE  0  //1: set front motor as windlass 0: set front motor as driver wheel

#define F_MOTOR_MAX_DUTY  180   //220
#define R_MOTOR_MAX_DUTY  180   //220
#define D_MOTOR_DUTY      180

#if (BAT_VOL == 12)

#define F_MOTOR_DEAD_D   0
#define R_MOTOR_DEAD_D   0

#else

#define F_MOTOR_DEAD_D   1
#define R_MOTOR_DEAD_D   2

#endif

#define D_MOTOR_MODE     0   //0: Digital 1: analog

#define EN_ST_ENGINE     0  //1: enable steering engine
#define ST_ENGINE_MAX_ANGLE     90  //steering engine max angle

#define EN_F_MOTOR_OCP     0   //1: enable front MOTOR over current protection
#define F_MOTOR_OCT        0   //  front MOTOR over current threshhold
#define EN_R_MOTOR_OCP     0   //1: enable rear MOTOR over current protection
#define R_MOTOR_OCT        0   //  rear MOTOR over current threshhold
#define EN_D_MOTOR_OCP     0   //1: enable dir MOTOR over current protection
#define D_MOTOR_OCT        0   //  dir MOTOR over current threshhold
//************************************************************************************/

//*********************************remote parameter***************************************/

//****************************************************************************************/
#endif
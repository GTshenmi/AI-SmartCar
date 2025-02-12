/*
 * ctrlsys.h
 *
 *  Created on: 2020��12��28��
 *      Author: 936305695
 */

#ifndef APP_SYS_CTRLSYS_H_
#define APP_SYS_CTRLSYS_H_

#include "os.h"

#define MSensorUnit     MotorSys.SensorUnit
#define MDecisionUnit   MotorSys.DecisionUnit
#define MExecutionUnit  MotorSys.ExecutionUnit

#define SSensorUnit     ServoSys.SensorUnit
#define SDecisionUnit   ServoSys.DecisionUnit
#define SExecutionUnit  ServoSys.ExecutionUnit

typedef enum
{
    CtrlSys_Running,CtrlSys_Stopped,CtrlSys_Sleeping,
}sys_state_t;

typedef struct unit
{
        void (*Init)(struct unit *self);
        void (*Run)(struct unit *self,void *data);
        void (*Start)(struct unit *self);
        void (*Stop)(struct unit *self);
        void (*Sleep)(struct unit *self,uint32_t time);
        void (*WakeUp)(struct unit *self);
        sys_state_t State;
        struct unit *Self;
}unit_t;

typedef struct ctrlsys
{
        void (*Init)(struct ctrlsys *self);
        unit_t SensorUnit;
        unit_t DecisionUnit;
        unit_t ExecutionUnit;
        //void *UsrData;
        struct ctrlsys *Self;
}ctrlsys_t;

extern ctrlsys_t ServoSys;

extern ctrlsys_t MotorSys;

void ServoCtrlSysInit(void);
void MotorCtrlSysInit(void);

#endif /* APP_SYS_CTRLSYS_H_ */

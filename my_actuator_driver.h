#ifndef MY_ACTUATOR_DRIVER_H
#define MY_ACTUATOR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ACTUATOR_STATE_OFF,
    ACTUATOR_STATE_ON,
    ACTUATOR_STATE_PULSE,
    ACTUATOR_STATE_ERROR
} ActuatorStateType;

bool MyActuator_Init(void);
bool MyActuator_SetState(ActuatorStateType state);
ActuatorStateType MyActuator_GetState(void);
void MyActuator_DeInit(void);


#endif /* MY_ACTUATOR_DRIVER_H */

#include "my_actuator_driver.h"

#define MY_ACTUATOR_PIN    0

static bool actuator_initialized = false;
static ActuatorStateType current_actuator_state = ACTUATOR_STATE_OFF;


bool MyActuator_Init(void) {


    MyActuator_SetState(ACTUATOR_STATE_OFF);
    actuator_initialized = true;
    return true;
}

bool MyActuator_SetState(ActuatorStateType state) {
    if (!actuator_initialized) {
        return false;
    }


    current_actuator_state = state;
    switch (state) {
        case ACTUATOR_STATE_ON:

            break;
        case ACTUATOR_STATE_OFF:

            break;
        case ACTUATOR_STATE_PULSE:

            break;
        case ACTUATOR_STATE_ERROR:

            break;
        default:
            return false;
    }
    return true;
}

ActuatorStateType MyActuator_GetState(void) {
    if (!actuator_initialized) {
        return ACTUATOR_STATE_ERROR;
    }

    return current_actuator_state;
}


void MyActuator_DeInit(void) {

    MyActuator_SetState(ACTUATOR_STATE_OFF);
    actuator_initialized = false;

}

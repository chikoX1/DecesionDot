#include <stdint.h>
#include <stdbool.h>


void platform_delay_ms(uint32_t ms);


#include "decisiondot.h"
#include "my_sensor_driver.h"
#include "my_actuator_driver.h"


void platform_delay_ms(uint32_t ms) {

    #ifndef REAL_TARGET_PLATFORM
    volatile uint32_t count = ms * 50000;
    while(count--) {
        __asm__ __volatile__ ("nop");
    }
    #endif
}


void app_setup(void) {




    if (!MySensor_Init()) {

        while(1);
    } else {

    }


    if (!MyActuator_Init()) {

        while(1);
    } else {

    }


    int16_t initial_sensor_val = MySensor_ReadValue();
    DecisionDot_Init(initial_sensor_val);




}

void app_loop(void) {


    int16_t current_sensor_value = MySensor_ReadValue();



    DecisionDot_ProcessSensorInput(current_sensor_value);
    int16_t filtered_value = DecisionDot_GetFilteredValue();



    DecisionDot_StateType current_state = DecisionDot_GetState();




    switch (current_state) {
        case DECISIONDOT_STATE_IDLE:
            MyActuator_SetState(ACTUATOR_STATE_OFF);

            break;

        case DECISIONDOT_STATE_ACTIVE:

            MyActuator_SetState(ACTUATOR_STATE_ON);

            break;

        case DECISIONDOT_STATE_ALERT:
            MyActuator_SetState(ACTUATOR_STATE_OFF);



            if (DecisionDot_GetStatusFlags() & DECISIONDOT_FLAG_THRESHOLD_CROSSED) {



            }
            break;

        default:

            MyActuator_SetState(ACTUATOR_STATE_ERROR);

            break;
    }



    platform_delay_ms(2000);


}




int main(void) {
    app_setup();

    while (1) {
        app_loop();
    }


}


/* Example for ESP-IDF (FreeRTOS)
void app_main(void) {
    app_setup();

    while(1) {
        app_loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
*/

#include "my_sensor_driver.h"

static bool sensor_initialized = false;
static int16_t mock_sensor_value = 250;
static bool data_ready_flag = false;

bool MySensor_Init(void) {

    sensor_initialized = true;
    data_ready_flag = true;
    return true;
}

int16_t MySensor_ReadValue(void) {
    if (!sensor_initialized) {
        return -1;
    }

    mock_sensor_value += 10;
    if (mock_sensor_value > 1000) {
        mock_sensor_value = 150;
    }

    data_ready_flag = false;

    return mock_sensor_value;
}


bool MySensor_IsDataReady(void) {

    if (!sensor_initialized) {
        return false;
    }
    return data_ready_flag;
}


void MySensor_StartReading(void) {

    if (sensor_initialized) {
        data_ready_flag = true;
    }

}


void MySensor_DeInit(void) {

    sensor_initialized = false;
}

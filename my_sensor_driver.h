#ifndef MY_SENSOR_DRIVER_H
#define MY_SENSOR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

bool MySensor_Init(void);
int16_t MySensor_ReadValue(void);
bool MySensor_IsDataReady(void);
void MySensor_StartReading(void);
void MySensor_DeInit(void);

#endif /* MY_SENSOR_DRIVER_H */

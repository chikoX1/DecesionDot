#ifndef DECISIONDOT_H
#define DECISIONDOT_H

#include <stdint.h>
#include <stdbool.h>

#define DECISIONDOT_EMA_ALPHA_Q15       3277
#define DECISIONDOT_ALERT_THRESHOLD_HIGH    850
#define DECISIONDOT_ALERT_THRESHOLD_LOW     830
#define DECISIONDOT_ACTIVE_THRESHOLD        300

typedef enum {
    DECISIONDOT_STATE_IDLE,
    DECISIONDOT_STATE_ACTIVE,
    DECISIONDOT_STATE_ALERT
} DecisionDot_StateType;

typedef enum {
    DECISIONDOT_FLAG_NONE              = 0,
    DECISIONDOT_FLAG_INITIALIZED       = (1 << 0),
    DECISIONDOT_FLAG_EMA_STABLE        = (1 << 1),
    DECISIONDOT_FLAG_THRESHOLD_CROSSED = (1 << 2)
} DecisionDot_StatusFlagsType;

void DecisionDot_Init(int16_t initialValue);
void DecisionDot_ProcessSensorInput(int16_t rawValue);
DecisionDot_StateType DecisionDot_GetState(void);
DecisionDot_StatusFlagsType DecisionDot_GetStatusFlags(void);
int16_t DecisionDot_GetFilteredValue(void);

#endif /* DECISIONDOT_H */

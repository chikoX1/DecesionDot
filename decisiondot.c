#include "decisiondot.h"

#define Q15_ONE (1 << 15)

typedef struct {
    DecisionDot_StateType       currentState;
    DecisionDot_StatusFlagsType statusFlags;
    int32_t                     emaFilteredValueQ15;
    int16_t                     lastFilteredValue;
} DecisionDot_InternalState_t;

static DecisionDot_InternalState_t dd_state;

static int16_t ema_update(int16_t newValue) {
    int32_t newValueQ15 = (int32_t)newValue << 15;
    int16_t filteredResult;

    if (!(dd_state.statusFlags & DECISIONDOT_FLAG_EMA_STABLE)) {
        dd_state.emaFilteredValueQ15 = newValueQ15;
        dd_state.statusFlags |= DECISIONDOT_FLAG_EMA_STABLE;
    } else {
        int64_t term1 = (int64_t)DECISIONDOT_EMA_ALPHA_Q15 * newValueQ15;
        int64_t term2 = (int64_t)(Q15_ONE - DECISIONDOT_EMA_ALPHA_Q15) * dd_state.emaFilteredValueQ15;
        dd_state.emaFilteredValueQ15 = (int32_t)((term1 + term2) >> 15);
    }

    filteredResult = (int16_t)((dd_state.emaFilteredValueQ15 + (1 << 14)) >> 15);
    dd_state.lastFilteredValue = filteredResult;
    return filteredResult;
}

static void update_fsm_state(int16_t filteredValue) {
    switch (dd_state.currentState) {
        case DECISIONDOT_STATE_IDLE:
            if (filteredValue >= DECISIONDOT_ALERT_THRESHOLD_HIGH) {
                dd_state.currentState = DECISIONDOT_STATE_ALERT;
                dd_state.statusFlags |= DECISIONDOT_FLAG_THRESHOLD_CROSSED;
            } else if (filteredValue >= DECISIONDOT_ACTIVE_THRESHOLD) {
                dd_state.currentState = DECISIONDOT_STATE_ACTIVE;
            }
            break;

        case DECISIONDOT_STATE_ACTIVE:
            if (filteredValue >= DECISIONDOT_ALERT_THRESHOLD_HIGH) {
                dd_state.currentState = DECISIONDOT_STATE_ALERT;
                dd_state.statusFlags |= DECISIONDOT_FLAG_THRESHOLD_CROSSED;
            } else if (filteredValue < DECISIONDOT_ACTIVE_THRESHOLD) {
                dd_state.currentState = DECISIONDOT_STATE_IDLE;
            }
           break;

        case DECISIONDOT_STATE_ALERT:
            if (filteredValue < DECISIONDOT_ALERT_THRESHOLD_LOW) {
                 dd_state.statusFlags &= ~DECISIONDOT_FLAG_THRESHOLD_CROSSED;
                 if (filteredValue >= DECISIONDOT_ACTIVE_THRESHOLD) {
                     dd_state.currentState = DECISIONDOT_STATE_ACTIVE;
                 } else {
                     dd_state.currentState = DECISIONDOT_STATE_IDLE;
                 }
            }
           break;

        default:
            dd_state.currentState = DECISIONDOT_STATE_IDLE;
            break;
    }
}

void DecisionDot_Init(int16_t initialValue) {
    dd_state.currentState = DECISIONDOT_STATE_IDLE;
    dd_state.statusFlags = DECISIONDOT_FLAG_NONE;

    dd_state.emaFilteredValueQ15 = (int32_t)initialValue << 15;
    dd_state.lastFilteredValue = initialValue;

     if (initialValue >= DECISIONDOT_ALERT_THRESHOLD_HIGH) {
         dd_state.currentState = DECISIONDOT_STATE_ALERT;
         dd_state.statusFlags |= DECISIONDOT_FLAG_THRESHOLD_CROSSED;
     } else if (initialValue >= DECISIONDOT_ACTIVE_THRESHOLD) {
         dd_state.currentState = DECISIONDOT_STATE_ACTIVE;
     } else {
         dd_state.currentState = DECISIONDOT_STATE_IDLE;
     }

    dd_state.statusFlags |= DECISIONDOT_FLAG_INITIALIZED;
}

void DecisionDot_ProcessSensorInput(int16_t rawValue) {
    if (!(dd_state.statusFlags & DECISIONDOT_FLAG_INITIALIZED)) {
        return;
    }

    if(dd_state.currentState != DECISIONDOT_STATE_ALERT) {
         dd_state.statusFlags &= ~DECISIONDOT_FLAG_THRESHOLD_CROSSED;
    }

    int16_t filteredValue = ema_update(rawValue);
    update_fsm_state(filteredValue);
}

DecisionDot_StateType DecisionDot_GetState(void) {
    return dd_state.currentState;
}

DecisionDot_StatusFlagsType DecisionDot_GetStatusFlags(void) {
    return dd_state.statusFlags;
}

int16_t DecisionDot_GetFilteredValue(void) {
    return dd_state.lastFilteredValue;
}

# DecisionDot - Lightweight State Management for Embedded Systems

DecisionDot is a compact and efficient C module designed for resource-constrained embedded systems (like ESP32, STM32). It helps manage device state based on smoothed sensor inputs and predefined thresholds, perfect for IoT applications with tight memory and power budgets.

It provides a simple way to interpret sensor data, filter out noise using an Exponential Moving Average (EMA), and determine an operational state (e.g., Idle, Active, Alert) using configurable thresholds with hysteresis.

---
**❗️ Development Status Notice ❗️**

Please be aware that the current version of `DecisionDot` publicly available serves as a **base model / reference implementation**. While functional for the core logic described, it may contain minor bugs or require further refinement for specific edge cases as it's under active development.

**A fully stable and comprehensively tested version is anticipated for release within the next 2 to 3 months.** Feedback and bug reports on this base version are welcome via Issues.

---
**Commercial Use & Support **

Companies interested in utilizing robust state-management logic based on `DecisionDot` principles for commercial products are invited to inquire about a dedicated, commercially licensed version.

Potential benefits of a commercial license include:
*   Enhanced stability and rigorous testing.
*   Regular security updates and patches (e.g., monthly cadence).
*   Dedicated technical support.
*   Access to potentially more advanced algorithms or features tailored to specific needs.

Please reach out directly to discuss your requirements and commercial licensing options.

---

## Key Features (Base Model)

*   **Finite State Machine (FSM):** Manages operational states (IDLE, ACTIVE, ALERT).
*   **Sensor Smoothing:** Includes a configurable fixed-point Exponential Moving Average (EMA) filter.
*   **Threshold Logic:** Implements state transitions based on HIGH/LOW thresholds with hysteresis.
*   **Status Flags:** Provides flags for conditions like initialization and threshold crossing.
*   **Lightweight:** Designed for minimal RAM (< 128KB available) and Flash footprint.
*   **Efficient:** Fast execution (< 100ms goal) suitable for low-power operation.
*   **Pure C:** Written in standard C (C99 recommended).
*   **No Dynamic Allocation:** Uses only static variables and fixed buffers.
*   **Modular:** Simple header (`decisiondot.h`) and implementation (`decisiondot.c`).

## Intended Use Cases (Base Model)

*   Prototyping smart thermostat control logic.
*   Implementing simple anomaly detection triggers.
*   Basic device status monitoring (Active vs. Idle).
*   Foundation for state logic in hobbyist or evaluation projects.

## File Structure

This project includes the core module and example usage:

YourProjectRoot/
└── src/ # Source files
├── main.c # Example main application logic
├── decisiondot.c # DecisionDot module implementation
├── decisiondot.h # DecisionDot module public interface
├── my_sensor_driver.c # PLACEHOLDER for your sensor reading code
├── my_sensor_driver.h # PLACEHOLDER header for sensor driver
├── my_actuator_driver.c # PLACEHOLDER for your actuator control code
└── my_actuator_driver.h # PLACEHOLDER header for actuator driver

*(Note: Build configuration like `platformio.ini` or `Makefile` would be in `YourProjectRoot`)*

## How to Use (Base Model)

1.  **Include:** Add `decisiondot.c` to your build process and include `decisiondot.h` in files that need to interact with the module (e.g., `main.c`).
2.  **Implement Drivers:** **Crucially, you must replace the placeholder code in `my_sensor_driver.c` and `my_actuator_driver.c` with code specific to your hardware**, using your target platform's HAL or SDK (e.g., STM32 HAL, ESP-IDF drivers).
3.  **Initialize:** Call `DecisionDot_Init()` once during setup with an initial sensor reading.
    ```c
    #include "decisiondot.h"
    #include "my_sensor_driver.h" // Your implemented driver

    void app_setup() {
        MySensor_Init(); // Initialize your sensor
        int16_t initial_value = MySensor_ReadValue(); // Get first reading
        DecisionDot_Init(initial_value);
        // ... other setup ...
    }
    ```
4.  **Process Inputs:** In your main loop or a periodic task, read the sensor and process the value.
    ```c
    #include "my_actuator_driver.h" // Your implemented driver

    void app_loop() {
        int16_t raw_value = MySensor_ReadValue();
        DecisionDot_ProcessSensorInput(raw_value);

        // Get state and react
        DecisionDot_StateType state = DecisionDot_GetState();
        switch(state) {
            case DECISIONDOT_STATE_IDLE:
                MyActuator_SetState(ACTUATOR_STATE_OFF); // Example action
                break;
            case DECISIONDOT_STATE_ACTIVE:
                MyActuator_SetState(ACTUATOR_STATE_ON); // Example action
                break;
            case DECISIONDOT_STATE_ALERT:
                MyActuator_SetState(ACTUATOR_STATE_OFF); // Example safety action
                // Check flag for first entry into alert
                if (DecisionDot_GetStatusFlags() & DECISIONDOT_FLAG_THRESHOLD_CROSSED) {
                    // Send alert notification, etc.
                }
                break;
        }
        // Get filtered value if needed: int16_t smooth_val = DecisionDot_GetFilteredValue();
        // Add platform delay/sleep: platform_delay_ms(1000);
    }
    ```

## Configuration (Base Model)

Adjust the behavior by modifying the `#define` constants at the top of `decisiondot.h`:

*   `DECISIONDOT_EMA_ALPHA_Q15`: Controls EMA smoothing (lower value = more smoothing). Value is in Q1.15 format.
*   `DECISIONDOT_ALERT_THRESHOLD_HIGH`: Sensor value level to trigger the ALERT state.
*   `DECISIONDOT_ALERT_THRESHOLD_LOW`: Sensor value level to exit the ALERT state (hysteresis).
*   `DECISIONDOT_ACTIVE_THRESHOLD`: Sensor value level to transition between IDLE and ACTIVE states.

*(Note: Threshold values should match the scale of the `int16_t` sensor values passed to `DecisionDot_ProcessSensorInput`.)*

## Testing (Base Model)

*   **Host-Based:** The provided driver files (`my_sensor_driver.c`, `my_actuator_driver.c`) can be modified with mock implementations (using `printf`, fixed sequences, etc.) to allow compilation and execution on a host PC (using GCC/Clang). This verifies the core logic of `decisiondot` and `main.c`. See the `main.c` file for an example host `platform_delay_ms` and a limited loop.
*   **Target Hardware (Essential):** Full testing **requires** the target hardware (ESP32, STM32). Use your chosen development environment (PlatformIO, CubeIDE, etc.) to build, flash, and debug directly on the MCU. This is necessary to validate sensor/actuator driver interaction, real-time performance, memory usage, and power consumption.

## Dependencies

*   Standard C Compiler (C99 or later recommended).
*   Standard C headers: `<stdint.h>`, `<stdbool.h>`.
*   Target-specific Hardware Abstraction Layer (HAL) or SDK for sensor/actuator drivers (e.g., ESP-IDF, STM32Cube HAL).

## Contributing

Feedback on the base model (via GitHub Issues) is welcome during the active development phase. For commercial use or support inquiries, please refer to the "Commercial Use & Support" section above.

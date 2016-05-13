#pragma once

/* *
 * Set and initialize for vibration motors
 * "Set gpio pins."
 */
void hardwareSetup();

/* *
 * Change states for gpio pins for driving motor
 */
int vibrationStateChange(int motor_state);

/* *
 * Reset states of gpio
 */
void reset();

/* *
 * Delay func
 */
void delayMS(int ms);

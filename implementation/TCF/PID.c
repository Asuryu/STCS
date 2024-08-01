#include "PID.h"

#include <stdlib.h>

int PID(int goal_temperature, int freq, int *current_temperature, int *state) {
    int temperature_size = sizeof(current_temperature) / sizeof(current_temperature[0]);
    float *error = (float *)malloc(temperature_size * sizeof(float));
    float *goal_temperature_array = (float *)malloc(temperature_size * sizeof(float));

    float Kp = 3.0; 
    float Ki = 1.0; 
    float Kd = 0.05;

    for(int i = 0; i < temperature_size; i++) {
        goal_temperature_array[i] = goal_temperature;
    }

    for(int i = 2; i < temperature_size; i++) {
        error[i] = goal_temperature_array[i] - current_temperature[i];
        state[i] = state[i - 1] + error[i] * (Kp + Ki * freq / 2 + 2 * Kd / freq) +
                    error[i - 1] * (-Kp + Ki * freq / 2 - 4 * Kd / freq) +
                    error[i - 2] * Kd / freq;
    }

    return state[temperature_size - 1];
}
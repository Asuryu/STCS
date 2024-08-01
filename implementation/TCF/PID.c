#include "PID.h"
#include <stdlib.h>
#include <stdio.h>

int PID(int goal_temperature, int freq, int *current_temperature, int *state, int temperature_size) {
    float *error = (float *)malloc(temperature_size * sizeof(float));
    if (error == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return -1;
    }

    float Kp = 3.0;
    float Ki = 0.005;
    float Kd = 0.0;

    for (int i = 0; i < temperature_size; i++) {
        error[i] = goal_temperature - current_temperature[i];
    }

    float proportional = Kp * error[temperature_size - 1];
    float integral = 0.0;
    for (int i = 0; i < temperature_size; i++) {
        integral += error[i];
    }
    integral *= Ki / freq;

    float derivative = 0.0;
    if (temperature_size > 1) {
        derivative = Kd * (error[temperature_size - 1] - error[temperature_size - 2]) * freq;
    }
    float control_signal = proportional + integral + derivative;

    int final_state = (control_signal > 0) ? 1 : -1;

    free(error);
    return final_state;
}
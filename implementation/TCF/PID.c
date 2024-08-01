#include "PID.h"

#include <stdlib.h>
#include <stdio.h>

int PID(int goal_temperature, int freq, int *current_temperature, int *state) {
    int temperature_size = sizeof(current_temperature) / sizeof(current_temperature[0]);
    float *error = (float *)malloc(temperature_size * sizeof(float));

    int final_state;

    float Kp = 3.0; 
    float Ki = 0.005; 
    float Kd = 0;
    //float Kd = 0.05;

    printf("Data:\nFreq: %d \t Kp: %f ; Ki: %f ; Kd: %f\nErrors: ", freq, Kp, Ki, Kd );

    for(int i = 0; i < temperature_size; i++) {
        error[i] = goal_temperature - current_temperature[i];
        printf("%f \t", error[i]);
    }
    
    printf("\n\n[1] %f\n[2] %f\n[3] %f\n" , state[0] + error[2] * (Kp + Ki * freq / 2 + 2 * Kd / freq),  error[1] * (Ki * freq - 4 * Kd / freq), error[0] * (Kd * 2 / freq + Ki * freq / 2 - Kp));

    final_state = state[sizeof(current_temperature) / sizeof(current_temperature[0])-2] + error[2] * (Kp + Ki * (freq / 2) + 2 * (Kd / freq)) 
                    + error[1] * (Ki * freq - 4 * (Kd / freq))
                    + error[0] * (Kd * (2 / freq) + Ki * (freq / 2) - Kp);
    
     free(error);

    return final_state;
}
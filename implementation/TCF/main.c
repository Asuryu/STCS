#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PID.h"

// Update the temperature based on the heater state and aun exposure period (ECLIPSE/NORMAL)
void update_temperature(int *current_temperature, int *state, int size, int period) {
    for (int i = 0; i < size; i++) {
        if (period == 0) { // NORMAL
            current_temperature[i] += (state[i] == 1) ? 1 : -1;
        } else if (period == 1) { // ECLIPSE
            current_temperature[i] += (state[i] == 1) ? 4 : -7;
        }
    }
}

// Saves temperature and time to a temperature.txt file
void log_temperature(const char *filename, int *current_temperature, int size, float time) {
    FILE *file = fopen(filename, "a");
    
    fprintf(file, "Time: %.1f seconds, Current Temperatures: ", time);
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", current_temperature[i]);
    }
    fprintf(file, "\n");
    
    fclose(file);
}

int main() {
    int freq = 5; //f in Hz
    int goal_temperature = 50; // Setpoint chosen
    int period = 0; // 0 for NORMAL, 1 for ECLIPSE
    int size = 3; // Array Size (Depends on what comes from TSL)
    int current_temperature[1000] = {1, 2, 3};
    int state[1000] = {0, 0, 0}; // Initial States (0 --> OFF)
    float time_elapsed = 0.0;

    // Clean the archive before simul
    FILE *file = fopen("temperature.txt", "w");
    if (file != NULL) {
        fclose(file);
    } else {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }


    while (1) {

        int final_state = PID(goal_temperature, freq, current_temperature, state, size);

        for (int i = 0; i < size; i++) {
            state[i] = final_state;
        }

        update_temperature(current_temperature, state, size, period);

        log_temperature("temperatura.txt", current_temperature, size, time_elapsed);
        time_elapsed += 1.0 / freq;

        // Switch between NORMAL and ECLIPSE for simulation
        period = 1 - period;

        // Simulate a delay corresponding to the frequency range - programe sleeps during 0.2s if freq=5Hz
        struct timespec delay = {0, 1000000000 / freq};
        nanosleep(&delay, NULL);
    }

    printf("Simulation Completed. Results on temperature.txt.\n");

    return 0;
}

#include <stdio.h>

#include "PID.h"
#include <stdlib.h>

int main() {

    int freq = 5;
    int goal_temperature = 10; 
    int size = 1000; 
    int current_temperature[size];
    current_temperature[0]=1;
    current_temperature[1]=2;
    current_temperature[2]=3;
    int state[3] = {0, 0, 0};

    int final_state = PID(goal_temperature, freq, current_temperature, state);
    printf("Final state: %d\n", final_state);

    for (int j=3;j<size;j++){

    final_state = PID(goal_temperature, freq, current_temperature, state);
    printf("Final state: %d\n", final_state);

    if (final_state>0){
        current_temperature[j]= current_temperature[j-1]+2;}
    if (final_state<0){
        current_temperature[j]= current_temperature[j-1]-2;}
    }

    FILE *file = fopen("temperaturas.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    for (int j = 0; j < size; j++) {
        fprintf(file, "%d\n", current_temperature[j]);
    }

    fclose(file);

    return 0;
}
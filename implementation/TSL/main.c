#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include "config.h"


// Define states and Bool Type
#define NORMAL 1
#define ECLIPSE 2
#define SUN_EXPOSURE 3

typedef enum {false, true} bool;


// Define Structcs
struct ThermalPair {
    bool heater;
    double thermistor;
};

struct TSL_data{
    uint16_t clock;
    int period;     //period being analyzed
};


// Inicialize all the functions
void TSL_init(struct TSL_data *tsl);
void verify_periods(struct TSL_data *tsl);
int setHeaterState(struct ThermalPair TP_block);
int modify_temperatures(struct ThermalPair* TP_block, int state);
int write_csv(struct ThermalPair TP_block);


// Function to initialize clock and period
void TSL_init(struct TSL_data *tsl){
    tsl->clock = 0;
    tsl->period = NORMAL;
}


// Function verify the clock period and define the state
void verify_periods(struct TSL_data *tsl) {
    uint8_t lowerBits = tsl->clock & 0x00FF;
 
    if ((lowerBits >= 0x00 && lowerBits <= 0x1F) || (lowerBits >= 0x60 && lowerBits <= 0xFF)) {
        tsl->period = NORMAL;
    } else if (lowerBits >= 0x20 && lowerBits <= 0x3F) {
        tsl->period = ECLIPSE;
    } else if (lowerBits >= 0x40 && lowerBits <= 0x5F) {
        tsl->period = SUN_EXPOSURE;
    }
}


// Function to update temperatures based on the state -> 0: Normal    1: Eclipse    2: Sun Exposure  
int modify_temperatures(struct ThermalPair* TP_block, int state){
    
    switch (state) {
    case NORMAL:
        if (TP_block->heater){
            TP_block->thermistor += 1;
        }
        else{
            TP_block->thermistor -= 1;
        }
        return 0;
        break;
    case ECLIPSE:
        if (TP_block->heater){
            TP_block->thermistor += 4;
        }
        else{
            TP_block->thermistor -= 7;
        }
        return 0;
        break;
    case SUN_EXPOSURE:
        if (TP_block->heater){
            TP_block->thermistor += 7;
        }
        else{
            TP_block->thermistor -= 1;
        }
        return 0;
        break;
    default:
        return -1;
        break;
    }
}


// Functions and variables for Pipe
int fd_temp_info_pipe, fd_response_pipe;

int main() {

    // printf("TSL started\n");

    if((mkfifo(TEMP_INFO_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){
		
        perror("ERROR CREATING TEMP INFO NAMED PIPE"); 
		exit(0);
	}

    // printf("TSL created temp_info_pipe\n");

    if((mkfifo(RESPONSE_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){

		perror("ERROR CREATING RESPONSE NAMED PIPE"); 
		exit(0);
    }

    // printf("TSL created response_pipe\n");

    if ((fd_temp_info_pipe = open(TEMP_INFO_PIPE, O_RDWR)) < 0) {
        
        perror("ERROR OPENING SENSOR NAMED PIPE");
        exit(0);
    }
    // printf("TSL opened temp_info_pipe\n");

    if ((fd_response_pipe = open(RESPONSE_PIPE, O_RDWR)) < 0) {
        
        perror("ERROR OPENING CONSOLE NAMED PIPE");
        exit(0);
    }
    // printf("TSL opened response_pipe\n");

    // sleep(10);


    // char message[100];
    // sprintf(message, "TSL: %d", clock);
    // printf("TSL sending: %s\n", message);
    // write(fd_temp_info_pipe, message, strlen(message)+1);


    // Initialize the Heaters Sstate and the Thermistors temperature
    struct ThermalPair pair_array[4] = {
        {false, rand() % 13 - 5},    // random between -5 and 7
        {false, rand() % 13 - 5},
        {false, rand() % 13 - 5},
        {false, rand() % 13 - 5}
    };

    struct TSL_data tsl;
    TSL_init(&tsl);

    // Auxiliar for Now
    int temp_result = 0;
    int error_array[2] = {0,0};

    // Possible Errors
    // [0] -> ????
    // [1] -> temperature error

    while(1){
        usleep(200 * 1000);
        
        verify_periods(&tsl);

        // something to get the data from pive
        // setHeaterState
        
        // Modify Temperatures
        for(int i=0; i<4; i++){
            if(modify_temperatures(&pair_array[i], tsl.period) == -1){
                printf("setTemperature ERROR: Unknown state\n");  
                error_array[1] += 1;  
            }
        } 

        // write_csv
        
        tsl.period++;
        printf("%d",tsl.period);
    }

    close(fd_temp_info_pipe);    
    close(fd_response_pipe);

    unlink(TEMP_INFO_PIPE);
	unlink(RESPONSE_PIPE);

    return 0;
}   
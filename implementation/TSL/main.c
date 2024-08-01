#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "config.h"


// Function to initialize clock and period
void TSL_init(struct TSL_data *tsl){
    tsl->clock = 0;
    tsl->period = NORMAL;
}


// Function verify the clock period and define the state
void verify_periods(struct TSL_data *tsl) {
    uint8_t lowerBits = tsl->clock & 0x00FF;
 
    if (lowerBits <= 0x1F || lowerBits >= 0x60){
        tsl->period = NORMAL;
    } else if (lowerBits >= 0x20 && lowerBits <= 0x3F) {
        tsl->period = ECLIPSE;
    } else if (lowerBits >= 0x40 && lowerBits <= 0x5F) {
        tsl->period = SUN_EXPOSURE;
    }
}


// Function to turn ON/OFF each Heater
void setHeaterState(struct ThermalPair *TP_block, char *heater_state){
    TP_block[0].heater = heater_state[0];
    TP_block[1].heater = heater_state[1];
    TP_block[2].heater = heater_state[2];
    TP_block[3].heater = heater_state[3];   
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


const char* get_timestamp() {
    
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int milliseconds = tv.tv_usec / 1000; 

    char time_string[100];
    strftime(time_string, sizeof(time_string), "%Y-%m-%dT%H:%M:%S", local_time);

    char final_string[120]; 
    snprintf(final_string, sizeof(final_string), "%s.%03d", time_string, milliseconds);

    return final_string;
}


int file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}
 
 
// Build the data string
char * buildData (struct ThermalPair* TP_block, char *timestamp, int state){
 
     char buffer[512];
 
    snprintf(buffer, sizeof(buffer), "%lf, %lf, %lf, %lf, %d, %d, %d, %d, %s, %d, null\n", TP_block[0].heater, TP_block[1].heater, 
    TP_block[2].heater, TP_block[3].heater, TP_block[0].thermistor, TP_block[1].thermistor, TP_block[2].thermistor, TP_block[3].thermistor, timestamp, state);
 
    printf("%s\n", buffer);
   
    return buffer;
 
}
 
// function to write in cvs (correct)
void writeToCSVCorrect(const char *filename, const char *header, const char *data){
 
    FILE *file;
 
    int exists = file_exists(filename);
    file = fopen(filename, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(-1);
    }
    if (!exists) {
        fprintf(file, "%s\n", header);
    }
 
    fprintf(file, "%s\n", data);
 
    fclose(file);
 
}
 
// function to write in cvs (incorrect)
void writeToCSVError(const char *filename, const char *header, char *error, char *timestamp){
 
    FILE *file;
 
    char buffer[512];
   
    snprintf(buffer, sizeof(buffer), "null, null, null, null, null, null, null, null, %s, null, %s\n", timestamp, error);
 
 
    int exists = file_exists(filename);
    file = fopen(filename, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(-1);
    }
    if (!exists) {
        fprintf(file, "%s\n", header);
    }
 
    fprintf(file, "%s\n", buffer);
 
    fclose(file);
 
}


// Functions and variables for Pipe
int fd_temp_info_pipe, fd_response_pipe;

int main() {
    
    char *data;
    const char *filename = "data.csv";
    const char *header = "THERM-01, THERM-02, THERM-03, THERM-04, HTR-1, HTR-2, HTR-3, HTR-4, TIMESTAMP, ENVIRONMENT, ERROR";

    // printf("TSL started\n");

    if((mkfifo(TEMP_INFO_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){
		
        char *error_msg = "ERROR CREATING TEMP INFO NAMED PIPE";
        perror(error_msg); 
        write_csv_errors(filename, header, error_msg, get_timestamp());
		exit(0);
	}

    // printf("TSL created temp_info_pipe\n");

    if((mkfifo(RESPONSE_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){
        
        char *error_msg = "ERROR CREATING RESPONSE NAMED PIPE";
        perror(error_msg); 
        write_csv_errors(filename, header, error_msg, get_timestamp());
		exit(0);
    }

    // printf("TSL created response_pipe\n");

    if ((fd_temp_info_pipe = open(TEMP_INFO_PIPE, O_RDWR)) < 0) {
        
        char *error_msg = "ERROR OPENING SENSOR NAMED PIPE";
        perror(error_msg); 
        write_csv_errors(filename, header, error_msg, get_timestamp());
        exit(0);
    }
    // printf("TSL opened temp_info_pipe\n");

    if ((fd_response_pipe = open(RESPONSE_PIPE, O_RDWR)) < 0) {
        
        char *error_msg = "ERROR OPENING CONSOLE NAMED PIPE";
        perror(error_msg); 
        write_csv_errors(filename, header, error_msg, get_timestamp());
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

    char *data;
    const char *filename = "data.csv";
    const char *header = "THERM-01, THERM-02, THERM-03, THERM-04, HTR-1, HTR-2, HTR-3, HTR-4, TIMESTAMP, ENVIRONMENT, ERROR";

    // Auxiliar for Now
    char heaters_state[4] = {0,0,1,1};


    while(1){
        usleep(200 * 1000);
        
        verify_periods(&tsl);

        // something to get the data from pipe

        setHeaterState(pair_array, heaters_state);
        
        for(int i=0; i<4; i++){
            if(modify_temperatures(&pair_array[i], tsl.period) == -1){
                write_csv_errors(filename, header, "setTemperature ERROR: Unknown state", get_timestamp());
            }
        } 

        data = buildData(&pair_array, get_timestamp(), tsl.period); 
        write_csv_correct(filename, header, data);

        tsl.period++;
        printf("%d",tsl.period);
    }

    close(fd_temp_info_pipe);    
    close(fd_response_pipe);

    unlink(TEMP_INFO_PIPE);
	unlink(RESPONSE_PIPE);

    return 0;
}   

#include "config.h"


// Function to initialize clock and period
void TSL_init(struct TSL_data *tsl){
    tsl->clock = 0;
    tsl->period = NORMAL;
}



// Function verify the clock period and define the state
void verify_periods(struct TSL_data *tsl) {
    int8_t lowerBits = tsl->clock & 0x00FF;
    printf("lowerBits: %d\n", lowerBits);
 
    if (lowerBits <= 0x1F || (lowerBits >= 0x60 && lowerBits <= 0xFF)) {
        printf("NORMAL\n");
        tsl->period = NORMAL;
    } else if (lowerBits >= 0x20 && lowerBits <= 0x3F) {
        printf("ECLIPSE\n");
        tsl->period = ECLIPSE;
    } else if (lowerBits >= 0x40 && lowerBits <= 0x5F) {
        printf("SUN_EXPOSURE\n");
        tsl->period = SUN_EXPOSURE;
    }
}


// Function to turn ON/OFF each Heater
void setHeaterState(struct ThermalPair *TP_block, int *heater_state){
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
        break;
    case ECLIPSE:
        if (TP_block->heater){
            TP_block->thermistor += 4;
        }
        else{
            TP_block->thermistor -= 7;
        }
        break;
    case SUN_EXPOSURE:
        if (TP_block->heater){
            TP_block->thermistor += 7;
        }
        else{
            TP_block->thermistor -= 1;
        }
        break;
    default:
        return -1;
        break;
    }
    return 0;

}


// Functions and variables for Pipe
int fd_temp_info_pipe, fd_response_pipe;

void* read_response_thread(void* args) {
    ThreadArgs *thread_args = (ThreadArgs*)args;
    int fd = thread_args->fd;
    int *new_heater_states = thread_args->new_heater_states;

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);

    printf("TSL waiting for response\n");
    while(1){
        if (select(fd + 1, &read_set, NULL, NULL, NULL) > 0) {
            if (FD_ISSET(fd, &read_set)) {
                char message[100];
                read(fd, message, 100);

                // {STATE};{STATE};{STATE};{STATE}
                sscanf(message, "%d;%d;%d;%d", &new_heater_states[0], &new_heater_states[1], &new_heater_states[2], &new_heater_states[3]);
                printf("TSL received: %d %d %d %d\n", new_heater_states[0], new_heater_states[1], new_heater_states[2], new_heater_states[3]);
            }
        }
    }
    pthread_exit((void*)-1);
}

void write_temp_info_pipe(char* message) {
    write(fd_temp_info_pipe, message, strlen(message) + 1);
}

int main() {

    // printf("TSL started\n");

    if((mkfifo(TEMP_INFO_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){
		
        char *error_msg = "ERROR CREATING TEMP INFO NAMED PIPE";
        perror(error_msg); 
        //write_csv_errors(error_msg);
		exit(0);
	}

    // printf("TSL created temp_info_pipe\n");

    if((mkfifo(RESPONSE_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){
        
        char *error_msg = "ERROR CREATING RESPONSE NAMED PIPE";
        perror(error_msg); 
        //write_csv_errors(error_msg);
		exit(0);
    }

    // printf("TSL created response_pipe\n");

    if ((fd_temp_info_pipe = open(TEMP_INFO_PIPE, O_RDWR)) < 0) {
        
        char *error_msg = "ERROR OPENING SENSOR NAMED PIPE";
        perror(error_msg); 
        //write_csv_errors(error_msg);
        exit(0);
    }
    // printf("TSL opened temp_info_pipe\n");

    if ((fd_response_pipe = open(RESPONSE_PIPE, O_RDWR)) < 0) {
        
        char *error_msg = "ERROR OPENING CONSOLE NAMED PIPE";
        perror(error_msg); 
        //write_csv_errors(error_msg);
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

    int error_array[2] = {0,0};

    int new_heater_states[4] = {0,0,0,0};

    pthread_t thread;
    ThreadArgs thread_args = {fd_response_pipe, new_heater_states};


    if (pthread_create(&thread, NULL, read_response_thread, &thread_args) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    while(1){
        sleep(2);

        // Verify Periods
        verify_periods(&tsl);

        // something to get the data from pipe

        setHeaterState(pair_array, new_heater_states);

        // print pair_array
        for (int i = 0; i < 4; i++)
        {
            printf("Heater: %d, Thermistor: %f\n", pair_array[i].heater, pair_array[i].thermistor);
        }
        
        

        
        for(int i=0; i<4; i++){
            if(modify_temperatures(&pair_array[i], tsl.period) == -1){
                //write_csv_errors("setTemperature ERROR: Unknown state");
            }
        } 
        char message[100];
        //{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE}
        sprintf(message, "%f-%d;%f-%d;%f-%d;%f-%d", 
                    pair_array[0].thermistor, pair_array[0].heater, 
                    pair_array[1].thermistor, pair_array[1].heater, 
                    pair_array[2].thermistor, pair_array[2].heater, 
                    pair_array[3].thermistor, pair_array[3].heater);

        write_temp_info_pipe(message);

        // write_csv
        
        tsl.clock++;
        printf("TSL period: %d\n", tsl.clock);
    }

    void *status;
    if (pthread_join(thread, &status) != 0) {
        perror("Failed to join thread");
        return 1;
    }

    close(fd_temp_info_pipe);    
    close(fd_response_pipe);

    unlink(TEMP_INFO_PIPE);
	unlink(RESPONSE_PIPE);

    return 0;
}   
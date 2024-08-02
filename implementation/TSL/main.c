#include "config.h"

struct sigaction sa;
volatile sig_atomic_t sigint_received = 0;
pthread_t thread;

// Functions and variables for Pipe
int fd_temp_info_pipe, fd_response_pipe;

// Function to initialize clock and period
void TSL_init(struct TSL_data *tsl){
    tsl->clock = 0;
    tsl->period = NORMAL;
}

// Signal handler for SIGINT
void sigint_handler() {
    printf("SIGINT received\n");
    sigint_received = 1;
}

int cleanup() {

    pthread_join(thread, NULL);
    while (wait(NULL) != -1){
        continue;
    }
    close(fd_temp_info_pipe);
    close(fd_response_pipe);

    return 0;
}

void handle_error(char *error) {
    perror(error);
    cleanup();
    exit(0);
}



// Function verify the clock period and define the state
void verify_periods(struct TSL_data *tsl) {
    int8_t lowerBits = tsl->clock & 0x00FF;
 
    if (lowerBits <= 0x1F || lowerBits >= 0x60) {
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


// Function to update thermistor temperatures
//  based on Heater Status (ON/OFF) and the state -> 1: Normal    2: Eclipse    3: Sun Exposure  
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
    // Throws ERROR value: Unknown state
        return -1;
        break;
    }
    return 0;

}


const char * get_timestamp() {
    
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int milliseconds = tv.tv_usec / 1000; 

    char time_string[100];
    strftime(time_string, sizeof(time_string), "%Y-%m-%dT%H:%M:%S", local_time);

    char *final_string = malloc(120); 
    snprintf(final_string, 120, "%s.%03d", time_string, milliseconds);

    return final_string;
}


int file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}
 
 
// Build the data string
char * buildData (struct ThermalPair* TP_block,const char *timestamp, int state){
 
    char *buffer = malloc(512);

 
    snprintf(buffer, 512, "%f, %f, %f, %f, %s, %s, %s, %s, %s, %s, null", 
                        TP_block[0].thermistor, 
                        TP_block[1].thermistor, 
                        TP_block[2].thermistor, 
                        TP_block[3].thermistor,
                        TP_block[0].heater ? "On" : "Off", 
                        TP_block[1].heater ? "On" : "Off", 
                        TP_block[2].heater ? "On" : "Off", 
                        TP_block[3].heater ? "On" : "Off",                        
                        timestamp,
                        state == NORMAL ? "Normal" : (state == ECLIPSE ? "Eclipse" : (state == SUN_EXPOSURE ? "Sun Exposure" : "Unknown")));
 
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
void writeToCSVError(const char *filename, const char *header, char *error, const char *timestamp){
 
    FILE *file;
 
    char *buffer = malloc(512);
   
    snprintf(buffer,512, "null, null, null, null, null, null, null, null, %s, null, %s", timestamp, error);
    
    free((char *)timestamp);
 
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

    free(buffer);
}


// Functions and variables for Pipe
int fd_temp_info_pipe, fd_response_pipe;

void* read_response_thread(void* args) {

    struct sigaction sigint_action;
	sigint_action.sa_handler = sigint_handler;
	sigemptyset(&sigint_action.sa_mask);
	sigint_action.sa_flags = 0;
	if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
		handle_error("sigaction");
		exit(EXIT_FAILURE);
	}

    ThreadArgs *thread_args = (ThreadArgs*)args;
    int fd = thread_args->fd;
    int *new_heater_states = thread_args->new_heater_states;

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);

    printf("TSL waiting for response\n");
    while(!sigint_received) {
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
    close(fd);
    return NULL;
}

void write_temp_info_pipe(char* message) {
    write(fd_temp_info_pipe, message, strlen(message) + 1);
}

int main() {
    char *data;
    const char *filename = "data.csv";
    const char *header = "THERM-01, THERM-02, THERM-03, THERM-04, HTR-1, HTR-2, HTR-3, HTR-4, TIMESTAMP, ENVIRONMENT, ERROR";


    struct sigaction sigint_action;
	sigint_action.sa_handler = sigint_handler;
	sigemptyset(&sigint_action.sa_mask);
	sigint_action.sa_flags = 0;
	if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
        char *error_msg = "sigaction";
        writeToCSVError(filename, header, error_msg, get_timestamp());

		handle_error("sigaction");
		exit(EXIT_FAILURE);
	}

    // printf("TSL started\n");

    if((mkfifo(TEMP_INFO_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){
		
        char *error_msg = "ERROR CREATING TEMP INFO NAMED PIPE";
        writeToCSVError(filename, header, error_msg, get_timestamp());

        handle_error(error_msg); 
		exit(0);
	}

    // printf("TSL created temp_info_pipe\n");

    if((mkfifo(RESPONSE_PIPE,O_CREAT|O_EXCL|0600)<0) 
            && (errno != EEXIST)){
        
        char *error_msg = "ERROR CREATING RESPONSE NAMED PIPE";
        writeToCSVError(filename, header, error_msg, get_timestamp());

        handle_error(error_msg); 
		exit(0);
    }

    // printf("TSL created response_pipe\n");

    if ((fd_temp_info_pipe = open(TEMP_INFO_PIPE, O_RDWR)) < 0) {
        
        char *error_msg = "ERROR OPENING SENSOR NAMED PIPE";
        writeToCSVError(filename, header, error_msg, get_timestamp());

        handle_error(error_msg); 
        exit(0);
    }
    // printf("TSL opened temp_info_pipe\n");

    if ((fd_response_pipe = open(RESPONSE_PIPE, O_RDWR)) < 0) {
        
        char *error_msg = "ERROR OPENING CONSOLE NAMED PIPE";
        writeToCSVError(filename, header, error_msg, get_timestamp());

        handle_error(error_msg); 
        exit(0);
    }

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

    int new_heater_states[4] = {0,0,0,0};

    pthread_t thread;
    ThreadArgs thread_args = {fd_response_pipe, new_heater_states};


    if (pthread_create(&thread, NULL, read_response_thread, &thread_args) != 0) {
        handle_error("Failed to create thread");
        return 1;
    }

    while(!sigint_received){
        usleep(200 * 1000); // 200ms

        // Verify Periods
        verify_periods(&tsl);
        printf("Clock: %d\n", tsl.clock);
        printf("Period: %d\n", tsl.period);

        // something to get the data from pipe

        setHeaterState(pair_array, new_heater_states);

        // print pair_array
        for (int i = 0; i < 4; i++)
        {
            printf("Heater: %d, Thermistor: %f\n", pair_array[i].heater, pair_array[i].thermistor);
        }
        
        

         // 1 -> Normal 
         // 2 -> 
        
        for(int i=0; i<4; i++){
            if(modify_temperatures(&pair_array[i], tsl.period) == -1){
                writeToCSVError(filename, header, "setTemperature ERROR: Unknown state", get_timestamp());
            }
        } 
        char message[100];
        //{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE}
        sprintf(message, "%d;%f-%d;%f-%d;%f-%d;%f-%d", 
                    tsl.clock,
                    pair_array[0].thermistor, pair_array[0].heater, 
                    pair_array[1].thermistor, pair_array[1].heater, 
                    pair_array[2].thermistor, pair_array[2].heater, 
                    pair_array[3].thermistor, pair_array[3].heater);

        write_temp_info_pipe(message);

        data = buildData(pair_array, get_timestamp(), tsl.period); 
        writeToCSVCorrect(filename, header, data);

        tsl.clock++;        
    }

    if (cleanup() <0 ) {
        char *error_msg = "Failed to cleanup";
        handle_error(error_msg);
        writeToCSVError(filename, header, error_msg, get_timestamp());
        return -1;
    }


    return 0;
}   

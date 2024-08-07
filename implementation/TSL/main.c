#include "config.h"

struct sigaction sigint_action;
struct sigaction sigtstp_action;

// Initialization for threads
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


void sigint_handler() {/** 
    *@brief SIGINT handler function

    * This function will set the sigint_received variable to 1 when a SIGINT signal is received.

    *@param void 
    *@return void
    **/

    printf("SIGINT received\n");
    sigint_received = 1;
}


int cleanup() {/**
    * @brief Cleanup function

    * This function will close the file descriptors and unlink the named pipes.

    * @param void
    * @return int: 0 if successful, -1 otherwise
    **/

    pthread_join(thread, NULL);
    while (wait(NULL) != -1){
        continue;
    }
    close(fd_temp_info_pipe);
    close(fd_response_pipe);

    unlink(TEMP_INFO_PIPE);
    unlink(RESPONSE_PIPE);

    return 0;
}


void handle_error(char *error) {/** 
    * @brief Handle error function
    
    * This function will print the error message and call the cleanup function.

    * @param error (char*): The error message to be printed
    * @return void    
    **/

    perror(error);
    cleanup();
    exit(0);
}


void verify_periods(struct TSL_data *tsl) {
    /**
     * @brief Verifies and sets the period based on the 8 lower bits of the clock.
     
    * This function checks the lower 8 bits of the clock, of the
    * TSL_data structure, and sets the period accordingly.
    
    * @param tsl Pointer to the TSL_data structure whose clock's lower bits will be verified.
    
    * @return void
    */

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


void setHeaterState(struct ThermalPair *TP_block, int *heater_state){
    /**
     * @brief This function will turn ON/OFF each Heater. Updates each heater in ThermalPair according to states in heater_state
     
    * @param TP_block thermalPair matrix 
    * @param heater_state array of integers representing the heater's current states (heater_state)
    
    * @return void
    */

    TP_block[0].heater = heater_state[0];
    TP_block[1].heater = heater_state[1];
    TP_block[2].heater = heater_state[2];
    TP_block[3].heater = heater_state[3];   
}


int modify_temperatures(struct ThermalPair* TP_block, int state){

    /**
    * @brief This function will update the thermistor temperatures based on Heater Status (ON/OFF) and the state
    * -> 1: Normal  -> 2: Eclipse  -> 3: Sun Exposure
     
    * @param TP_block thermalPair matrix 
    * @param state array of integers representing the heater's current states (heater_state)
    
    * @return int: 0 if successful, -1 otherwise
    */
    
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

    /**
    * @brief This function will determinate the current time in the correct format (example: "2024-08-02T15:04:05").
    * First gets the current time in seconds (current_time) and calculates the milliseconds (milliseconds).
    * Then formats the local time into a string (time_string) and combines the formatted time_string with milliseconds into the final_string.
     
    * @param void
    
    * @return char: A pointer to a dynamically allocated string containing the formatted timestamp with milliseconds - final_string
    */
    
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
    /** 
    * @brief Check if the file already exists. (0 in case of file exist and 1 in case if file doesn't exists)
 
    * @param filename (char*): The csv file name
   
    * @return: int
    */

    return access(filename, F_OK) == 0;
}
 
 
// Build the data string
char * buildData (struct ThermalPair* TP_block,const char *timestamp, int state){
    /** 
    * @brief This function will build the data to put in csv. In this case without error. 
    * Like this -6.000000, 3.000000, 5.000000, -2.000000, Off, Off, Off, Off, 2024-08-02T10:35:25.517, Eclipse, null
 
    * @param TP_block (ThermalPair*): Have the heater (value 0 for Off and 1 for on) and thermistor (temperature) 
    informations to build the string
    * @param timestamp (char *): Have the time information
    * @param state (int): Have the state information can be Normal (with value 1), Eclipse (with value 2), Sun Exposure (with value 3)
   
    * @return: char *
    */

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
    /** 
    * @brief This function appends data to a file (CSV). If the file doesn't exist, it creates
    * the file and writes a header before appending the data.
 
    * @param filename (char*): The csv file name
    * @param header (char *): The header of the csv file. Like this: "THERM-01, THERM-02, THERM-03, 
    * THERM-04, HTR-1, HTR-2, HTR-3, HTR-4, TIMESTAMP, ENVIRONMENT, ERROR"
    * @param data (char *): Data to save in CSV file
    
    * @return: void
    */

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
     /** 
    * @brief This function appends data to a file (CSV) with problems "null, null, null, null, null, 
    * null, null, null, 2024-08-02T10:35:27.121, null, Sensor Error". If the file doesn't exist, it 
    * creates the file and writes a header before appending the data.
 
    * @param filename (char*): The csv file name
    * @param header (char *): The header of the csv file. Like this: "THERM-01, THERM-02, THERM-03, 
    * THERM-04, HTR-1, HTR-2, HTR-3, HTR-4, TIMESTAMP, ENVIRONMENT, ERROR"
    * @param error (char *): The error message
    * @param timestamp (char *): The time information
    
    * @return: void
    */

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


void* read_response_thread(void* args) {/**
    * @brief Read response thread function

    * This thread will read the response from the pipe and update the new_heater_states array.

    * @param args (void*): The arguments for the thread
    * @return void*
    */

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
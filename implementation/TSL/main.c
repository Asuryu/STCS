#include "config.h"

int fd_temp_info_pipe, fd_response_pipe;

int sigint_received = 0;

// Signal handler for SIGINT
void sigint_handler() {
    sigint_received = 1;
}

int main() {


    int16_t clock = 0;

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

    while(!sigint_received){
        
    }


    // char message[100];
    // sprintf(message, "TSL: %d", clock);
    // printf("TSL sending: %s\n", message);
    // write(fd_temp_info_pipe, message, strlen(message)+1);
    
    close(fd_temp_info_pipe);    
    close(fd_response_pipe);

    unlink(TEMP_INFO_PIPE);
	unlink(RESPONSE_PIPE);

    return 0;
}   
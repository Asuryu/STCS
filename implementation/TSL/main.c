#include "config.h"

int fd_temp_info_pipe, fd_response_pipe;


// void sigint_handler() {
//     sigint_received = 1;
// }

int main() {

    // struct sigaction sigint_action;
    // sigint_action.sa_handler = sigint_handler;
    // sigemptyset(&sigint_action.sa_mask);
    // sigint_action.sa_flags = 0;
    // if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
    //     perror("sigaction");
    //     exit(EXIT_FAILURE);
    // }


    int fd;
    int16_t clock = 0;

    if((mkfifo(TEMP_INFO_PIPE,O_CREAT|O_EXCL|0600)<0) && (errno != EEXIST)){
		erro_handler("ERROR CREATING TEMP INFO NAMED PIPE"); 
		exit(0);
	}

    if((mkfifo(RESPONSE_PIPE,O_CREAT|O_EXCL|0600)<0) && (errno != EEXIST)){
		erro_handler("ERROR CREATING RESPONSE NAMED PIPE"); 
		exit(0);
    }

	// Open named pipes
  	if ((fd_temp_info_pipe = open(TEMP_INFO_PIPE,O_WRONLY))<0){
		erro_handler("ERROR OPENING SENSOR NAMED PIPE"); 
		exit(0);
  	}
  	if ((fd_response_pipe = open(RESPONSE_PIPE,O_RDONLY))<0){
		erro_handler("ERROR OPENING CONSOLE NAMED PIPE"); 
		exit(0);
  	}  


    // while(!sigint_received){
    //     sleep(0.2);
    //     clock++;
    //     printf("%d",clock);
    // }

    close(fd);

    return 0;
}
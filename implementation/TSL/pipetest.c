#include "config.h"

int fd_temp_info_pipe, fd_response_pipe;


// void sigint_handler() {
//     sigint_received = 1;
// }

int main() {

    if ((fd_temp_info_pipe = open(TEMP_INFO_PIPE, O_RDWR)) < 0) {
        perror("ERROR OPENING SENSOR NAMED PIPE");
        exit(0);
    }
    printf("TSL opened temp_info_pipe\n");

    if ((fd_response_pipe = open(RESPONSE_PIPE, O_RDWR)) < 0) {
        perror("ERROR OPENING CONSOLE NAMED PIPE");
        exit(0);
    }
    printf("TSL opened response_pipe\n");



    fd_set read_set;

    while (1)
    {
        FD_ZERO(&read_set);
        FD_SET(fd_temp_info_pipe,&read_set);

        if (select(fd_temp_info_pipe+1,&read_set,NULL,NULL,NULL)>0){
            if(FD_ISSET(fd_temp_info_pipe,&read_set)){
                char message[100];
                read(fd_temp_info_pipe, message, sizeof(message));
                printf("TSL received: %s\n", message);

                // write random combination following the pattern {STATE};{STATE};{STATE};{STATE}
                char response[100];
                // random binary number
                sprintf(response, "%d;%d;%d;%d", rand() % 2, rand() % 2, rand() % 2, rand() % 2);
                write(fd_response_pipe, response, strlen(response) + 1);
                printf("TSL sent: %s\n", response);

            }
        }   


    }

    close(fd_temp_info_pipe);    
    close(fd_response_pipe);

    unlink(TEMP_INFO_PIPE);
	unlink(RESPONSE_PIPE);

    return 0;
}   
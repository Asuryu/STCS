#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

    int fd;
    int16_t clock = 0;
    char * myfifo = "/tmp/pipe";

    mkfifo(myfifo,0666);

    fd = open(myfifo, O_CREAT);


    while(1){
        usleep(200 * 1000);
        clock++;
        printf("%d",clock);
    }

    close(fd);

    return 0;
}
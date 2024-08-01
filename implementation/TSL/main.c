#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum {false, true} bool;

struct ThermalPair {
    bool heater;
    double thermistor;
};

struct HeaterGroup{
    double thermistor1;
    double thermistor2;
    double thermistor3;
    double thermistor4;
};


int setHeaterState(struct ThermalPair TP_block);
int modify_temperatures(struct ThermalPair TP_block);
int write_csv(struct ThermalPair TP_block);
int verify_periods(int16_t clock);


int main() {

    int fd;
    int16_t clock = 0;
    char * myfifo = "/tmp/pipe";

    mkfifo(myfifo,0666);

    fd = open(myfifo, O_CREAT);

    while(1){
        usleep(200 * 1000);
        // check the last 8 bits from clock
        // something to get the data from pive
        // setHeaterState()
        // modify_temperatures()
        // write_csv()
        clock++;
        printf("%d",clock);
    }

    close(fd);

    return 0;
}
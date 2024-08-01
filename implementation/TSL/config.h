#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <sys/msg.h>
#include <sys/select.h>

#define TEMP_INFO_PIPE "temp_info_pipe"
#define RESPONSE_PIPE "response_pipe"

typedef enum {false, true} bool;


typedef struct ThermalPair {
    bool heater;
    double thermistor;
} ThermalPair;

typedef struct TSL_data {
    int16_t clock;
    int period;     //period being analyzed
} TSL_data;

typedef struct {
    int fd;
    int *new_heater_states;
} ThreadArgs;

void TSL_init(struct TSL_data *tsl);
void verify_periods(struct TSL_data *tsl);
int setHeaterState(struct ThermalPair TP_block);
int modify_temperatures(struct ThermalPair* TP_block, int state);
int write_csv(struct ThermalPair TP_block);

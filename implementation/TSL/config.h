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

// Define response informations for pipe
#define TEMP_INFO_PIPE "temp_info_pipe"
#define RESPONSE_PIPE "response_pipe"

// Define states and Bool Type
#define NORMAL 1
#define ECLIPSE 2
#define SUN_EXPOSURE 3

typedef enum {false, true} bool;

// Define Structcs
struct ThermalPair {
    bool heater;
    float thermistor;
};

struct TSL_data{
    uint16_t clock;
    int period;     //period being analyzed
};


// Inicialize all the functions
void TSL_init(struct TSL_data *tsl);
void verify_periods(struct TSL_data *tsl);
void setHeaterState(struct ThermalPair* TP_block, char *heater_state);
int modify_temperatures(struct ThermalPair* TP_block, int state);
void write_csv_correct(struct ThermalPair* TP_block, char *timestamp, int state);
void write_csv_errors(char *error);
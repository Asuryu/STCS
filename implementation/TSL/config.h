#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/select.h>


// Define response informations for pipe
#define TEMP_INFO_PIPE "temp_info_pipe"
#define RESPONSE_PIPE "response_pipe"

// Define states and Bool Type
#define NORMAL 1
#define ECLIPSE 2
#define SUN_EXPOSURE 3

typedef enum {false, true} bool;

typedef struct ThermalPair {
    int heater;
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

struct sigaction sigint_action;
struct sigaction sigtstp_action;

// Inicialize all the functions
void TSL_init(struct TSL_data *tsl);
void verify_periods(struct TSL_data *tsl);

void setHeaterState(struct ThermalPair* TP_block, int *heater_state);
int modify_temperatures(struct ThermalPair *TP_block, int state);
const char* get_timestamp();
char * buildData(struct ThermalPair* TP_block, const char *timestamp, int state);
void writeToCSVCorrect (const char *filename, const char *header, const char *data);
void writeToCSVError(const char *filename, const char *header, char *error,const char *timestamp);

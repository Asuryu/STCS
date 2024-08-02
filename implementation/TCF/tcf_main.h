#include <stdio.h>
#include <stdlib.h>
#include <math.h> //4 NaN
#include <time.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <ctype.h>

#define N_HEATERS 4
#define HISTORY_SIZE 2

//{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE}
#define TEMPS_SINTAX "%d;%f-%d;%f-%d;%f-%d;%f-%d"

#define TEMP_INFO_PIPE "/tmp/temp_info_pipe"
#define RESPONSE_PIPE "/tmp/response_pipe"

typedef struct{
    int fd_temp_info_pipe, fd_response_pipe;
} PipeData;

void setTemps(const float newTemps[N_HEATERS]);
float *getTempsAt(int index);
float getThermisterTempsAt(int hist_index, int therm_index);
int enableTCF(pthread_t *pidThread);
void disableTCF(pthread_t *pidThread);
void setSetPoint(float setPoint);
void *control_loop();


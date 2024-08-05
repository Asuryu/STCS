#ifndef TCF_MAIN_H
#define TCF_MAIN_H

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
#include <string.h>

#include "../TCF_TSL/config.h"

#define N_HEATERS 4
#define HISTORY_SIZE 2

//{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE}
#define TEMPS_SINTAX "%d;%f-%d;%f-%d;%f-%d;%f-%d"


typedef struct{
    int fd_temp_info_pipe, fd_response_pipe;
} PipeData;

void setTemps(const float newTemps[N_HEATERS]);
float *getTempsAt(int index);
float getThermisterTempsAt(int hist_index, int therm_index);
int enableTCF(pthread_t *pidThread, PipeData* pd);
void disableTCF(pthread_t *pidThread, PipeData *pd);
void setSetPoint(float newSetPoint, int index);
void setSetPoints(float newSetPoints[N_HEATERS]);
void setAllSetPoints(float newSetPoint);
int openPipes(PipeData *pd);
void closePipes(PipeData *pd);
int createPipes();
int writeToPipe(int fd, char* buf);
void * control_loop(void * pdata) ;
void setFrequency(int newFreq);
void ui();

#endif // TCF_MAIN_H
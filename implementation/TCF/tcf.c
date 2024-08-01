#include <stdio.h>
#include <math.h> //4 NaN

#include <pthread.h>
#include<unistd.h>

#define N_HEATERS 4
#define HISTORY_SIZE 2

//{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE}

#define TEMPS_SINTAX "%f-%d;%f-%d;%f-%d;%f-%d"

float temps[HISTORY_SIZE][N_HEATERS];
unsigned int enabled;


void *controlTemp(void* pdata){
    while (enabled)
    {
        printf(".");
        sleep(1);
    }
    return 0;
    
}

int enableTCF(pthread_t *pidThread) {
    return pthread_create(pidThread,NULL, &controlTemp, NULL) != 0 ? -1 : 0;
}



int main(int argc, char **argv) {
    //memset(&temps, nan, sizeof(temps)); //init array with NaNs so later we can check if it has input temps or not
    enabled = 1;
    pthread_t pidThread;
    
    if(enableTCF(&pidThread) == -1) return -1; 

    printf("\nPress enter to end ");
    getchar();
    enabled = 0;
    

    pthread_join(pidThread,NULL);
    return 0;
}
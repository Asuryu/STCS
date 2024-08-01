#include <stdio.h>
#include <math.h> //4 NaN

#include <pthread.h>
#include<unistd.h>

#include <ctype.h>

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

void setTemps(const float newTemps[N_HEATERS]){
    unsigned i =0, h=0;

    while(i<HISTORY_SIZE)
        if(isnan(temps[i++][0])) //encontrou um vazio
            goto emptyAtI;

    //não há "untapped"
    //1. a fila vai para trás
    i--;
    while(i > 0){
        h = 0;
        while(h < N_HEATERS){
            temps[i-1][h] = temps[i][h];
            h++;
        }
        i--;
    }
   
   //adiciona o mais recente na ultima pos da fila
    h = 0;
    while(h < N_HEATERS){
        temps[HISTORY_SIZE-1][h] = newTemps[h];
        h++;
    }
    return;

//adiciona o mais recente no primeiro lugar disponivel
emptyAtI: 
    h = 0;
    while(h < N_HEATERS){
        temps[i-1][h] = newTemps[h];
        h++;
    }
}

//try start thread
int enableTCF(pthread_t *pidThread) {
    enabled = 1;
    return pthread_create(pidThread,NULL, &controlTemp, NULL) != 0 ? -1 : 0;
}



int main(int argc, char **argv) {
    setbuf(stdout, NULL); 
   
    pthread_t pidThread;
    unsigned i=0;
    
    //init array with NaNs so later we can check if it has input temps or not
    while(i<HISTORY_SIZE)
        temps[i++][0] = NAN;

    
    if(enableTCF(&pidThread) == -1) return -1; 
    printf("Press enter to end ");
    getchar();
    enabled = 0;
    
    pthread_join(pidThread,NULL);
    return 0;
}
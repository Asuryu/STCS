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
<<<<<<< HEAD

//test function to be deleted
=======
float set_point;

>>>>>>> eb30157020f26c8bba082162a8a9ecc650bef77e
void *controlTemp(void* pdata){
    while (enabled)
    {
        printf(".");
        sleep(1);
    }
    return 0;
    
}

/* Set Temperatures Function
 * Inserts new temperature array in the last position of the LIFO history array, or the first empty one if it exists.
 * 
 * Input/Params: 
 * imutable float array of N_HEATERS size containing the values to insert
 * 
 * Output/Return: none
 */
void setTemps(const float newTemps[N_HEATERS]){
    unsigned i =0, h=0;

    while(i<HISTORY_SIZE)
        if(isnan(temps[i++][0])) //found empty spot on the history
            goto emptyAtI;

    // history is full:
    // apply LIFO: pushes values back by 1
    i--;
    while(i > 0){
        h = 0;
        while(h < N_HEATERS){
            temps[i-1][h] = temps[i][h];
            h++;
        }
        i--;
    }
   
   //adds the new temperatures to the last position of the list
    h = 0;
    while(h < N_HEATERS){
        temps[HISTORY_SIZE-1][h] = newTemps[h];
        h++;
    }
    return;

//adds the new temperatures to the first empty position of the list
emptyAtI: 
    h = 0;
    while(h < N_HEATERS){
        temps[i-1][h] = newTemps[h];
        h++;
    }
}

/* Get Temperatures At (Index) Function
 * Returns the temperatures at a given position of the history array - if there are any.
 * 
 * Input/Params: 
 * integer containing the position wanted
 * 
 * Output/Return: 
 *      NULL if the index is out of bounds or there is no data
 *      imutable float array containing the temperatures at the given index
 */
const float *getTempsAt(int index){
    return index < 0 || index >= HISTORY_SIZE || isnan(temps[index][0])  ?  NULL : temps[index];
}


/* Get Temperatures At (Index) Function
 * Returns the temperature of a specific thermister at a given position of the history array - if there is any.
 * 
 * Input/Params: 
 * integer containing the position of the history 
 * integer containing the thermister position/id
 * 
 * Output/Return: 
 *      NaN if any index is out of bounds or there is no data
 *      float containing the temperature of the given thermister at the given history index
 */
const float getThermisterTempsAt(int hist_index, int therm_index){
    return hist_index < 0 || therm_index < 0 || hist_index >= HISTORY_SIZE || therm_index >= N_HEATERS ? NAN : temps[hist_index][therm_index];
}

//try start thread (w/ enable set to 1)
int enableTCF(pthread_t *pidThread) {
    enabled = 1;
    return pthread_create(pidThread,NULL, &controlTemp, NULL) != 0 ? -1 : 0;
}

void disableTCF(pthread_t *pidThread) {
    enabled = 0;
    pthread_join(*pidThread,NULL);
}

void setSetPoint(float setPoint) {
   set_point = setPoint >= -20 && setPoint <= 20 ? setPoint : set_point;
}
int main(int argc, char **argv) {
    setbuf(stdout, NULL); 
   
    pthread_t pidThread;
    unsigned i=0;
    
    //init array with NaNs so later we can check if it has input temps or not
    while(i<HISTORY_SIZE)
        temps[i++][0] = NAN;
    
    if(enableTCF(&pidThread) == -1) return -1; 

    //for testing only!
    printf("Press enter to end ");
    getchar();

    disableTCF(&pidThread);

    return 0;
}
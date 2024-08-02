#include "tcf.h"

float temps[HISTORY_SIZE][N_HEATERS];
unsigned int enabled;

float set_point;

// test function to be deleted
void *controlTemp(void *pdata)
{
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
void setTemps(const float newTemps[N_HEATERS])
{
    unsigned i = 0, h = 0;

    while (i < HISTORY_SIZE)
        if (isnan(temps[i++][0])) // found empty spot on the history
            goto emptyAtI;

    // history is full:
    // apply LIFO: pushes values back by 1
    i--;
    while (i > 0)
    {
        h = 0;
        while (h < N_HEATERS)
        {
            temps[i - 1][h] = temps[i][h];
            h++;
        }
        i--;
    }

    // adds the new temperatures to the last position of the list
    h = 0;
    while (h < N_HEATERS)
    {
        temps[HISTORY_SIZE - 1][h] = newTemps[h];
        h++;
    }
    return;

// adds the new temperatures to the first empty position of the list
emptyAtI:
    h = 0;
    while (h < N_HEATERS)
    {
        temps[i - 1][h] = newTemps[h];
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
const float *getTempsAt(int index)
{
    return index < 0 || index >= HISTORY_SIZE || isnan(temps[index][0]) ? NULL : temps[index];
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
const float getThermisterTempsAt(int hist_index, int therm_index)
{
    return hist_index < 0 || therm_index < 0 || hist_index >= HISTORY_SIZE || therm_index >= N_HEATERS ? NAN : temps[hist_index][therm_index];
}

// try start thread (w/ enable set to 1)
int enableTCF(pthread_t *pidThread)
{
    enabled = 1;
    return pthread_create(pidThread, NULL, &controlTemp, NULL) != 0 ? -1 : 0;
}

void disableTCF(pthread_t *pidThread)
{
    enabled = 0;
    pthread_join(*pidThread, NULL);
}

void setSetPoint(float setPoint)
{
    set_point = setPoint >= -20 && setPoint <= 20 ? setPoint : set_point;
}

void control_loop(int freq)
{
    int fd_temp_info_pipe, fd_response_pipe;
    if ((mkfifo(TEMP_INFO_PIPE, O_CREAT | O_EXCL | 0600) < 0) && (errno != EEXIST))
    {
        perror("ERROR CREATING TEMP INFO NAMED PIPE");
        exit(0);
    }

    if ((mkfifo(RESPONSE_PIPE, O_CREAT | O_EXCL | 0600) < 0) && (errno != EEXIST))
    {
        perror("ERROR CREATING RESPONSE NAMED PIPE");
        exit(0);
    }

    if ((fd_temp_info_pipe = open(TEMP_INFO_PIPE, O_RDWR)) < 0)
    {
        perror("ERROR OPENING SENSOR NAMED PIPE");
        exit(0);
    }

    if ((fd_response_pipe = open(RESPONSE_PIPE, O_RDWR)) < 0)
    {
        print("ERROR OPENING CONSOLE NAMED PIPE");
        exit(0);
    }

    char buffer[1024];
    ssize_t bytes_read;
    float temp_values[N_HEATERS];
    int response[4];

    while (enabled)
    {
        //Read Temperatures
        bytes_read = read(fd_temp_info_pipe, temps, sizeof(buffer) - 1);
        if (bytes_read > 0){
            buffer[bytes_read] = '\0';
            sscanf(buffer, TEMPS_SINTAX, &temp_values[0], &temp_values[1], &temp_values[2], &temp_values[3]);
            setTemps(temp_values);
        }

        //Control Function

        //Write Response
        sprintf(buffer, "%d;%d;%d;%d", response[0], response[1], response[2], response[3]);
        write(fd_response_pipe, buffer, 8);
        sleep(1 / freq);
    }
    
}

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);

    pthread_t pidThread;
    unsigned i = 0;

    // init array with NaNs so later we can check if it has input temps or not
    while (i < HISTORY_SIZE)
        temps[i++][0] = NAN;

    if (enableTCF(&pidThread) == -1)
        return -1;

    // for testing only!
    printf("Press enter to end ");
    getchar();

    disableTCF(&pidThread);

    return 0;
}
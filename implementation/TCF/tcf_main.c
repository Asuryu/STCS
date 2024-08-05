#include "tcf_main.h"
#include "pid.h"

float temps[HISTORY_SIZE][N_HEATERS];
unsigned int enabled;

float set_point[N_HEATERS];
pthread_mutex_t spMutex;
int lastRegIndex;

int frequency;
pthread_mutex_t frequencyMutex;
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
float *getTempsAt(int index)
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
float getThermisterTempsAt(int hist_index, int therm_index)
{
    return hist_index < 0 || therm_index < 0 || hist_index >= HISTORY_SIZE || therm_index >= N_HEATERS ? NAN : temps[hist_index][therm_index];
}

// try start thread (w/ enable set to 1)
int enableTCF(pthread_t *pidThread, PipeData *pd)
{
    enabled = 1;
    return pthread_create(pidThread, NULL, &control_loop, pd) != 0 ? -1 : 0;
}

void disableTCF(pthread_t *pidThread, PipeData *pd)
{
    enabled = 0;
    pthread_join(*pidThread, NULL);

    openPipes(pd);
    char *buffer = malloc(sizeof(char) * 8 + 1);
    sprintf(buffer, "%d;%d;%d;%d", 0, 0, 0, 0);

    if (writeToPipe(pd->fd_response_pipe, buffer) < 0)
        printf("ERROR WRITING TO PIPE.\n");
    closePipes(pd);
}

void setSetPoint(float newSetPoint, int index)
{
    pthread_mutex_lock(&spMutex);

    set_point[index] = newSetPoint >= -20 && newSetPoint <= 20 ? newSetPoint : set_point[index];

    pthread_mutex_unlock(&spMutex);
}

void setSetPoints(float newSetPoints[N_HEATERS])
{
    for (int i = 0; i < N_HEATERS; i++)
        setSetPoint(newSetPoints[i], i);
}

void setAllSetPoints(float newSetPoint)
{
    for (int i = 0; i < N_HEATERS; i++)
        setSetPoint(newSetPoint, i);
}

int openPipes(PipeData *pd)
{
    if ((pd->fd_temp_info_pipe = open(TEMP_INFO_PIPE, O_RDWR)) < 0 || (pd->fd_response_pipe = open(RESPONSE_PIPE, O_RDWR)) < 0)
    {
        perror("ERROR OPENING NAMED PIPES\n");
        return -1;
    }
    return 0;
}

void closePipes(PipeData *pd)
{
    close(pd->fd_response_pipe);
    close(pd->fd_temp_info_pipe);
}

int createPipes()
{
    if (((mkfifo(TEMP_INFO_PIPE, O_CREAT | O_EXCL | 0600) < 0) && (errno != EEXIST)) || ((mkfifo(RESPONSE_PIPE, O_CREAT | O_EXCL | 0600) < 0) && (errno != EEXIST)))
    {
        perror("ERROR CREATING NAMED PIPES\n");
        return -1;
    }
    return 0;
}

int writeToPipe(int fd, char *buf)
{
    return write(fd, buf, sizeof(buf));
}

void *control_loop(void *pdata)
{
    printf("Control Loop\n");
    PipeData *pd = (PipeData *)pdata;

    if (openPipes(pd) == -1)
        return NULL;

    char buffer[1024];
    ssize_t bytes_read;
    float temp_values[N_HEATERS];
    int state_values[N_HEATERS];
    int response[4];
    float integral[] = {0, 0, 0, 0};
    float derivative[] = {0, 0, 0, 0};
    float initial_temp[] = {0, 0, 0, 0};
    pthread_mutex_lock(&frequencyMutex);
    float step = 1 / frequency;
    pthread_mutex_unlock(&frequencyMutex);
    int index;

    while (enabled)
    {
        // Read Temperatures
        bytes_read = read(pd->fd_temp_info_pipe, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';
            sscanf(buffer, TEMPS_SINTAX, &index, &temp_values[0], &state_values[0], &temp_values[1], &state_values[1], &temp_values[2], &state_values[2], &temp_values[3], &state_values[3]);
            if (index == lastRegIndex)
                continue;

            lastRegIndex = index;
            setTemps(temp_values);
        }

        // Control Function
        pthread_mutex_lock(&spMutex);
        pid(temp_values, integral, initial_temp, derivative, step, set_point, response);
        pthread_mutex_unlock(&spMutex);

        // Write Response
        sprintf(buffer, "%d;%d;%d;%d", response[0], response[1], response[2], response[3]);
        // printf("RESPONSE: %s\n", buffer);
        if (writeToPipe(pd->fd_response_pipe, buffer) < 0)
            printf("ERROR WRITING TO PIPE.\n");
        sleep(step);
    }

    closePipes(pd);
    return 0;
}

void setFrequency(int newFreq)
{
    pthread_mutex_lock(&frequencyMutex);
    frequency = newFreq > 0 && newFreq < 6 ? newFreq : frequency;
    pthread_mutex_unlock(&frequencyMutex);
}

void userSetSetpoints()
{
    int choice = -1, idx;
    float sp, sps[4];
    printf("1. Change all setpoints\n2. Change all setpoints to one value \n3. Change one setpoint\n");
    switch (choice)
    {
    case 1:
        printf("Setpoints sintax: {sp},{sp},{sp},{sp}\n");
        scanf("%f,%f,%f,%f", &sps[0], &sps[1], &sps[2], &sps[3]);

        setSetPoints(sps);
        break;
    case 2:
        printf("Desired setpoint:\n");
        scanf("%f", &sp);

        setAllSetPoints(sp);
        break;
    case 3:
        printf("Desired index [0-%d]:\n", N_HEATERS);
        scanf("%d", &idx);

        printf("Desired setpoint:\n");
        scanf("%f", &sp);

        setSetPoint(sp, idx);
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }
}

void ui(pthread_t *pidThread, PipeData *pd)
{
    int choice = -1, aux;
    while (1)
    {
        printf("Thermal Control Function\n");
        enabled == 1 ? printf("1. Disable\n") : printf("1. Enable\n");
        printf("2. Change frequency\n3. Change setpoint\n4. Exit\n");

        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            if (enabled == 0)
            {
                enableTCF(pidThread, pd);
            }
            else
            {
                disableTCF(pidThread, pd);
            }
            break;
        case 2:
            printf("Desired frequency: [1-5]\n");
            scanf("%d", &aux);
            setFrequency(aux);
            break;
        case 3:
            userSetSetpoints();
            break;
        case 4:
            return;
        default:
            printf("Invalid choice.\n");
            break;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <freq>\n", argv[0]);
        exit(-1);
    }

    frequency = atoi(argv[1]);
    if (frequency < 1 || frequency > 5)
    {
        printf("Invalid frequency. 1 <= freq <= 5 \n");
        exit(-1);
    }

    if (createPipes() == -1)
        return -1;
    if (pthread_mutex_init(&frequencyMutex, NULL) != 0)
    {
        printf("Mutex initialization has failed\n");
        return -1;
    }
    if (pthread_mutex_init(&spMutex, NULL) != 0)
    {
        printf("Mutex initialization has failed\n");
        return -1;
    }

    setAllSetPoints(0); // default

    setbuf(stdout, NULL);

    lastRegIndex = -1;

    pthread_t pidThread;
    unsigned i = 0;

    // init array with NaNs so later we can check if it has input temps or not
    while (i < HISTORY_SIZE)
        temps[i++][0] = NAN;

    PipeData pd;

    ui(&pidThread, &pd);

    disableTCF(&pidThread, &pd);
    pthread_mutex_destroy(&frequencyMutex);
    pthread_mutex_destroy(&spMutex);

    return 0;
}
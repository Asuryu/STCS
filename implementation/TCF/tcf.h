#include <stdio.h>
#include <math.h> //4 NaN

#include <pthread.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <ctype.h>

#define N_HEATERS 4
#define HISTORY_SIZE 2

//{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE};{TEMP}-{STATE}
#define TEMPS_SINTAX "%f-%d;%f-%d;%f-%d;%f-%d"

#define TEMP_INFO_PIPE "temp_info_pipe"
#define RESPONSE_PIPE "response_pipe"
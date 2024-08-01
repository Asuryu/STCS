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

#define TEMP_INFO_PIPE "temp_info_pipe"
#define RESPONSE_PIPE "response_pipe"


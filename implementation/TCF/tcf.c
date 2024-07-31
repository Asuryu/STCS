#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <pthread.h>
#include<unistd.h>

#define N_HEATERS 4

typedef struct{
    pthread_mutex_t *pmutex; //to share w/ TSL?
    int id; //just for show
}TData;

void *start_ (void* tdata){
    //just for show
    TData* pdata = (TData*) tdata;
    printf("start [%d]\n",pdata->id);
    sleep(pdata->id +1 );
    printf("end [%d]\n",pdata->id);
    // ------------

    //TODO: get data from TSL & process

    //cleanup
    pthread_mutex_destroy(pdata->pmutex);
    free(pdata->pmutex);
    free(pdata); //just mutex so ok
    return 0;
}
int enableTCF(pthread_t *threads) {
    TData *data[N_HEATERS];

    unsigned i;
    for ( i = 0; i < N_HEATERS; i++) {
        data[i] = (TData*)malloc(sizeof(TData)); //init data: mutex 

        if (data[i] == NULL) 
            goto failedDataAlloc;
        

        //init mutex
        data[i]->pmutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        if (data[i]->pmutex == NULL)
            goto failedMutexAlloc;

        if (pthread_mutex_init(data[i]->pmutex, NULL) != 0) 
            goto failedMutexInit;
        

        data[i]->id = i; //just for show

        //create thread
        if (pthread_create(&threads[i], NULL, &start_, data[i]) != 0) 
            goto failedThreadCreate;
        
    }

    return 0;

//errors & cleanup
failedThreadCreate:
    printf("ERROR! Failed creating thread %d\n", i + 1);
    goto cleanup;

failedMutexInit:
    printf("ERROR! Failed creating mutex %d\n", i + 1);
    goto cleanup;

failedMutexAlloc:
    free(data[i]);
    printf("ERROR! Failed allocating memory for mutex %d\n", i + 1);
    goto cleanup;

failedDataAlloc:
    printf("ERROR! Failed allocating memory for data %d\n", i + 1);
    goto cleanup;

cleanup:
    for (unsigned j = 0; j < i; j++) {
        pthread_mutex_destroy(data[j]->pmutex);
        free(data[j]->pmutex);
        free(data[j]);
    }
    return -1;
}

int main(int argc, char **argv) {
    //alloc mem for threads
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * N_HEATERS);
    if (threads == NULL) {
        fprintf(stderr, "Failed to allocate memory for threads\n");
        return -1;
    }

    //to show enable func:
    if (enableTCF(threads) != 0) {
        free(threads);
        return -1;
    }
    //TODO: disable .. sigint ig?

    //wait for threads to finish 4now
    for (unsigned i = 0; i < N_HEATERS; i++) 
        pthread_join(threads[i], NULL);
    

    //cleanup
    free(threads);
    return 0;
}
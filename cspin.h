#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct cspin {
    size_t el_size;
    char* elements;
    int delay;
    bool busy;
    size_t cur;
    pthread_t thread;
    pthread_mutex_t mutex;
    const char* msg;
} cspin;

int cspin_start(cspin** t, const char* msg);
void cspin_cancel(cspin** t);
void cspin_restart(cspin** t, const char* msg);
void cspin_free(cspin** t);

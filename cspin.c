#include "cspin.h"

inline static cspin* cspin_make() {
    cspin* t = malloc(sizeof(cspin));
    if(!t) return NULL;

    t->el_size = 4;

    t->elements = malloc(4*sizeof(char));
    if(!t->elements) {free(t); return NULL;}
    t->elements[0] = '-', t->elements[1] = '/',
    t->elements[2] = '|', t->elements[3] = '\\';

    t->delay = 100000;
    t->busy = true;
    t->cur = 0;
    t->msg = NULL;
    pthread_mutex_init(&t->mutex, NULL);

    return t;
}
inline static void _cspin_free(cspin** t) {
    #define t (*t)
    free(t->elements);
    free(t);
    t = NULL;
    #undef t
}

inline static void* cspin_work(void* arg) {
    cspin* t = (cspin*)arg;
    printf("%s ", t->msg);
    while(t->busy) {
        putchar(t->elements[(t->cur++)%t->el_size]);
        fflush(stdout);
        usleep(t->delay);
        putchar('\b');
        fflush(stdout);
    }
    pthread_exit(0);
}

int cspin_start(cspin** t, const char* msg) {
    #define t (*t)
    t = cspin_make();
    if(!t) return -1;
    t->msg = msg;
    if(pthread_create(&t->thread, NULL, cspin_work, t)) return -1;
    return 0;
    #undef t
}

void cspin_cancel(cspin** t) {
    #define t (*t)
    pthread_cancel(t->thread);
    pthread_join(t->thread, NULL);
    fprintf(stdout, "\b \r");
    #undef t
}

void cspin_restart(cspin** t, const char* msg) {
    #define t (*t)
    if(msg) t->msg = msg;
    pthread_create(&t->thread, NULL, cspin_work, t);
    #undef t
}

void cspin_free(cspin** t) {
    cspin_cancel(t);
    _cspin_free(t);
}

//
//  main.c
//  Assignment_2
//
//  Created by Kishan on 2/22/18.
//  Copyright © 2018 Kishan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct thread_data {
    int id;
};

typedef void (Start)(void *, struct thread_data *, pthread_t *);

typedef void (Join)(pthread_t);

typedef struct _Thread {
    pthread_t id;
    Start *start;    // typedef Start which is a function pointer
    Join *join;      // typede Join which is a function pointer
} Thread;

void start(void *runner, struct thread_data *params_id, pthread_t *thread_id) {

    int ret = pthread_create(thread_id, NULL, runner, (void *) params_id);
    if (ret) {
    
        printf("Error! Unable to create thread %d!\n", ret);
    }
}

void join(pthread_t thread_id) {

    void *status;
    pthread_join(thread_id,(void **)&status);
}

void *runner(void * params) {

    for (int i=0; i<10; i++) {
    
        struct thread_data *data=(struct thread_data *) params;
        printf("Thread %d - %d\n", data->id, i);
        sleep((int)(rand()%2));
    }
    return 0;
}

int main(int argc, char *argv[])
{
    srand(time(0));
    
    int i, num = 4;
    struct thread_data params[num];
    for (i=0; i<num; i++) params[i].id = i;  // thread identifier
    
    Thread thread[num];
    for (int i=0; i<num; i++) {
        thread[i].start = start;
        thread[i].join = join;
    }
    
    for (i=0; i<num; i++) {
        thread[i].start(runner, &params[i], &thread[i].id);
    }
    
    for (i=0; i<num; i++) {
        thread[i].join(thread[i].id);
    }
    
    pthread_exit(NULL);
    return 0;
}

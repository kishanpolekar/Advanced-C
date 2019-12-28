//
//  main.c
//  Assignment3
//
//  Created by Kishan on 4/2/18.
//  Copyright © 2018 Kishan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 32

sem_t full, empty;      // The semaphores
pthread_mutex_t mutex;  // The mutex lock

int queue [BUFFER_SIZE];
int x, s, e;

struct thread_data {
    int id;
};

typedef void (Start)(void *, struct thread_data *, pthread_t *);

typedef void (Join)(pthread_t);

typedef struct _Thread {
    pthread_t id;
    Start *start;
    Join *join;
} Thread;

void *producer (void * params) {
    
    while (1) {
        
        int val;
        struct thread_data *data=(struct thread_data *) params;
        val = (int)(rand()%10);
	if (x>31) {

		printf("BUFFER is FULL...\n");
	}
	sem_wait(&full);
       	pthread_mutex_lock(&mutex);
        queue[(e++)%32] = val;
       	printf("Thread %d - Produce %d (%d)\n", data->id, val, ++x);
       	pthread_mutex_unlock(&mutex);
        sem_post(&empty);
	sleep((int)(rand()%2));
    }
    
    return 0;
}

void *consumer (void * params) {
    
    while (1) {
        
        int val;
        struct thread_data *data=(struct thread_data *) params;
        if (x<1 || s==e && s!=0) {
        
	    printf("BUFFER is EMPTY...\n");
	}
	sem_wait(&empty);
	pthread_mutex_lock(&mutex); 
        val = queue[(s++)%32];
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        printf("Thread %d - Consume %d (%d)\n", data->id, val, --x);
	sleep((int)(rand()%2));
    }
    
    return 0;
}

void start(void *func, struct thread_data *params_id, pthread_t *thread_id) {
    
    int ret;
    struct thread_data *data=(struct thread_data *) params_id;
    if (data->id==1)
        ret = pthread_create(thread_id, NULL, consumer, (void *) params_id);
    else
        ret = pthread_create(thread_id, NULL, producer, (void *) params_id);
    if (ret) {
        
        printf("Error! Not able to create thread %d!\n", ret);
    }
}

void join(pthread_t thread_id) {
    
    void *status;
    pthread_join(thread_id,(void **)&status);
}

void Init() {
    
    sem_init(&full, 1, BUFFER_SIZE);
    sem_init(&empty, 0, 0);
    pthread_mutex_init(&mutex, NULL);
}

int main(int argc, char *argv[])
{
    srand(time(0));
    
    int num = 3;
    struct thread_data params[num];
    for (int i=0; i<num; i++) params[i].id = i;
    
    Init();
    
    Thread thread[num];
    for (int i=0; i<num; i++) {
        thread[i].start = start;
        thread[i].join = join;
    }
    thread[0].start(producer, &params[0], &thread[0].id);
    thread[1].start(consumer, &params[1], &thread[1].id);
    thread[2].start(producer, &params[2], &thread[2].id);
    
    for (int i=0; i<num; i++) {
        thread[i].join(thread[i].id);
    }
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    return 0;
}

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define BUFFER_CAPACITY 8
volatile int buffer[BUFFER_CAPACITY];
volatile int buffer_size = 0;
volatile bool producerFinished = false;
bool usingMutex = false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct timespec interval = {0, 1};

void *producer_thread(void *user) {
    int i = 0;
    while (i < 30) {
        if (buffer_size < 8) {
            if (usingMutex) {
                pthread_mutex_lock(&mutex);
            }
            buffer[buffer_size] = i;
            buffer_size++;
            if (usingMutex) {
                pthread_mutex_unlock(&mutex);
            }
            i++;
        } else {
            nanosleep(&interval, NULL);
        }
    }
    producerFinished = true;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        usingMutex = true;
    }
    pthread_t producer;
    pthread_create(&producer, NULL, producer_thread, NULL);
    //consumer thread
    while (!producerFinished) {
        if (buffer_size > 0) {
            if (usingMutex) {
                pthread_mutex_lock(&mutex);
            }
            buffer_size--;
            int toPrint = buffer[buffer_size];
            if (usingMutex) {
                pthread_mutex_unlock(&mutex);
            }
            printf("%d\n", toPrint);
        }
    }
    pthread_join(producer, NULL);
    return 0;
}

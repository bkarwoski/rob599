#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define BUFFER_CAPACITY 8
int buffer[BUFFER_CAPACITY];
int buffer_size = 0;
bool producerFinished = false;

struct timespec interval = {0, 1};

void *producer_thread(void *user) {
    // loop over all 30 messages, trying to put them on the stack
    // only quit and set 'producer_finished = true' once ALL 30 messages
    // have been sent
    int i = 0;
    while (i < 30) {
        if (buffer_size < 8) {
            buffer[buffer_size] = i;
            buffer_size++;
            i++;
        } else {
            nanosleep(&interval, NULL);
        }
    }
    producerFinished = true;
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t producer;
    pthread_create(&producer, NULL, producer_thread, NULL);
    //consumer thread
    while (!producerFinished) {
        if (buffer_size > 0) {
            printf("%d\n", buffer[buffer_size-1]);
            buffer_size--;
        }
    }
    pthread_join(producer, NULL);
    return 0;
}
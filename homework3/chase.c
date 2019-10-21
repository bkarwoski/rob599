#define _GNU_SOURCE
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "bmp.h"
#include "graphics.h"
#include "image_server.h"
#define M_PI 3.14159265358979323846
#define WIDTH 640
#define HEIGHT 480
#define BLOCK_SIZE 40
#define MAP "XXXXXXXXXXXXXXXX" \
            "X              X" \
            "X  XXXX   XXX  X" \
            "X   XX      X  X" \
            "X       XXX    X" \
            "XXXXXX         X" \
            "X         XXXXXX" \
            "X    XXX       X" \
            "X  XX     XX   X" \
            "X   X    XX    X" \
            "X      XXX     X" \
            "XXXXXXXXXXXXXXXX"
#define MAP_W (WIDTH / BLOCK_SIZE)
#define MAP_H (HEIGHT / BLOCK_SIZE)

typedef struct agent {
    bool is_runner;
    double x;
    double y;
    double theta;
    double vel;
    double ang_vel;
} agent_t;

typedef struct search_node {
    int depth;
    agent_t runner;
    agent_t chaser;
} search_node_t;

typedef struct state {
    int time_step;
    bitmap_t bmp;
    size_t image_size;
    uint8_t *image_data;

    bool runner_caught;

    agent_t runner;
    agent_t chaser;
} state_t;

void drawMap(bitmap_t *bmp) {
    color_bgr_t black = {0, 0, 0};
    color_bgr_t yellow = {0, 255, 255};
    color_bgr_t white = {255, 255, 255};
    for(int i = 0; i < bmp->width * bmp->height; i++){
        bmp->data[i] = white;
        //printf("i = %d\n", i);
    }
    for (int i = 0; i < MAP_W * MAP_H; i++) {
        if (MAP[i] == 'X') {
            vector_xy_t nextBlock = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
            double xPos = BLOCK_SIZE / 2 + (i % MAP_W) * BLOCK_SIZE;
            printf("xPos = %f\n", xPos);
            double yPos = BLOCK_SIZE / 2 + ((i - i % MAP_W) / MAP_W) * BLOCK_SIZE;
            printf("yPos = %f\n", yPos);
            gx_trans(xPos, yPos, &nextBlock);
            gx_fill_poly(bmp, black, &nextBlock);
            vector_delete(&nextBlock);
        }
    }

}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("usage: %s <time steps> <fast=0|1|2> <initial runner index>\n", argv[0]);
        return 1;
    }
    int seconds = 0;
    long nanoseconds = 40 * 1000 * 1000;
    struct timespec interval = { seconds, nanoseconds };
    int fast = atoi(argv[2]);
    state_t state;
    state.time_step = atoi(argv[1]);
    bitmap_t bmp = {0};
    bmp.width = WIDTH;
    bmp.height = HEIGHT;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    image_server_start("8000");
    for (int i = 0; i < state.time_step; i++) {
        if (fast == 0) {
            drawMap(&bmp);
            bmp_serialize(&bmp, serialized_bmp);
            image_server_set_data(bmp_size, serialized_bmp);
            nanosleep(&interval, NULL);
        }

    }
    free(serialized_bmp); 
    free(bmp.data);
    return 0;
}

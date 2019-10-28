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
#include "collision.h"
#define M_PI 3.14159265358979323846
#define WIDTH 640
#define HEIGHT 480
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
#define BLOCK_SIZE 40
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
    }
    for (int i = 0; i < MAP_W * MAP_H; i++) {
        if (MAP[i] == 'X') {
            vector_xy_t nextBlock = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
            double xPos = BLOCK_SIZE / 2 + (i % MAP_W) * BLOCK_SIZE;
            double yPos = BLOCK_SIZE / 2 + ((i - i % MAP_W) / MAP_W) * BLOCK_SIZE;
            gx_trans(xPos, yPos, &nextBlock);
            gx_fill_poly(bmp, black, &nextBlock);
            vector_delete(&nextBlock);
        }
    }
}

void updateGraphics(state_t *state) {
    color_bgr_t red = {0, 0, 255};
    color_bgr_t green = {0, 255, 0};
    drawMap(&state->bmp);
    vector_xy_t runPoints = gx_rob();
    gx_rot(state->runner.theta, &runPoints);
    gx_trans(state->runner.x, state->runner.y, &runPoints);
    gx_fill_poly(&state->bmp, green, &runPoints);
    vector_delete(&runPoints);
    vector_xy_t chasePoints = gx_rob();
    gx_rot(state->chaser.theta, &chasePoints);
    gx_trans(state->chaser.x, state->chaser.y, &chasePoints);
    gx_fill_poly(&state->bmp, red, &chasePoints);
    vector_delete(&chasePoints);
}

int runnerAction(void) {
    int action = 0;
    // int action = rand() % 20;
    // if (action == 2 || action == 3) {
    //     return action;
    // }
    printf("%d ", action);
    return action;
}

int chaserAction(int timestep) {
    int action = 0;
    if (timestep == 0) {
        action = 2;
    } else if (timestep <= 9) {
        action = 1;
    }
    
    printf("%d\n", action);
    return action;
}

void applyAction(agent_t *bot, int action) {
    if (action == 1) {
        bot->vel += 2;
        if (bot->vel > 12) {
            bot->vel = 12;
        }
    }
    if (action == 2) {
        bot->ang_vel += M_PI / 16;
    }
    if (action == 3) {
        bot->ang_vel -= M_PI / 16;
    }
}

bool resolveWallCollisions(agent_t *bot) {
    bool collided = false;
    double bRadius = sqrt(2 * BLOCK_SIZE * BLOCK_SIZE) / 2;
    double rRadius = sqrt(pow((4 / 3 * 20), 2) + 10 * 10) / 2;
    double collision_dist_sq = pow((bRadius + rRadius), 2);
    for (int x = 0; x < MAP_W; x++) {
        for (int y = 0; y < MAP_H; y++) {
            int i = x + MAP_W * y;
            if (MAP[i] == 'X') {
                double xPos = BLOCK_SIZE * (x + 0.5);
                double yPos = BLOCK_SIZE * (y + 0.5);
                double dist_sq = pow(xPos - bot->x, 2) + pow(yPos - bot->y, 2);
                if (dist_sq <= collision_dist_sq) {
                    //printf("approx collision\n");
                    vector_xy_t nextBlock = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
                    gx_trans(xPos, yPos, &nextBlock);
                    vector_xy_t rob = gx_rob();
                    if (collision(&nextBlock, &rob)) {
                        printf("Collision w/ tile %d, %d\n", x, y);
                        double dx = bot->x - xPos;
                        double dy = bot->y - yPos;
                        double dist = sqrt(dx * dx + dy * dy);
                        bot->x += dx / dist_sq * 0.5;
                        bot->y += dy / dist_sq * 0.5;
                        collided = true;
                    }
                    vector_delete(&nextBlock);
                    vector_delete(&rob);
                }
            }
        }
    }
    return collided;
}

bool resolveRobCollisions(state_t *state) {
    return false;
}

void moveBot(agent_t *bot, int action) {
    applyAction(bot, action);
    bot->theta += bot->ang_vel;
    bot->ang_vel *= 0.8;
    bot->x += bot->vel * cos(bot->theta);
    bot->y += bot->vel * -sin(bot->theta);
    if (resolveWallCollisions(bot)) {
        bot->vel *= 0.25;
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
    int runnerIndex = atoi(argv[3]);
    int chaserIndex = 97;
    state_t state;
    state.time_step = atoi(argv[1]);
    state.bmp.width = WIDTH;
    state.bmp.height = HEIGHT;
    state.bmp.data = calloc(state.bmp.width * state.bmp.height, sizeof(color_bgr_t));
    state.image_size = bmp_calculate_size(&state.bmp);
    state.image_data = malloc(state.image_size);
    state.runner.x = BLOCK_SIZE / 2 + (runnerIndex % MAP_W) * BLOCK_SIZE;
    state.runner.y = BLOCK_SIZE / 2 + ((runnerIndex - runnerIndex % MAP_W) / MAP_W) * BLOCK_SIZE;
    state.runner.theta = 0;
    state.runner.ang_vel = 0;
    state.chaser.x = WIDTH / 2;
    state.chaser.y = HEIGHT / 2;
    state.chaser.theta = 0;
    state.chaser.ang_vel = 0;

    image_server_start("8000");
    for (int i = 0; i < state.time_step; i++) {
        if (fast == 0) {
            updateGraphics(&state);
            bmp_serialize(&state.bmp, state.image_data);
            image_server_set_data(state.image_size, state.image_data);
            printf("%d: ", i);
            printf("x: %.2f ", state.chaser.x);
            printf("y: %.2f ", state.chaser.y);
            printf("theta: %.2f ", state.chaser.theta);
            printf("vel: %.2f ", state.chaser.vel);
            printf("ang_vel: %.2f\n", state.chaser.ang_vel);

            nanosleep(&interval, NULL);
            moveBot(&state.runner, runnerAction());
            moveBot(&state.chaser, chaserAction(i));

        }
    }
    free(state.image_data); 
    free(state.bmp.data);
    return 0;
}

#define _GNU_SOURCE
#include <lcm/lcm.h>
#include <termios.h>
#include <time.h>
#include "collision.h"
#include "graphics.h" //needed for collision resolution
#include "settings.h"
#include "lcmtypes/action_t.h"
#include "lcmtypes/agent_t.h"
#include "lcmtypes/reset_t.h"
#include "lcmtypes/settings_t.h"
#include "lcmtypes/world_t.h"
#include "lcm_handle_async.h"

typedef struct state {
    int time_step;
    bool runner_caught;
    agent_t runner;
    agent_t chaser;
    int init_runner_idx;
    int delay_every;
    int max_velocity;
    lcm_t *lcm;
} state_t;

int runnerAction(void) {
    int action = rand() % (19 + 1);
    if (action == 1 || action == 2) {
        return action;
    }
    return 0;
}

void applyAction(agent_t *bot, int action) {
    if (action == 1) {
        bot->vel += 2;
        if (bot->vel > 12) {
            bot->vel = 12;
        }
    }
    if (action == 2) {
        bot->ang_vel += M_PI / 16.0;
    }
    if (action == 3) {
        bot->ang_vel -= M_PI / 16.0;
    }
}

void resolveWallCollisions(agent_t *bot) {
    bool collided = false;
    bool any_collision = true;
    double bRadius = sqrt(2 * BLOCK_SIZE * BLOCK_SIZE) / 2.0;
    double rRadius = sqrt(pow(ROB_LENGTH / 2.0, 2) + pow(ROB_HEIGHT / 2.0, 2));
    double collision_dist_sq = pow((bRadius + rRadius), 2);
    while (any_collision) {
        any_collision = false;
        for (int x = 0; x < MAP_W; x++) {
            for (int y = 0; y < MAP_H; y++) {
                int i = x + (int)MAP_W * y;
                if (MAP[i] == 'X') {
                    double xPos = BLOCK_SIZE * (x + 0.5);
                    double yPos = BLOCK_SIZE * (y + 0.5);
                    double dist_sq = pow(xPos - bot->x, 2) + pow(yPos - bot->y, 2);
                    if (dist_sq <= collision_dist_sq) {
                        vector_xy_t nextBlock = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
                        gx_trans(xPos, yPos, &nextBlock);
                        vector_xy_t rob = gx_rob();
                        gx_rot(bot->theta, &rob);
                        gx_trans(bot->x, bot->y, &rob);
                        bool isCollided = collision(&nextBlock, &rob);
                        if (isCollided) {
                            any_collision = true;
                            //printf("Collision w/ tile %d, %d, ", x, y);
                            double dx = bot->x - xPos;
                            double dy = bot->y - yPos;
                            double dist = sqrt(dx * dx + dy * dy);
                            //printf("movement from %.2f, %.2f ", bot->x, bot->y);
                            bot->x += dx / dist * 0.5;
                            bot->y += dy / dist * 0.5;
                            collided = true;
                            //printf("to %.2f, %.2f ", bot->x, bot->y);
                            //printf("with robot xs: %.2f, %.2f, %.2f ",
                            //    rob.xData[0], rob.xData[1], rob.xData[2]);
                            //printf("ys: %.2f, %.2f, %.2f\n",
                            //    rob.yData[0], rob.yData[1], rob.yData[2]);
                            //isCollided = collision(&nextBlock, &rob);
                        }
                        vector_delete(&nextBlock);
                        vector_delete(&rob);
                    }
                }
            }
        }
    }
    if (collided) {
        bot->vel *= 0.25;
    }
}

bool robCollision(agent_t runner, agent_t chaser) {
    vector_xy_t runnerPoints = gx_rob();
    gx_rot(runner.theta, &runnerPoints);
    gx_trans(runner.x, runner.y, &runnerPoints);
    vector_xy_t chaserPoints = gx_rob();
    gx_rot(chaser.theta, &chaserPoints);
    gx_trans(chaser.x, chaser.y, &chaserPoints);
    bool doesCollide = collision(&runnerPoints, &chaserPoints);
    vector_delete(&chaserPoints);
    vector_delete(&runnerPoints);
    return doesCollide;
}

void moveBot(agent_t *bot) {
    bot->theta += bot->ang_vel;
    bot->ang_vel *= 0.8;
    bot->x += bot->vel * cos(bot->theta);
    bot->y += bot->vel * -sin(bot->theta);
}

double seconds_now(void) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now)) {
        fprintf(stderr, "Retrieving system time failed.\n");
        exit(1);
    }
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

void reset_sim(state_t *s) {
    srand(0);
    s->runner.x = BLOCK_SIZE / 2.0 + (s->init_runner_idx % (int)MAP_W) * BLOCK_SIZE;
    s->runner.y = BLOCK_SIZE / 2.0 + ((s->init_runner_idx - s->init_runner_idx % (int)MAP_W) /
                                      MAP_W) *
                                         BLOCK_SIZE;
    s->runner.theta = 0;
    s->runner.ang_vel = 0;
    s->runner.vel = 0;
    s->runner.is_runner = true;
    s->chaser.x = WIDTH / 2.0;
    s->chaser.y = HEIGHT / 2.0;
    s->chaser.theta = 0;
    s->chaser.ang_vel = 0;
    s->chaser.vel = 0;
    s->time_step = 0;
}

void setup_sim(state_t *s) {
    s->init_runner_idx = 17;
    s->delay_every = 1;
    reset_sim(s);
}

void handle_settings(const lcm_recv_buf_t *rbuf, const char *channel,
                     const settings_t *msg, void *userdata) {
    state_t *s = (state_t *)userdata;
    s->delay_every = msg->delay_every;
}

void handle_reset(const lcm_recv_buf_t *rbuf, const char *channel,
                  const reset_t *msg, void *userdata) {
    state_t *s = (state_t *)userdata;
    s->init_runner_idx = msg->initial_runner_idx;
    reset_sim(s);
}

void handle_action(const lcm_recv_buf_t *rbuf, const char *channel,
                   const action_t *msg, void *userdata) {
    state_t *s = (state_t *)userdata;
    if (msg->vel - s->chaser.vel > 2) {
        s->chaser.vel += 2;
    } else {
        s->chaser.vel = msg->vel;
    }

    if (s->chaser.vel > 12) {
        s->chaser.vel = 12;
    }

    if (msg->ang_vel > M_PI / 16) {
        s->chaser.ang_vel = M_PI / 16;
    } else if (msg->ang_vel < -M_PI / 16) {
        s->chaser.ang_vel = -M_PI / 16;
    } else {
        s->chaser.ang_vel = msg->ang_vel;
    }
}

int main(int argc, char *argv[]) {
    lcm_t *lcm = lcm_create(NULL);
    int seconds = 0;
    state_t state = {0};
    //settings_t settings = {0};
    reset_t reset = {0};
    //action_t action = {0};
    world_t world = {0};
    //agent_t runner = {0};
    //agent_t chaser = {0};
    setup_sim(&state);
    state.lcm = lcm;
    /*reset_t_subscription_t *reset_sub* =*/ reset_t_subscribe(lcm, "RESET_bkarw", handle_reset, &state);
    /* settings_t_subscription_t *settings_sub = */settings_t_subscribe(lcm, "SETTINGS_bkarw",
                                                                   handle_settings, &state);
    /*action_t_subscription_t *action_sub = */action_t_subscribe(lcm, "ACTION_bkarw",
                                                             handle_action, &state);
    while (true) {
        double start = seconds_now();
        lcm_handle_async(state.lcm);
        moveBot(&state.chaser);
        applyAction(&state.runner, runnerAction());
        moveBot(&state.runner);
        resolveWallCollisions(&state.chaser);
        resolveWallCollisions(&state.runner);
        if (robCollision(state.runner, state.chaser)) {
            printf("\e[2K\rRunner caught on step %d\n", state.time_step);
            reset_sim(&state);
            reset_t_publish(lcm, "RESET_bkarw", &reset);
        } else {
            state.time_step++;
        }
        world.chaser = state.chaser;
        world.runner = state.runner;
        world_t_publish(lcm, "WORLD_bkarw", &world);
        long nanoseconds = 40000 * 1000 / state.delay_every;
        nanoseconds -= (long)((seconds_now() - start) * 1000 * 1000);
        struct timespec interval = {seconds, nanoseconds};
        nanosleep(&interval, NULL);
    }
    reset_t_unsubscribe(lcm, reset_sub);
    settings_t_unsubscribe(lcm, settings_sub);
    action_t_unsubscribe(lcm, action_sub);
    return 0;
}

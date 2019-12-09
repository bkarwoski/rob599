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
    int action = rand() % 20;
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
    //applyAction(bot, action);
    //TODO: check inputs
    bot->theta += bot->ang_vel;
    bot->ang_vel *= 0.8;
    bot->x += bot->vel * cos(bot->theta);
    bot->y += bot->vel * -sin(bot->theta);
}
//TODO: split up field control into world / controller
// void field_control(state_t *s) {
//     double height = ROB_HEIGHT;
//     double width = height * 4 / 3.0;
//     double robot_r = sqrt((height / 2) * (height / 2) + (width / 2 * width / 2));
//     double wall_r = BLOCK_SIZE / sqrt(2);
//     double fx = 0;
//     double fy = 0;
//     double dx = s->runner.x - s->chaser.x;
//     double dy = s->runner.y - s->chaser.y;
//     double to_goal_dist = sqrt(dx * dx + dy * dy);
//     double dx_norm = dx / to_goal_dist;
//     double dy_norm = dy / to_goal_dist;
//     fx += dx_norm * s->to_goal_magnitude * pow(to_goal_dist, s->to_goal_power);
//     fy += dy_norm * s->to_goal_magnitude * pow(to_goal_dist, s->to_goal_power);

//     for (int x = 0; x < (int)MAP_W; x++) {
//         for (int y = 0; y < (int)MAP_H; y++) {
//             int i = x + (int)MAP_W * y;
//             if (MAP[i] == 'X') {
//                 double bx = BLOCK_SIZE * (x + 0.5);
//                 double by = BLOCK_SIZE * (y + 0.5);
//                 double obs_center_dist = sqrt(pow(s->chaser.x - bx, 2) + pow(s->chaser.y - by, 2));
//                 double to_obs_dist = obs_center_dist - robot_r - wall_r;
//                 to_obs_dist = fmax(0.1, to_obs_dist);
//                 double to_chaserx = (s->chaser.x - bx) / obs_center_dist;
//                 double to_chasery = (s->chaser.y - by) / obs_center_dist;
//                 fx += to_chaserx * s->avoid_obs_magnitude * pow(to_obs_dist, s->avoid_obs_power);
//                 fy += to_chasery * s->avoid_obs_magnitude * pow(to_obs_dist, s->avoid_obs_power);
//             }
//         }
//     }
//     double target_theta = atan2(-fy, fx);
//     double theta_error = target_theta - s->chaser.theta;
//     while (theta_error > M_PI) {
//         theta_error -= 2 * M_PI;
//     }
//     while (theta_error < -M_PI) {
//         theta_error += 2 * M_PI;
//     }
//     s->chaser.ang_vel = 0.4 * theta_error;
//     if (s->chaser.ang_vel > M_PI / 16) {
//         s->chaser.ang_vel = M_PI / 16;
//     }
//     if (s->chaser.ang_vel < -M_PI / 16) {
//         s->chaser.ang_vel = -M_PI / 16;
//     }
//     s->chaser.vel = fmin(s->max_velocity, s->chaser.vel + 2.0);
// }

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
    //long nanoseconds = 40 * 1000 * 1000;
    //struct timespec interval = {seconds, nanoseconds};
    state_t state = {0};
    settings_t settings = {0};
    reset_t reset = {0};
    action_t action = {0};
    world_t world = {0};
    agent_t runner = {0};
    agent_t chaser = {0};
    setup_sim(&state);
    state.lcm = lcm;
    reset_t_subscription_t *reset_sub = reset_t_subscribe(lcm, "RESET_bkarw", handle_reset, NULL);
    settings_t_subscription_t *settings_sub = settings_t_subscribe(lcm, "SETTINGS_bkarw",
                                                                   handle_settings, NULL);
    action_t_subscription_t *action_sub = action_t_subscribe(lcm, "ACTION_bkarw",
                                                             handle_action, NULL);
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
        world_t_publish(lcm, "WORLD_bkarw", &world);
        long nanoseconds = 40 * 1000 * 1000 / (int)state.delay_every;
        nanoseconds -= (long)((seconds_now() - start) * 1000 * 1000);
        struct timespec interval = {seconds, nanoseconds};
        nanosleep(&interval, NULL);
    }
    reset_t_unsubscribe(lcm, reset_sub);
    settings_t_unsubscribe(lcm, settings_sub);
    action_t_unsubscribe(lcm, action_sub);
    return 0;
}

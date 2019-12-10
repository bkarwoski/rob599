#define _GNU_SOURCE
#include <lcm/lcm.h>
//#include <termios.h>
//#include <time.h>
//#include "collision.h"
//#include "graphics.h" //needed for collision resolution
#include "settings.h"
#include "lcmtypes/action_t.h"
#include "lcmtypes/agent_t.h"
#include "lcmtypes/reset_t.h"
#include "lcmtypes/settings_t.h"
#include "lcmtypes/world_t.h"
//#include "lcm_handle_async.h"

typedef struct state {
    int user_action;
    //int time_step;
    agent_t runner;
    agent_t chaser;
    //int init_runner_idx;
    //int delay_every;
    //int select_idx;
    double to_goal_magnitude;
    int to_goal_power;
    double avoid_obs_magnitude;
    int avoid_obs_power;
    int max_velocity;
    lcm_t *lcm;
    action_t *action;
} state_t;

void field_control(state_t *s) {
    double height = ROB_HEIGHT;
    double width = height * 4 / 3.0;
    double robot_r = sqrt((height / 2) * (height / 2) + (width / 2 * width / 2));
    double wall_r = BLOCK_SIZE / sqrt(2);
    double fx = 0;
    double fy = 0;
    double dx = s->runner.x - s->chaser.x;
    double dy = s->runner.y - s->chaser.y;
    double to_goal_dist = sqrt(dx * dx + dy * dy);
    double dx_norm = dx / to_goal_dist;
    double dy_norm = dy / to_goal_dist;
    fx += dx_norm * s->to_goal_magnitude * pow(to_goal_dist, s->to_goal_power);
    fy += dy_norm * s->to_goal_magnitude * pow(to_goal_dist, s->to_goal_power);

    for (int x = 0; x < (int)MAP_W; x++) {
        for (int y = 0; y < (int)MAP_H; y++) {
            int i = x + (int)MAP_W * y;
            if (MAP[i] == 'X') {
                double bx = BLOCK_SIZE * (x + 0.5);
                double by = BLOCK_SIZE * (y + 0.5);
                double obs_center_dist = sqrt(pow(s->chaser.x - bx, 2) + pow(s->chaser.y - by, 2));
                double to_obs_dist = obs_center_dist - robot_r - wall_r;
                to_obs_dist = fmax(0.1, to_obs_dist);
                double to_chaserx = (s->chaser.x - bx) / obs_center_dist;
                double to_chasery = (s->chaser.y - by) / obs_center_dist;
                fx += to_chaserx * s->avoid_obs_magnitude * pow(to_obs_dist, s->avoid_obs_power);
                fy += to_chasery * s->avoid_obs_magnitude * pow(to_obs_dist, s->avoid_obs_power);
            }
        }
    }
    double target_theta = atan2(-fy, fx);
    double theta_error = target_theta - s->chaser.theta;
    if (theta_error > M_PI) {
        theta_error -= 2 * M_PI;
    }
    if (theta_error < -M_PI) {
        theta_error += 2 * M_PI;
    }
    s->chaser.ang_vel = 0.4 * theta_error;
    if (s->chaser.ang_vel > M_PI / 16) {
        s->chaser.ang_vel = M_PI / 16;
    }
    if (s->chaser.ang_vel < -M_PI / 16) {
        s->chaser.ang_vel = -M_PI / 16;
    }
    // s->chaser.vel = fmin(s->max_velocity, s->chaser.vel + 2.0);
    s->action->ang_vel = s->chaser.ang_vel;
    s->action->vel = s->chaser.vel;
}

void handle_settings(const lcm_recv_buf_t *rbuf, const char *channel,
            const settings_t *msg, void *userdata) {
                //printf("handling settings\n");
    state_t *s = (state_t *)userdata;
    s->avoid_obs_magnitude = msg->avoid_obs_magnitude;
    s->avoid_obs_power = msg->avoid_obs_power;
    s->max_velocity = msg->max_vel;
    s->to_goal_magnitude = msg->to_goal_magnitude;
    s->to_goal_power = msg->to_goal_power;
}

void handle_world(const lcm_recv_buf_t *rbuf, const char *channel,
            const world_t *msg, void *userdata) {
                printf("handling world\n");
    state_t *s = (state_t *)userdata;
    s->chaser = msg->chaser;
    s->runner = msg->chaser;
    field_control(s);
    action_t_publish(s->lcm, "ACTION_bkarw", s->action);
}

int main (int argc, char *argv[]) {
    agent_t chaser = {0};
    agent_t runner = {0};
    action_t action = {0};
    runner.is_runner = true;
    state_t state = {0};
    state.runner = runner;
    state.chaser = chaser;
    state.action = &action;
    state.lcm = lcm_create(NULL);
    world_t world = {0};
    //action_t action = {0};
    settings_t settings = {0};
    settings_t_subscription_t *settings_sub = settings_t_subscribe(state.lcm, "SETTINGS_bkarw",
                                                                   handle_settings, &state);
    world_t_subscription_t *world_sub = world_t_subscribe(state.lcm, "WORLD_bkarw",
                                                                   handle_world, &state);
    while (true) {
        lcm_handle(state.lcm);
        //printf("looping lcm_handle\n");
    }
    return 0;
}

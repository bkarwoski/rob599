#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <pthread.h>
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
#define MAP_W (1.0 * WIDTH / BLOCK_SIZE)
#define MAP_H (1.0 * HEIGHT / BLOCK_SIZE)
#define ROB_HEIGHT 20
#define ROB_LENGTH (ROB_HEIGHT * 4 / 3.0)
#define HL_ON "\e[7m"
#define HL_OFF "\e[0m"

typedef struct settings {
    int32_t initial_runner_idx;
    int32_t delay_every;
    double to_goal_magnitude;
    int32_t to_goal_power;
    double avoid_obs_magnitude;
    int32_t avoid_obs_power;
    double max_vel;
} settings_t;

typedef struct reset {
    int32_t initial_runner_idx;
} reset_t;

typedef struct agent {
    bool is_runner;
    double x;
    double y;
    double theta;
    double vel;
    double ang_vel;
} agent_t;

typedef struct state {
    int user_action;
    int time_step;
    bitmap_t bmp;
    size_t image_size;
    uint8_t *image_data;
    bool runner_caught;
    agent_t runner;
    agent_t chaser;
    int init_runner_idx;
    int delay_every;
    int select_idx;
    double to_goal_magnitude;
    int to_goal_power;
    double avoid_obs_magnitude;
    int avoid_obs_power;
    int max_velocity;
} state_t;

typedef struct action {
    double vel;
    double ang_vel;
} action_t;

typedef struct world_t {
    agent_t runner;
    agent_t chaser;
}
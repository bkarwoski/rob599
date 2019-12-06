#define _GNU_SOURCE
#include <lcm/lcm.h>
#include <termios.h>
#include <time.h>
#include "bmp.h"
#include "graphics.h"
#include "image_server.h"
#include "settings.h"
#include "lcmtypes/settings_t.h"
#include "lcmtypes/reset_t.h"

struct termios original_termios;

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

void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    //reactivate blinking cursor
    printf("\e[?25h\n");
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
    s->to_goal_magnitude = 50.0;
    s->to_goal_power = 0;
    s->avoid_obs_magnitude = 1;
    s->avoid_obs_power = -2;
    s->max_velocity = 12;
    s->bmp.width = WIDTH;
    s->bmp.height = HEIGHT;
    s->bmp.data = calloc(s->bmp.width * s->bmp.height, sizeof(color_bgr_t));
    s->image_size = bmp_calculate_size(&s->bmp);
    s->image_data = malloc(s->image_size);
    s->select_idx = 0;
    reset_sim(s);
}

void handle_up(state_t *s) {
    if (s->select_idx == 0) {
        s->init_runner_idx++;
        while (MAP[s->init_runner_idx] == 'X') {
            s->init_runner_idx++;
        }
    }
    if (s->select_idx == 1) {
        s->delay_every++;
    }
    if (s->select_idx == 2) {
        s->to_goal_magnitude *= 2;
    }
    if (s->select_idx == 3) {
        if (s->to_goal_power <= 2) {
            s->to_goal_power++;
        }
    }
    if (s->select_idx == 4) {
        s->avoid_obs_magnitude *= 2;
    }
    if (s->select_idx == 5) {
        if (s->avoid_obs_power <= 2) {
            s->avoid_obs_power++;
        }
    }
    if (s->select_idx == 6) {
        if (s->max_velocity <= 11) {
            s->max_velocity++;
        }
    }
}

void handle_down(state_t *s) {
    if (s->select_idx == 0) {
        s->init_runner_idx--;
        while (MAP[s->init_runner_idx] == 'X') {
            s->init_runner_idx--;
        }
    }
    if (s->select_idx == 1) {
        if (s->delay_every > 1) {
            s->delay_every--;
        }
    }
    if (s->select_idx == 2) {
        s->to_goal_magnitude /= 2;
    }
    if (s->select_idx == 3) {
        if (s->to_goal_power >= -2) {
            s->to_goal_power--;
        }
    }
    if (s->select_idx == 4) {
        s->avoid_obs_magnitude /= 2;
    }
    if (s->select_idx == 5) {
        if (s->avoid_obs_power >= -2) {
            s->avoid_obs_power--;
        }
    }
    if (s->select_idx == 6) {
        if (s->max_velocity > 1) {
            s->max_velocity--;
        }
    }
}

void disp_interface(state_t *s) {
    //printf("\r %stesting%s index = %d", HL_ON, HL_OFF, s->select_idx);
    printf("\ridx=");
    printf("%s%d%s", s->select_idx == 0 ? HL_ON : "", s->init_runner_idx, HL_OFF);
    //printf("%s%8.2f%s", goal_mag_selected ? HIGHLIGHT : "", to_goal_magnitude, CLEAR_HIGHLIGHT);
    printf(" delay_every=");
    printf("%s%d%s", s->select_idx == 1 ? HL_ON : "", s->delay_every, HL_OFF);

    printf(" to_goal_mag=");
    printf("%s%4.2f%s", s->select_idx == 2 ? HL_ON : "", s->to_goal_magnitude, HL_OFF);

    printf(" to_goal_pow=");
    printf("%s%d%s", s->select_idx == 3 ? HL_ON : "", s->to_goal_power, HL_OFF);

    printf(" avoid_obs_mag=");
    printf("%s%4.2f%s", s->select_idx == 4 ? HL_ON : "", s->avoid_obs_magnitude, HL_OFF);

    printf(" avoid_obs_pow=");
    printf("%s%d%s", s->select_idx == 5 ? HL_ON : "", s->avoid_obs_power, HL_OFF);

    printf(" max_vel=");
    printf("%s%d%s", s->select_idx == 6 ? HL_ON : "", s->max_velocity, HL_OFF);
    fflush(stdout);
}

void *io_thread(void *user) {
    //deactivate blinking cursor
    printf("\e[?25l\n");
    state_t *state = user;
    tcgetattr(0, &original_termios);
    atexit(reset_terminal);
    struct termios new_termios;
    memcpy(&new_termios, &original_termios, sizeof(new_termios));
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    while (true) {
        int c = getc(stdin);
        if (c == 'q') {
            exit(0);
        }
        if (c == 'r') {
            reset_sim(state);
        }
        if (c == 27) {
            c = getc(stdin);
            if (c == 91) {
                c = getc(stdin);
                if (c == 'A') {
                    handle_up(state);
                } else if (c == 'B') {
                    handle_down(state);
                } else if (c == 'C') {
                    //right
                    state->select_idx++; //update to wraparound
                } else if (c == 'D') {
                    //left
                    state->select_idx--;
                }
            }
        }
    }
}

void drawMap(bitmap_t *bmp) {
    color_bgr_t black = {0, 0, 0};
    color_bgr_t white = {255, 255, 255};
    for (int i = 0; i < bmp->width * bmp->height; i++) {
        bmp->data[i] = white;
    }
    for (int i = 0; i < MAP_W * MAP_H; i++) {
        if (MAP[i] == 'X') {
            vector_xy_t nextBlock = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
            double xPos = BLOCK_SIZE / 2.0 + (i % (int)MAP_W) * BLOCK_SIZE;
            double yPos = BLOCK_SIZE / 2.0 + ((i - i % (int)MAP_W) * 1.0 / MAP_W) * BLOCK_SIZE;
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

double seconds_now(void) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now)) {
        fprintf(stderr, "Retrieving system time failed.\n");
        exit(1);
    }
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

int main(int argc, char *argv[]) {
    lcm_t *lcm = lcm_create(NULL);
    state_t state = {0};
    setup_sim(&state);
    int seconds = 0;
    long nanoseconds = 40000 * 1000;
    pthread_t userInput;
    pthread_create(&userInput, NULL, io_thread, &state);
    settings_t settings = {0};
    if (argc < 2) {
        image_server_start("8000");
    }
    while (true) {
        double start = seconds_now();
        //lcm_t *lcm = lcm_create(NULL);
        //settings_t settings = {0};
        settings.avoid_obs_magnitude = state.avoid_obs_magnitude;
        settings.avoid_obs_power = state.avoid_obs_power;
        settings.delay_every = state.delay_every;
        settings.initial_runner_idx = state.init_runner_idx;
        settings.max_vel = state.max_velocity;
        settings.to_goal_magnitude = state.to_goal_magnitude;
        settings.to_goal_power = state.to_goal_power;
        if (argc < 2) {
            updateGraphics(&state);
            bmp_serialize(&state.bmp, state.image_data);
            image_server_set_data(state.image_size, state.image_data);
        }
        nanoseconds -= (long)((seconds_now() - start) * 1000 * 1000);
        struct timespec interval = {seconds, nanoseconds};
        nanosleep(&interval, NULL);

        settings_t_publish(lcm, "SETTINGS_bkarw", &settings);
    }

    return 0;
}

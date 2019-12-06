#define _GNU_SOURCE
#include <termios.h>
#include "settings.h"

struct termios original_termios;

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

int main(int argc, char *argv[]) {
    state_t state = {0};
    setup_sim(&state);
    pthread_t userInput;
    pthread_create(&userInput, NULL, io_thread, &state);
    if (argc < 2) {
        image_server_start("8000");
    }
    while (true) {
        if (argc < 2) {
            updateGraphics(&state);
            bmp_serialize(&state.bmp, state.image_data);
            image_server_set_data(state.image_size, state.image_data);
        }
    }

    return 0;
}
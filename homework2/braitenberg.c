#define _GNU_SOURCE
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "graphics.h"
#include "image_server.h"
#define M_PI 3.14159265358979323846
#define LAMP_XS (double[]) {124.1, 349.1, 449.1}
#define LAMP_YS (double[]) {224.1, 99.1, 349.1}
#define LAMP_N (sizeof(LAMP_XS) / sizeof(LAMP_XS[0]))

void drawBck(bitmap_t *bmp) {
    color_bgr_t black = {0, 0, 0};
    color_bgr_t yellow = {0, 255, 255};
    color_bgr_t white = {255, 255, 255};

    for(int i = 0; i < bmp->width * bmp->height; i++){
        bmp->data[i] = black;
    }
    double lampLen = 12;
    for(int i = 0; i < LAMP_N; i++) {
        vector_xy_t lamp = gx_rect(lampLen, lampLen);
        gx_rot(M_PI / 4, &lamp);
        gx_trans(LAMP_XS[i], LAMP_YS[i], &lamp);
        gx_fill_poly(bmp, yellow, &lamp);
        vector_delete(&lamp);
    }
    vector_xy_t border = gx_rect(600, 440);
    gx_trans(640 / 2, 480 / 2, &border);
    roundC(&border);
    gx_draw_poly(bmp, white, &border);
    vector_delete(&border);
}

double dp(double v[], double u[]) {
    double result = 0;
    //printf("v[0] = %f\n", v[0]);
    //printf("v[1] = %f\n", v[1]);
    for (int i = 0; i < 2; i++) {
        result += v[i] * u[i];
    }
    return result;
}

void updatePos(double *x, double *y, double *theta) {
    double c = cos(*theta);
    double s = sin(*theta);
    double move_l = 0;
    double move_r = 0;
    double lamp_power = 100000;
    double max_move = 12;
    double wheelbase = 80;
    for (int i = 0; i < LAMP_N; i++) {
        double xd = LAMP_XS[i] - *x;
        double yd = LAMP_YS[i] - *y;
        double dist_sq = xd * xd + yd * yd;
        double dir[] = {xd / sqrt(dist_sq), yd / sqrt(dist_sq)};

        double cl = cos(*theta + M_PI / 3);
        double sl = sin(*theta + M_PI / 3);
        double eye_l[] = {cos(*theta + M_PI / 3), -sin(*theta + M_PI / 3)};
        
        double cr = cos(*theta + -M_PI / 3);
        double sr = sin(*theta + -M_PI / 3);
        double eye_r[] = {cos(*theta - M_PI / 3), -sin(*theta - M_PI / 3)};
        
        move_l += fmax(0.0, dp(dir, eye_r)) * lamp_power / dist_sq;
        move_r += fmax(0.0, dp(dir, eye_l)) * lamp_power / dist_sq;

    }
    move_l = fmin(max_move, move_l);
    move_r = fmin(max_move, move_r);

    *theta += (move_r - move_l) / wheelbase;
    double forward_dist = (move_l + move_r) / 2;
    *x += forward_dist * cos(*theta);
    *y += forward_dist * -sin(*theta);
}

void updateGraphics(bitmap_t *bmp, double x, double y, double theta) {
    drawBck(bmp);
    vector_xy_t rob = gx_rob();
    gx_rot(theta, &rob);
    gx_trans(x, y, &rob);
    color_bgr_t green = {0, 255, 0};
    gx_fill_poly(bmp, green, &rob);
    vector_delete(&rob);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: : %s <time steps> <fast=0|1>\n", argv[0]);
        return 1;
    }
    bool isFast = atoi(argv[2]);
    int numSteps = atoi(argv[1]);

    

    double rob_x = 640 / 2;
    double rob_y = 480 / 2;
    double rob_theta = 0;
    for(int i = 0; i <= numSteps; i++) {
        bitmap_t bmp = {0};
        bmp.width = 640;
        bmp.height = 480;
        bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
        size_t bmp_size = bmp_calculate_size(&bmp);
        uint8_t *serialized_bmp = malloc(bmp_size);
        color_bgr_t white = {255, 255, 255};

        drawBck(&bmp);
        updateGraphics(&bmp,rob_x, rob_y, rob_theta);
        updatePos(&rob_x, &rob_y, &rob_theta);
        if (!isFast) {
            printf("delaying\n");
            int seconds = 0;
            long nanoseconds = 40 * 1000 * 1000;
            struct timespec interval = { seconds, nanoseconds };
            nanosleep(&interval, NULL);
        }
            bmp_serialize(&bmp, serialized_bmp);
            FILE *f = fopen("my_image.bmp", "wb");
            fwrite(serialized_bmp, bmp_size, 1, f);
            fclose(f);

            image_server_set_data(bmp_size, serialized_bmp);
            image_server_start("8000");
            //sleep(1);
            free(bmp.data);
            free(serialized_bmp); 
    }
   
    return 0;
}

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

bool intersects(double x1, double y1, double x2, double y2,
                double x3, double y3, double x4, double y4) {   
    double vec1[2] = {x2 - x1, y2 - y1};
    double t11[2] = {x3 - x1, y3 - y1};
    double t12[2] = {x4 - x1, y4 - y1};
    double cv1t11 = vec1[0] * t11[1] - vec1[1] * t11[0];
    double cv1t12 = vec1[0] * t12[1] - vec1[1] * t12[0];
    bool neg1 = (cv1t11 * cv1t12 <= 0);

    double vec2[2] = {x4 - x3, y4 - y3};
    double t21[2] = {x1 - x3, y1 - y3};
    double t22[2] = {x2 - x3, y2 - y3};
    double cv2t21 = vec2[0] * t21[1] - vec2[1] * t21[0];
    double cv2t22 = vec2[0] * t22[1] - vec2[1] * t22[0];
    bool neg2 = (cv2t21 * cv2t22 <= 0);

    bool intersected = (neg1 && neg2) && !((cv1t11 * cv1t12 == 0) && (cv2t21 * cv2t22 == 0));
    if (intersected) {
        printf("intersected\n");
    }
    return intersected;
}

bool contains(double x, double y, vector_xy_t *polyX) {
    int posCount = 0;
    int negCount = 0;
    for (int i = 0; i < polyX->size; i++) {
        int x1 = polyX->xData[i];
        int y1 = polyX->yData[i];
        int x2 = polyX->xData[(i + 1) % polyX->size];
        int y2 = polyX->yData[(i + 1) % polyX->size];
        double vec1[2] = {x2 - x1, y2 - y1};
        double t1[2] = {x - x1, y - y1};
        double cross = vec1[0] * t1[1] - vec1[1] * t1[0];
        printf("cross: %f x1: %d y1: %d x2: %d y2: %d\n", cross, x1, y1, x2, y2);
        if (cross < 0){
            negCount++;
        } else if (cross > 0) {
            posCount++;
        }
    }
    if (negCount > 0 && posCount > 0) {
        return false;
    }
    return false;
}

bool collision(vector_xy_t *poly1, vector_xy_t *poly2) {
    for (int k = 0; k < poly1->size; k++) {
        double x1 = poly1->xData[k];
        double y1 = poly1->yData[k];
        double x2 = poly1->xData[(k + 1) % poly1->size];
        double y2 = poly1->yData[(k + 1) % poly1->size];
        for (int j = 0; j < poly2->size; j++) {
            double x3 = poly2->xData[j];
            double y3 = poly2->yData[j];
            double x4 = poly2->xData[(j + 1) % poly2->size];
            double y4 = poly2->yData[(j + 1) % poly2->size];
            if (intersects(x1, y1, x2, y2, x3, y3, x4, y4)) {
                return true;
            }
        }
    }
    return contains(poly1->xData[0], poly1->yData[0], poly2) ||
           contains(poly2->xData[0], poly2->yData[0], poly1);
}

bool robcollision(double x, double y, double theta, int lampN) {
    double lampLen = 12;
    vector_xy_t lamp = gx_rect(lampLen, lampLen);
    gx_rot(M_PI / 4, &lamp);
    gx_trans(LAMP_XS[lampN], LAMP_YS[lampN], &lamp);

    vector_xy_t rob = gx_rob();
    gx_rot(theta, &rob);
    gx_trans(x, y, &rob);
    // printf("x = %f, y = %f, theta = %f\n", x, y, theta);
    bool collides = collision(&lamp, &rob);

    vector_delete(&rob);
    vector_delete(&lamp);

    if (collides) {
        return true;
    }

    return false;
}

void handleCollision(double *rob_x, double *rob_y, double *rob_theta) {
    //for each lamp
    //while a collision occurs
    //if there is a collision, calculate the vector from robot to lamp
    //move robot coordinates 0.5 units along this vector
    printf("position: x = %f, y = %f\n", *rob_x, *rob_y);
    for (int i = 0; i < LAMP_N; i++) {
        int count = 0;
        while (robcollision(*rob_x, *rob_y, *rob_theta, i)) {
            count++;
            double dx = *rob_x - LAMP_XS[i];
            double dy = *rob_y - LAMP_YS[i];
            double dist = sqrt(dx * dx + dy * dy);
            *rob_x += dx / dist * 0.5;
            *rob_y += dy / dist * 0.5;
            printf("moving 0.5\n");
            printf("rob_x = %f\n", *rob_x);
            printf("rob_y = %f\n", *rob_y);
            if(count == 8) {
                printf("count is %d\n", count);
            }

        }
    }

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
    int seconds = 0;
    long nanoseconds = 40 * 1000 * 1000;
    struct timespec interval = { seconds, nanoseconds };
    for(int i = 0; i <= numSteps; i++) {
        printf("step %d\n", i);
        bitmap_t bmp = {0};
        bmp.width = 640;
        bmp.height = 480;
        bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
        size_t bmp_size = bmp_calculate_size(&bmp);
        uint8_t *serialized_bmp = malloc(bmp_size);
        color_bgr_t white = {255, 255, 255};

        if (!isFast || i == numSteps) {
            drawBck(&bmp);
            updateGraphics(&bmp,rob_x, rob_y, rob_theta);
            nanosleep(&interval, NULL);

            bmp_serialize(&bmp, serialized_bmp);
            image_server_set_data(bmp_size, serialized_bmp);
            image_server_start("8000");
        }

        updatePos(&rob_x, &rob_y, &rob_theta);
        handleCollision(&rob_x, &rob_y, &rob_theta);

        if (isFast && i == numSteps) {
            sleep(1);
        }
        free(bmp.data);
        free(serialized_bmp); 
    }
   
    return 0;
}

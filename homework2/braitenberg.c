#define _GNU_SOURCE
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
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
    gx_draw_poly(bmp, white, &border);
    vector_delete(&border);

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

    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    //make bmp
    updateGraphics(&bmp,640 / 2, 480 / 2, 0);

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);
    free(bmp.data);
    free(serialized_bmp);    
    return 0;
}

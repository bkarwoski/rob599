#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#include "image_server.h"
#include "bmp.h"
#include "graphics.h"
#include "vector_xy_t.h"
#include "vector_xy_i32_t.h"
#define M_PI 3.14159265358979323846

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: ./rasterize <test case>\n");
        return 1;
    }

    int caseNum = atoi(argv[1]);
    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    if (caseNum == 2) {
        gx_draw_line(&bmp, white, 10, 10, 200, 200);
    } else if (caseNum == 3) {
        vector_xy_t rect = gx_rect(4, 4);
        roundC(&rect);
        gx_draw_poly(&bmp, white, &rect);
        vector_delete(&rect);
    } else if (caseNum == 4) {
        vector_xy_t rect = gx_rect(4, 4);
        gx_trans(2, 2, &rect);
        roundC(&rect);
        gx_draw_poly(&bmp, white, &rect);
        vector_delete(&rect);
    } else if (caseNum == 5) {
        vector_xy_t rect = gx_rect(5, 5);
        gx_trans(2, 2, &rect);
        roundC(&rect);
        gx_draw_poly(&bmp, white, &rect);
        vector_delete(&rect);
    } else if (caseNum == 6) {
        vector_xy_t rect = gx_rect(600, 440);
        gx_trans(640 / 2, 480 / 2, &rect);
        roundC(&rect);
        gx_draw_poly(&bmp, white, &rect);
        vector_delete(&rect);
    } else if (caseNum == 7) {
        vector_xy_t rect = gx_rect(600, 440);
        gx_trans(640 / 2, 480 / 2, &rect);
        gx_fill_poly(&bmp, white, &rect);
        vector_delete(&rect);
    } else if (caseNum == 8) {
        vector_xy_t rob = gx_rob();
        gx_trans(400, 400, &rob);
        gx_fill_poly(&bmp, white, &rob);
        vector_delete(&rob);
    } else if (caseNum == 9) {
        vector_xy_t rob = gx_rob();
        gx_rot(M_PI / 6, &rob);
        gx_trans(400, 400, &rob);
        gx_fill_poly(&bmp, white, &rob);
        vector_delete(&rob);
    } else {
        printf("usage: ./rasterize <test case>\n");
        return 1;
    }
    //make the bmp

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    //free vectors
    free(bmp.data);
    free(serialized_bmp);

   
}

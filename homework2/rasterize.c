#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#include "image_server.h"
#include "bmp.h"
#include "graphics.h"
#include "vector_xy_t.h"
#include "vector_xy_i32_t.h"


void case_1 (void) {
    //all black
    bitmap_t bmp = { 0 };
    bmp.width = 640;
    bmp.height = 480;
    // color_bgr_t represents the color for each pixel
    // calloc gives enough space for width*height of these pixels
    // and calloc also sets the initial values of all of these to zero (black)
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    free(bmp.data);
    free(serialized_bmp);
}

void case_2 (void) {
    vector_xy_i32_t coords = vector_i32_create();
    vector_i32_append(&coords, 250, 250);
    printf("coords: x: %d  y: %d\n", coords.xData[0], coords.yData[0]);

    //draw a white line from (10, 10) to (200, 200) on a black background of size 640 by 480
    bitmap_t bmp = { 0 };
    bmp.width = 640;
    bmp.height = 480;
    // color_bgr_t represents the color for each pixel
    // calloc gives enough space for width*height of these pixels
    // and calloc also sets the initial values of all of these to zero (black)
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};
    gx_plotLine(10, 10, 200, 200, &bmp, white);
    gx_draw(&bmp, white, &coords);

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    vector_i32_delete(&coords);
    free(bmp.data);
    free(serialized_bmp);
}

int main(int argc, char*argv[]) {
    if (argc != 2) {
        printf("usage: ./rasterize <test case>\n");
        return 1;
    }
    int i = atoi(argv[1]);
    if (i == 1) {
        case_1();
        return 0;
    }
    if (i == 2) {
        case_2();
        return 0;
    }

    printf("usage: ./rasterize <test case>\n");
    return 1;
}

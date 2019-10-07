#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#include "image_server.h"
#include "bmp.h"
#include "graphics.h"
#include "vector_xy_t.h"
#include "vector_xy_i32_t.h"

/*
void case_2(void) {
    //draw a white line from (10, 10) to (200, 200) on a black background of size 640 by 480
    vector_xy_t coords = vector_create();
    vector_append(&coords, 250, 250);
    //printf("coords: x: %d  y: %d\n", coords.xData[0], coords.yData[0]);
    gx_draw_line()
    //gx_plotLine(10, 10, 200, 200, &bmp, white);
    //gx_draw(&bmp, white, &coords);

    vector_delete(&coords);
}

void case_3(void) {
    //draw a 4 by 4 pixel white square centered at (0, 0)
    vector_xy_t sqPoints = vector_create();

    vector_append(&sqPoints, 2, -2);
    vector_append(&sqPoints, 2, 2);
    vector_append(&sqPoints, -2, 2);
    vector_append(&sqPoints, -2, -2);

    roundC(&sqPoints);
    //printf("sqPoints.xData[0] = %f\n", sqPoints.xData[0]);

    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    vector_xy_t perimeter = vector_create();
    int x0 = 0;
    int y0 = 0;
    int x1 = 0;
    int y1 = 0;
    for (int i = 0; i < sqPoints.size; i++) {
        x0 = sqPoints.xData[i];
        y0 = sqPoints.yData[i];
        x1 = sqPoints.xData[(i + 1) % sqPoints.size];
        y1 = sqPoints.yData[(i + 1) % sqPoints.size];
        gx_rasterize_line(x0, y0, x1, y1, &perimeter);
    }
    
    printf("sqPoints.xData[3]: %f\n", sqPoints.xData[3]);
    printf("sqPoints.yData[3]: %f\n", sqPoints.yData[3]);
    gx_draw(&bmp, white, &perimeter);

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    vector_delete(&sqPoints);
    vector_delete(&perimeter);
    free(bmp.data);
    free(serialized_bmp);
}

void case_4(void) {
    //draw a 4 by 4 pixel white square centered at (2, 2)
    vector_xy_t sqPoints = vector_create();

    vector_append(&sqPoints, 2, -2);
    vector_append(&sqPoints, 2, 2);
    vector_append(&sqPoints, -2, 2);
    vector_append(&sqPoints, -2, -2);

    roundC(&sqPoints);

    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    vector_xy_t perimeter = vector_create();
    gx_perimeter(&sqPoints, &perimeter);
    //gx_trans(double x, double y, vector_xy_t *orig, vector_xy_t *shifted)
    gx_trans(2, 2, &perimeter, &perimeter);
    
    printf("sqPoints.xData[3]: %f\n", sqPoints.xData[3]);
    printf("sqPoints.yData[3]: %f\n", sqPoints.yData[3]);
    gx_draw(&bmp, white, &perimeter);

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    vector_delete(&sqPoints);
    vector_delete(&perimeter);
    free(bmp.data);
    free(serialized_bmp);
}

void case_5(void) {
    // draw a 5 by 5 pixel white square centered at (2, 2)
    vector_xy_t sqPoints = vector_create();

    vector_append(&sqPoints, 2.5, -2.5);
    vector_append(&sqPoints, 2.5, 2.5);
    vector_append(&sqPoints, -2.5, 2.5);
    vector_append(&sqPoints, -2.5, -2.5);

    roundC(&sqPoints);

    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    vector_xy_t perimeter = vector_create();
    gx_perimeter(&sqPoints, &perimeter);
    gx_trans(2, 2, &perimeter, &perimeter);
    gx_draw(&bmp, white, &perimeter);

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    vector_delete(&sqPoints);
    vector_delete(&perimeter);
    free(bmp.data);
    free(serialized_bmp);
}

void case_6(void) {
    // draw a 600 (width – 40) by 440 (height – 40)
    // white border centered at the center of the image (640 / 2, 480 / 2)
    vector_xy_t sqPoints = vector_create();
    int width = 600;
    int height = 440;
    vector_append(&sqPoints, width / 2, -1 * height/2);
    vector_append(&sqPoints, width / 2, height / 2);
    vector_append(&sqPoints, -1 * width / 2, height / 2);
    vector_append(&sqPoints, -1 * width / 2, -1 * height / 2);

    roundC(&sqPoints);

    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    vector_xy_t perimeter = vector_create();
    gx_perimeter(&sqPoints, &perimeter);
    gx_trans(640/2, 480/2, &perimeter, &perimeter);
    gx_draw(&bmp, white, &perimeter);

    vector_delete(&sqPoints);
    vector_delete(&perimeter);
    free(bmp.data);
    free(serialized_bmp);
}

void case_7(void) {
    //fill in the rectangle from the 6th test case.
    vector_xy_t sqPoints = vector_create();
    int width = 600;
    int height = 440;
    vector_append(&sqPoints, width / 2, -1 * height/2);
    vector_append(&sqPoints, width / 2, height / 2);
    vector_append(&sqPoints, -1 * width / 2, height / 2);
    vector_append(&sqPoints, -1 * width / 2, -1 * height / 2);

    roundC(&sqPoints);

    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    vector_xy_t perimeter = vector_create();
    gx_perimeter(&sqPoints, &perimeter);
    gx_trans(640/2, 480/2, &perimeter, &perimeter);
    //gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points)
    gx_fill(&perimeter);
    gx_draw(&bmp, white, &perimeter);

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    vector_delete(&sqPoints);
    vector_delete(&perimeter);
    free(bmp.data);
    free(serialized_bmp);
}

void case_8(void) {
    //fill in a robot shape, translated to/centered at (400, 400).
    //It should end up 21 pixels wide on the y-axis 
    //and then 28 long on the x-axis
    vector_xy_t tPoints = vector_create();
    double height = 21;
    //double width = height * 4 / 3;
    double width = 28;
    vector_append(&tPoints, width, 0);
    vector_append(&tPoints, 0, height/2);
    vector_append(&tPoints, 0, -1 * height/2);

    roundC(&tPoints);

    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    color_bgr_t white = {255, 255, 255};

    vector_xy_t perimeter = vector_create();
    gx_perimeter(&tPoints, &perimeter);
    gx_trans(400, 400, &perimeter, &perimeter);
    //gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points)
    gx_fill(&perimeter);
    gx_draw(&bmp, white, &perimeter);

    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");
    sleep(1);

    vector_delete(&tPoints);
    vector_delete(&perimeter);
    free(bmp.data);
    free(serialized_bmp);
}*/

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

    vector_xy_t coords = vector_create();
    if (caseNum == 2) {
    vector_append(&coords, 10, 10);
    vector_append(&coords, 200, 200);
    int x0 = coords.xData[0];
    int x1 = coords.xData[1];
    int y0 = coords.yData[0];
    int y1 = coords.yData[1];
    gx_draw_line(&bmp, white, x0, y0, x1, y1);
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
    vector_delete(&coords);
    free(bmp.data);
    free(serialized_bmp);

    printf("usage: ./rasterize <test case>\n");
    return 1;
}

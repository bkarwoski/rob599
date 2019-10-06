#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "bmp.h"
#include "image_server.h"
#include "vector_xy_t.h"
#include "vector_xy_i32_t.h"


void gx_plotLine(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color) {
    //Use ONLY for test case 2!
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -1*abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2 = 0;
    while (true) {
    bmp->data[y0*640+x0] = color;
        if (x0 == x1 && y0 == y1) {
            break;
        }
    e2 = 2 * err;
    if (e2 >= dy) {
        err += dy;
        x0 += sx;
    }
    if (e2 <= dx) {
        err += dx;
        y0 += sy;
    }
        
    }
}

void gx_rasterize_line(int x0, int y0, int x1, int y1, vector_xy_t *perimeter) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -1*abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2 = 0;
    while (true) {
    //bmp->data[y0*640+x0] = color;
    double x0d = x0;
    double y0d = y0;
    vector_append(perimeter, x0d, y0d);
        if (x0 == x1 && y0 == y1) {
            break;
        }
    e2 = 2 * err;
    if (e2 >= dy) {
        err += dy;
        x0 += sx;
    }
    if (e2 <= dx) {
        err += dx;
        y0 += sy;
    }
        
    }
}

void gx_draw(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points) {
    //printf("starting gx_draw\n");
    //printf("points->size: %ld\n", points->size);
    int index = 0;
    for (int i = 0; i < points->size; i++) {
        //printf("looping, i = %d\n", i);
        if (points->xData[i] >= 0 && points->yData[i] >= 0) {
            //printf("non negative value printing\n");
            index = points->xData[i] + points->yData[i]*640;
            bmp->data[index] = color;
        }
    }
}

void roundC(vector_xy_t *doubles) {
    double xMin = doubles->xData[0];
    double yMin = doubles->yData[0];
    double epsilon = 1e-6;
    int32_t newX = 0;
    int32_t newY = 0;
    for (int i = 1; i < doubles->size; i++) {
        if (doubles->xData[i] < xMin) {
            xMin = doubles->xData[i];
        }
        if (doubles->yData[i] < yMin) {
            yMin = doubles->yData[i];
        }
    }
    for (int i = 0; i < doubles->size; i++) {
        if (doubles->xData[i] == xMin) {
            newX = ceil(doubles->xData[i]);
        } else {
            newX = floor(doubles->xData[i] - epsilon);
        }
        if (doubles->yData[i] == yMin) {
            newY = ceil(doubles->yData[i]);
        } else {
            newY = floor(doubles->yData[i] - epsilon);
        }
        doubles->xData[i] = newX;
        doubles->yData[i] = newY;
    }

}

/*
vector_xy_t *gx_rect(double width, double height) {
    vector_xy_t rect = vector_create();
    vector_append(&rect, width, height);
    vector_append(&rect, -1*width, height);
    vector_append(&rect, -1*width, -1*height);
    vector_append(&rect, width, -1*height);
    return &rect;
}
*/
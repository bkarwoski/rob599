#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "bmp.h"
#include "image_server.h"
#include "vector_xy_t.h"
#include "vector_xy_i32_t.h"


void gx_plotLine(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color) {
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


void gx_draw(bitmap_t *bmp, color_bgr_t color, vector_xy_i32_t *points) {
    for (int i = 0; i < points->size; i++) {
        
        bmp->data[points->xData[i] + points->yData[i]*640] = color;
    }
}


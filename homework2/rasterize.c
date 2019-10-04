#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#include "image_server.h"
#include "bmp.h"

void plotLine(int x0, int y0, int x1, int y1, bitmap_t *bmp) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -1*abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2 = 0;
    while (true) {
    bmp->data[y0*640+x0].r = 255;
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

int main(void) {
    bitmap_t bmp = { 0 }; // initialize to zeros
    bmp.width = 640;
    bmp.height = 480;
    // color_bgr_t represents the color for each pixel
    // calloc gives enough space for width*height of these pixels
    // and calloc also sets the initial values of all of these to zero (black)
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));

    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    plotLine(5, 5, 50, 50, &bmp);
    bmp_serialize(&bmp, serialized_bmp);
    // write to a file so we can check if it is a valid image
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);
    
    // if we wanted to make the first pixel white

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000"); // you can change the port number
    sleep(1);

    // remember to later free the bmp.data when we are done using it
    free(bmp.data);
    free(serialized_bmp);

    return 0;
}

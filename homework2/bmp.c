#include "bmp.h"

// calculate the number of bytes of memory needed to serialize the bitmap
// that is, to write a valid bmp file to memory
size_t bmp_calculate_size(bitmap_t *bmp) {
    size_t s = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + sizeof(bmp);
    return s;
}

// write the bmp file to memory at data, which must be at least
// bmp_calculate_size large.
void bmp_serialize(bitmap_t *bmp, uint8_t *data) {
    BITMAPFILEHEADER file_header = { 0 }; // start out as all zero values
    file_header.bfType = 'BM';
    file_header.bfSize = bmp_calculate_size(bmp);
    file_header.bfOffBits = 
    file_header.bfReserved1 = 0;
    file_header.bfReserved1 = 0;

    BITMAPINFOHEADER info_header = { 0 };
    info_header.biSize = 0x00000028;
    info_header.biWidth = bmp->width;
    info_header.biHeight = bmp->height;
    info_header.biPlanes = 1;
    info_header.biBitCount = 24;
    info_header.biCompression = 0;
    info_header.biSizeImage = 0;
    info_header.biXPelsPerMeter = 2835;
    info_header.biYPelsPerMeter = 2835;
    info_header.biClrUsed = 0;
    info_header.biClrImportant = 0;

     // keep track of the next place we should write in the data buffer
    uint8_t *data_out = data;

    memcpy(data_out, &file_header, sizeof(file_header)); // write X number of bytes
    data_out += sizeof(file_header); // and then move data_out forward X bytes

    memcpy(data_out, &info_header, sizeof(info_header));
    data_out += sizeof(info_header);

    /*for each row of pixel data, going from the bottom to the top {
        memcpy(data_out, &bmp.data[beginning of row], size of row of pixels in bytes));
        data_out += size of row of pixels in bytes;
    }*/
    for (int i = bmp->height -1; i >= 0; i--) {
        memcpy(data_out, bmp[i*(bmp->width)-1], sizeof(color_bgr_t)* bmp->width);

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
    
    // if we wanted to make the first pixel white
    // bmp.data[0].r = 255;
    // bmp.data[0].g = 255;
    // bmp.data[0].b = 255;

    // remember to later free the bmp.data when we are done using it
    free(bmp.data);

    return 0;
}

#include "bmp.h"

// calculate the number of bytes of memory needed to serialize the bitmap
// that is, to write a valid bmp file to memory
size_t bmp_calculate_size(bitmap_t *bmp) {
    size_t grbSize = bmp->width * bmp->height * sizeof(*bmp->data);
    size_t s = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + grbSize;
    //printf("calculated bmp size: %ld\n", s);
    return s;
}

// write the bmp file to memory at data, which must be at least
// bmp_calculate_size large.
void bmp_serialize(bitmap_t *bmp, uint8_t *data) {
    BITMAPFILEHEADER file_header = { 0 }; // start out as all zero values
    file_header.bfType = 0x4d42;
    file_header.bfSize = bmp_calculate_size(bmp);
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;
    file_header.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    BITMAPINFOHEADER info_header = { 0 };
    info_header.biSize = sizeof(BITMAPINFOHEADER);
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
    //printf("size of file_header: %ld\n", sizeof(file_header));
    memcpy(data_out, &info_header, sizeof(info_header));
    data_out += sizeof(info_header);
    //printf("size of info_header: %ld\n", sizeof(info_header));
    /*for each row of pixel data, going from the bottom to the top {
        memcpy(data_out, &bmp.data[beginning of row], size of row of pixels in bytes));
        data_out += size of row of pixels in bytes;
    }*/
    for (int i = bmp->height - 1; i >= 0; i--) {
        memcpy(data_out, &bmp->data[i*(bmp->width)], sizeof(color_bgr_t)* bmp->width);
        data_out += (sizeof(color_bgr_t)* bmp->width);
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
    bmp_serialize(&bmp, serialized_bmp);
    for (int i = 0; i < 16; i++) {
    printf("%x ", serialized_bmp[i]); // %x is for hexadecimal
    }
    printf("\n");
    // serialized_bmp now has the full bmp formatted image

    // write to a file so we can check if it is a valid image
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);
    
    // if we wanted to make the first pixel white
    // bmp.data[0].r = 255;
    // bmp.data[0].g = 255;
    // bmp.data[0].b = 255;

    // remember to later free the bmp.data when we are done using it
    free(bmp.data);
    free(serialized_bmp);

    return 0;
}

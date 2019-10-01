


#include <unistd.h>
#include "image_server.h"

int main(void) {
    size_t image_size = // calculate the size of the image
    uint8_t *image_data = malloc(image_size);
    // make image_data a valid bmp image file

    image_server_set_data(image_size, image_data);
    image_server_start("8000"); // you can change the port number
    sleep(1);
    return 0;
}


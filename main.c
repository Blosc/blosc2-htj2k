#include <stdint.h>
#include <stdio.h>
#include "include/wrapper.h"


int main(void) {
    image_t image;
    if (htj2k_read_image(&image, "input/teapot.ppm")) {
        printf("Read error.\n");
        return -1;
    }
    printf("Read ok.\n");

    if (htj2k_encoder(&image)) {
        printf("Encode error.\n");
        return -1;
    }
    printf("Encode ok.\n");

    if (htj2k_decoder()) {
        printf("Decode error.\n");
        return -1;
    }
    printf("Decode ok.\n");

    return 0;
}

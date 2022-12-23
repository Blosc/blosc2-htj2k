#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/blosc2_htj2k.h"


int main(void) {
    int ret = 0;
    image_t image;
    uint8_t *output = NULL;

    // Read source file(s)
    printf("Read\t");
    if (htj2k_read_image(&image, "input/teapot.ppm")) {
        goto error;
    }
    printf("OK\n");

    // Encode
    printf("Encode\t");
    output = malloc(image.buffer_len);
    int output_len = htj2k_encoder(image.buffer, image.buffer_len, output, image.buffer_len, &image);
    if (output_len == 0) {
        printf("NOOP\n");
        goto exit;
    }
    else if (output_len < 0) {
        goto error;
    }
    printf("OK\n");

    // Free image's allocated memory
    htj2k_free_image(&image);

    // Decode
    printf("Decode\t");
    if (htj2k_decoder(output, output_len)) {
        goto error;
    }
    printf("OK\n");
    goto exit;

error:
    printf("ERROR\n");
    ret = -1;

exit:
    free(output);
    return ret;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "blosc2_htj2k.h"


int main(void) {
    const char *ifname = "input/teapot.ppm";
    image_t image;
    uint8_t *tmp = NULL;
    int tmp_len = 0;
    uint8_t *out = NULL;
    int out_len = 0;
    int ret = 0;

    // Read source file(s)
    printf("Read\t");
    if (htj2k_read_image(&image, ifname)) {
        goto error;
    }
    printf("OK\n");

    // Encode
    printf("Encode\t");
    tmp = malloc(image.buffer_len);
    tmp_len = htj2k_encoder(image.buffer, image.buffer_len, tmp, image.buffer_len, &image);
    if (tmp_len == 0) {
        printf("NOOP\n");
        goto exit;
    }
    else if (tmp_len < 0) {
        goto error;
    }
    printf("OK\n");
    htj2k_free_image(&image); // Free image's allocated memory

    // Decode
    printf("Decode\t");
    out = malloc(image.buffer_len);
    out_len = htj2k_decoder(tmp, tmp_len, out, image.buffer_len, &image);
    if (out_len == 0) {
        printf("Not enough space\n");
        goto error;
    }
    else if (out_len < 0) {
        goto error;
    }
    printf("OK\n");

    // Write output file
    printf("Write\t");
    htj2k_write_ppm(out, out_len, &image, "output/teapot.ppm");
    printf("OK\n");

    htj2k_free_image(&image); // Free image's allocated memory
    goto exit;

error:
    printf("ERROR\n");
    ret = -1;

exit:
    free(tmp);
    free(out);
    return ret;
}

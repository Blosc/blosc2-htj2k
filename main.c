#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <b2nd.h>
#include <blosc/context.h>
#include "blosc2_htj2k.h"


int main(void) {
    blosc2_init();

    const char *ifname = "input/teapot.ppm";
    char *ofname = "output/teapot.ppm";

    //const char *ifname = "input/pexels-artem-saranin-1496373.ppm";
    //char *ofname = "output/pexels-artem-saranin-1496373.ppm";

    image_t image;

    // Read source file(s)
    printf("Read\t");
    if (htj2k_read_image(&image, ifname)) {
        return -1;
    }
    printf("OK\n");

    // Register codec
    printf("Register\t");
    blosc2_codec udcodec;
    udcodec.compcode = 244;
    udcodec.compver = 1;
    udcodec.complib = 1;
    udcodec.compname = "htj2k";
    udcodec.encoder = htj2k_encoder;
    udcodec.decoder = htj2k_decoder;
    int error = blosc2_register_codec(&udcodec);
    printf("OK\n");

    printf("Blosc version info: %s (%s)\n", blosc2_get_version_string(), BLOSC2_VERSION_DATE);

    int8_t ndim = 3;
    int64_t shape[] = {3, image.width, image.height};
    int32_t chunkshape[] = {3, image.width, image.height};
    int32_t blockshape[] = {3, 400, 600};
    uint8_t itemsize = 4;

    blosc2_cparams cparams = BLOSC2_CPARAMS_DEFAULTS;
    cparams.compcode = 244;
    cparams.typesize = itemsize;
    for (int i = 0; i < BLOSC2_MAX_FILTERS; i++) {
        cparams.filters[i] = 0;
    }

    blosc2_dparams dparams = BLOSC2_DPARAMS_DEFAULTS;
    blosc2_storage b2_storage = {.cparams=&cparams, .dparams=&dparams};

    b2nd_context_t *ctx = b2nd_create_ctx(&b2_storage, ndim, shape, chunkshape, blockshape, NULL, 0);

    b2nd_array_t *arr;
    BLOSC_ERROR(b2nd_from_cbuffer(ctx, &arr, image.buffer, image.buffer_len));
    b2nd_save(arr, "output/b2nd.b2nd");

    uint8_t *buffer;
    uint64_t buffer_size = itemsize;
    for (int i = 0; i < arr->ndim; ++i) {
        buffer_size *= arr->shape[i];
    }
    buffer = malloc(buffer_size);

    BLOSC_ERROR(b2nd_to_cbuffer(arr, buffer, buffer_size));

    // Write output file
    printf("Write\t");
    htj2k_write_ppm(buffer, buffer_size, &image, ofname);
    printf("OK\n");

    blosc2_destroy();
    return 0;
}

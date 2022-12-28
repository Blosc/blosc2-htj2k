#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <caterva.h>
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

    caterva_config_t cfg = CATERVA_CONFIG_DEFAULTS;
    cfg.compcodec = 244;
    for (int i = 0; i < BLOSC2_MAX_FILTERS; i++) {
        cfg.filters[i] = 0;
    }

    caterva_ctx_t *ctx;
    caterva_ctx_new(&cfg, &ctx);

    caterva_params_t params = {0};
    params.ndim = ndim;
    params.itemsize = itemsize;
    for (int i = 0; i < ndim; ++i) {
        params.shape[i] = shape[i];
    }

    caterva_storage_t storage = {0};
    for (int i = 0; i < ndim; ++i) {
        storage.chunkshape[i] = chunkshape[i];
        storage.blockshape[i] = blockshape[i];
    }

    caterva_array_t *arr;
    CATERVA_ERROR(caterva_from_buffer(ctx, image.buffer, image.buffer_len, &params, &storage, &arr));
    caterva_save(ctx, arr, "output/caterva.cat");

    uint8_t *buffer;
    uint64_t buffer_size = itemsize;
    for (int i = 0; i < arr->ndim; ++i) {
        buffer_size *= arr->shape[i];
    }
    buffer = malloc(buffer_size);

    CATERVA_ERROR(caterva_to_buffer(ctx, arr, buffer, buffer_size));

    // Write output file
    printf("Write\t");
    htj2k_write_ppm(buffer, buffer_size, &image, ofname);
    printf("OK\n");

    blosc2_destroy();
    return 0;
}

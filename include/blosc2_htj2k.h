#ifndef WRAPPER_H
#define WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t ssiz; // component's bit depth and sign
} component_t;

typedef struct {
    uint8_t *buffer;
    size_t buffer_len;
    uint16_t num_components;
    uint32_t width;
    uint32_t height;
    uint8_t max_bpp;
    component_t components[3];
} image_t;

int htj2k_read_image(image_t *image, const char *filename);
void htj2k_free_image(image_t *image);
int htj2k_encoder(
    const uint8_t *input,
    int32_t input_len,
    uint8_t *output,
    int32_t output_len,
    image_t *image
);
int htj2k_decoder(const uint8_t *input, int32_t input_len);

#ifdef __cplusplus
}
#endif

#endif

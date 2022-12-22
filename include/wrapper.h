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
    int32_t *buffer[3];
    uint16_t num_components;
    uint32_t width;
    uint32_t height;
    uint8_t max_bpp;
    component_t components[3];
} image_t;

int htj2k_read_image(image_t *image, const char *filename);
int htj2k_encoder(image_t *image);
int htj2k_decoder(void);

#ifdef __cplusplus
}
#endif

#endif

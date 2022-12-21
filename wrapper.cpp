/*
 * typedef int (*blosc2_filter_forward_cb)(const uint8_t*, uint8_t*, int32_t, uint8_t, blosc2_cparams*, uint8_t)
 * typedef int (*blosc2_filter_backward_cb)(const uint8_t*, uint8_t*, int32_t, uint8_t, blosc2_dparams*, uint8_t)
 */

#include <cstring>
#include <encoder.hpp>
#include <decoder.hpp>
#include "include/dec_utils.hpp"
#include "include/wrapper.h"


#define NO_QFACTOR 0xFF

#define IFNAME "input/teapot.ppm"
//#define JFNAME "output/teapot.jphc"
#define JFNAME "output/teapot.j2c"
#define OFNAME "output/teapot.ppm"

int htj2k_encode(void) {
    // Input variables
    const char *ofname = JFNAME;
    std::vector<std::string> ifnames = {IFNAME};
    uint8_t qfactor = NO_QFACTOR; // 255
    bool isJPH = false;
    uint8_t color_space = 0;
    uint32_t num_threads = 1;
    int32_t num_iterations = 1;     // Number of iterations (1-INT32_MAX)

    // Input buffer
    std::vector<int32_t *> input_buf;
    open_htj2k::image img(ifnames);  // input image
    uint16_t num_components = img.get_num_components();
    for (uint16_t c = 0; c < num_components; ++c) {
        input_buf.push_back(img.get_buf(c));
    }

    // Information of input image
    uint32_t img_width = img.get_width(), img_height = img.get_height();
    uint32_t img_orig_x = 0, img_orig_y = 0;
    open_htj2k::siz_params siz;
    siz.Rsiz   = 0;
    siz.Xsiz   = img_width + img_orig_x;
    siz.Ysiz   = img_height + img_orig_y;
    siz.XOsiz  = img_orig_x;
    siz.YOsiz  = img_orig_y;
    siz.XTsiz  = img_width;         // Tiles size (X) default to image size
    siz.YTsiz  = img_height;        // Tiles size (Y) default to image size
    siz.XTOsiz = 0;                 // Origin of first tile (X)
    siz.YTOsiz = 0;                 // Origin of first tile (Y)
    siz.Csiz   = num_components;
    for (uint16_t c = 0; c < siz.Csiz; ++c) {
        siz.Ssiz.push_back(img.get_Ssiz_value(c));
        auto compw = img.get_component_width(c);
        auto comph = img.get_component_height(c);
        siz.XRsiz.push_back(static_cast<unsigned char>(((siz.Xsiz - siz.XOsiz) + compw - 1) / compw));
        siz.YRsiz.push_back(static_cast<unsigned char>(((siz.Ysiz - siz.YOsiz) + comph - 1) / comph));
    }

    // Parameters related to COD marker
    uint32_t cblk_width = 4, cblk_height = 4; // Must be power of 2 and >= 4
    open_htj2k::cod_params cod;
    cod.blkwidth                      = static_cast<uint16_t>(cblk_width);
    cod.blkheight                     = static_cast<uint16_t>(cblk_height);
    cod.is_max_precincts              = true;   // If false then precincts size must be defined
    cod.use_SOP                       = false;  // Use SOP (Start Of Packet) marker
    cod.use_EPH                       = false;  // Use EPH (End of Packet Header) marker
    cod.progression_order             = 0;      // 0:LRCP 1:RLCP 2:RPCL 3:PCRL 4:CPRL
    cod.number_of_layers              = 1;
    cod.use_color_trafo               = 1;      // Use RGB->YCbCr color space conversion (1 or 0)
    cod.dwt_levels                    = 5;      // Number of DWT decomposition (0-32)
    cod.codeblock_style               = 0x040;
    cod.transformation                = 0;      // 0:lossy 1:lossless

    // Parameters related to QCD marker
    open_htj2k::qcd_params qcd{};
    qcd.is_derived          = false;
    qcd.number_of_guardbits = 1;        // Number of guard bits (0-8)
    qcd.base_step           = 0.0;      // Base step size for quantization (0.0 - 2.0)
    if (qcd.base_step == 0.0) {
        qcd.base_step = 1.0f / static_cast<float>(1 << img.get_max_bpp());
    }

    // Create encoder
    for (int i = 0; i < num_iterations; ++i) {
        open_htj2k::openhtj2k_encoder encoder(
            ofname,                 // output filename
            input_buf,
            siz,                    // information of input image
            cod,                    // parameters related to COD marker
            qcd,                    // parameters related to QCD marker
            qfactor,                // quality factor (0-100 or 255)
            isJPH,
            color_space,            // 0: RGB or 1: YCC
            num_threads             // num_threads
        );

        //encoder.set_output_buffer(outbuf);
        try {
            encoder.invoke(); // Returns size_t total_size
        } catch (std::exception &exc) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int htj2k_decode(void) {
    // Input variables
    const char *ifname = JFNAME;
    const char *ofname = OFNAME;
    uint8_t reduce_NL = 0;          // Number of DWT resolution reduction (0-32)
    uint32_t num_threads = 1;
    int32_t num_iterations = 1;     // Number of iterations (1-INT32_MAX)

    std::vector<int32_t *> buf;
    std::vector<uint32_t> img_width;
    std::vector<uint32_t> img_height;
    std::vector<uint8_t> img_depth;
    std::vector<bool> img_signed;
    for (int i = 0; i < num_iterations; ++i) {
        // Create decoder
        open_htj2k::openhtj2k_decoder decoder(ifname, reduce_NL, num_threads);
        for (auto &j : buf) {
            delete[] j;
        }
        buf.clear();
        img_width.clear();
        img_height.clear();
        img_depth.clear();
        img_signed.clear();

        // Invoke decoder
        try {
            decoder.invoke(buf, img_width, img_height, img_depth, img_signed);
        } catch (std::exception &exc) {
            return EXIT_FAILURE;
        }
    }

    const char *ofname_ext = strrchr(ofname, '.');
    write_ppm(ofname, ofname_ext, buf, img_width, img_height, img_depth, img_signed);

    return EXIT_SUCCESS;
}

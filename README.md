
# Images

- https://www.pexels.com/photo/photo-of-a-pathway-in-a-forest-1496373/

# Requisites

Install the imagemagick utiltities to convert from/to PPM:

    brew install imagemagick  # for Mac OSX

(use the `convert` utility to convert to/from PPM)

If desired, install the libtiff library (just for support reading TIFF):

    brew install libtiff  # for Mac OSX

# Build

Build (including OpenHTJ2K):

    export BLOSC=/absolute/path/to/c-blosc2
    make build

Then a number of commands from OpenHTJ2K will be available:

    mkdir output
    ./bin/open_htj2k_enc -i input/teapot.ppm -o output/teapot.j2c
    ./bin/open_htj2k_enc -i input/teapot.ppm -o output/teapot.jphc
    ./bin/open_htj2k_dec -i output/teapot.jphc -o output/teapot.ppm

Using the OpenHTJ2K plugin is possible via the main program.  For Mac:

    faltet@Mac-mini-de-Francesc ~/b/blosc2-htj2k (main)> DYLD_LIBRARY_PATH=$BLOSC/build/blosc ./a.out                                 (base)
    Read	OK
    Register	OK
    Blosc version info: 2.6.2.dev ($Date:: 2022-12-15 #$)
    Write	OK

and the roundtrip file should appear in the output directory:

    faltet@Mac-mini-de-Francesc ~/b/blosc2-htj2k (main)> ls -l output/                                                                     (base)
    total 141216
    -rw-r--r--@ 1 faltet  staff  72000017 Jan 27 13:17 pexels-artem-saranin-1496373.ppm
    -rw-r--r--  1 faltet  staff     45605 Jan 27 12:21 teapot.j2c
    -rw-r--r--  1 faltet  staff     45690 Jan 27 12:24 teapot.jphc
    -rw-r--r--  1 faltet  staff    196623 Jan 27 13:14 teapot.ppm


# Fileformats

JPEG 2000 (J2K-1 block coder)
- Part 1 .JP2 file format
- Part 1 .J2K or .J2C or .JPC code stream
- Part 2 Extensions .JPX or .JPF file format
- Part 3 Motion .MJ2 file format
- Part 6 Compound .JPM file format
- Part 15 HT .JPH file format
* Part 15 HT .JHC or .JPHC code stream

We use .JPHC

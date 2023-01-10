
# Images

- https://www.pexels.com/photo/photo-of-a-pathway-in-a-forest-1496373/

# Commands

Build (including OpenHTJ2K):

    export BLOSC=/absolute/path/to/blosc2
    make build

Then a number of commands from OpenHTJ2K will be available:

    ./bin/open_htj2k_enc -i input/teapot.ppm -o output/teapot.j2c
    ./bin/open_htj2k_enc -i input/teapot.ppm -o output/teapot.jphc
    ./bin/open_htj2k_dec -i output/teapot.jphc -o output/teapot.ppm

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

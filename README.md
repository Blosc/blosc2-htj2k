
# Images

- https://www.pexels.com/photo/photo-of-a-pathway-in-a-forest-1496373/

# Commands

    encoder -i input/teapot.ppm -o output/teapot.j2c
    encoder -i input/teapot.ppm -o output/teapot.jphc
    decoder -i output/teapot.jphc -o output/teapot.ppm

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

# Patch (blosc2)

```
diff --git plugins/plugin_utils.h plugins/plugin_utils.h
index 15f54d1b..fbf8ab69 100644
--- plugins/plugin_utils.h
+++ plugins/plugin_utils.h
@@ -4,7 +4,7 @@
   License: BSD 3-Clause (see LICENSE.txt)
 */

-void swap_store(void *dest, const void *pa, int size);
+BLOSC_EXPORT void swap_store(void *dest, const void *pa, int size);

 int32_t deserialize_meta(uint8_t *smeta, int32_t smeta_len, int8_t *ndim, int64_t *shape,
                          int32_t *chunkshape, int32_t *blockshape);
```

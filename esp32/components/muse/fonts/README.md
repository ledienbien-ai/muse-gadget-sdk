# Vietnamese letters for the screen

LVGL's built-in fonts stop at ASCII. These files add the letters Vietnamese
writes with accents, so the screen can show Vietnamese text
(`muse_fonts.c` puts them to use):

| File | Font | Used for |
|---|---|---|
| `muse_font_montserrat_vi_14.c`, `_16.c`, `_20.c` | Montserrat Medium, 4 bpp | the accented letters only, behind LVGL's own Montserrat of the same size |
| `muse_font_mono_vi_20.c` | JetBrains Mono ExtraBold, 4 bpp, 12 px wide | the state word and the captions in Vietnamese: ASCII and the accented letters |
| `muse_font_mono_vi_16.c` | the same at 16 px, 10 px wide | the same on a screen too short for the bigger one (296×240, 320×240) |

Both fonts are under the SIL Open Font License 1.1 (`OFL-Montserrat.txt`,
`OFL-JetBrainsMono.txt`). The Apache License of the rest of this directory doesn't
cover them.

## Making them again

With [lv_font_conv](https://github.com/lvgl/lv_font_conv), `Montserrat-Medium.ttf`
from LVGL's `scripts/built_in_font/`, and JetBrains Mono's `latin` and
`vietnamese` files at weight 800 from the `@fontsource/jetbrains-mono` package:

```sh
VI="-r 0xC0-0xC3 -r 0xC8-0xCA -r 0xCC-0xCD -r 0xD2-0xD5 -r 0xD9-0xDA -r 0xDD \
    -r 0xE0-0xE3 -r 0xE8-0xEA -r 0xEC-0xED -r 0xF2-0xF5 -r 0xF9-0xFA -r 0xFD"
VI2="-r 0x102-0x103 -r 0x110-0x111 -r 0x128-0x129 -r 0x168-0x169 \
     -r 0x1A0-0x1A1 -r 0x1AF-0x1B0 -r 0x1EA0-0x1EF9"

for s in 14 16 20; do
  npx lv_font_conv --bpp 4 --size $s --no-compress --font Montserrat-Medium.ttf $VI $VI2 \
    --format lvgl --lv-include lvgl.h --lv-font-name muse_font_montserrat_vi_$s \
    -o muse_font_montserrat_vi_$s.c
done

for s in 16 20; do
  npx lv_font_conv --bpp 4 --size $s --no-compress \
    --font jetbrains-mono-latin-800-normal.woff -r 0x20-0x7E $VI \
    --font jetbrains-mono-vietnamese-800-normal.woff $VI2 \
    --format lvgl --lv-include lvgl.h --lv-font-name muse_font_mono_vi_$s -o muse_font_mono_vi_$s.c
done
```

Then put the notice at the top of each file back.

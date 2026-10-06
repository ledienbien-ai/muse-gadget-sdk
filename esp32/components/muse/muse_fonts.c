/*
 * Copyright (c) 2026 ledienbien-ai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "muse_fonts.h"

LV_FONT_DECLARE(muse_font_montserrat_vi_14)
LV_FONT_DECLARE(muse_font_montserrat_vi_16)
LV_FONT_DECLARE(muse_font_montserrat_vi_20)
LV_FONT_DECLARE(muse_font_mono_vi_16)
LV_FONT_DECLARE(muse_font_mono_vi_20)

/*
 * LVGL's Montserrat with the Vietnamese letters behind it: a copy of the font
 * that falls back to fonts/ for what it lacks. Both sit on one baseline; the
 * copy's lines are as tall as the taller of the two, so an accent on a capital
 * isn't cut off at the top of a label.
 */
static const struct {
    int px;
    const lv_font_t *plain;
    const lv_font_t *accents;
} FONTS[] = {
    { 14, &lv_font_montserrat_14, &muse_font_montserrat_vi_14 },
    { 16, &lv_font_montserrat_16, &muse_font_montserrat_vi_16 },
    { 20, &lv_font_montserrat_20, &muse_font_montserrat_vi_20 },
};
#define FONT_COUNT (sizeof(FONTS) / sizeof(FONTS[0]))

static lv_font_t s_with_accents[FONT_COUNT];
static bool s_made;
static bool s_vietnamese;
static bool s_short;

static void make(void)
{
    if (s_made) {
        return;
    }
    for (size_t i = 0; i < FONT_COUNT; i++) {
        lv_font_t *f = &s_with_accents[i];
        const lv_font_t *plain = FONTS[i].plain, *accents = FONTS[i].accents;
        *f = *plain;
        f->fallback = accents;
        int above = LV_MAX(plain->line_height - plain->base_line, accents->line_height - accents->base_line);
        f->base_line = LV_MAX(plain->base_line, accents->base_line);
        f->line_height = above + f->base_line;
    }
    s_made = true;
}

void muse_fonts_init(bool vietnamese, bool short_screen)
{
    make();
    s_vietnamese = vietnamese;
    s_short = short_screen;
}

const lv_font_t *muse_font_vi(int px)
{
    make();
    for (size_t i = 0; i < FONT_COUNT; i++) {
        if (FONTS[i].px == px) {
            return &s_with_accents[i];
        }
    }
    return &lv_font_montserrat_28;   /* digits and symbols only */
}

const lv_font_t *muse_font(int px)
{
    if (s_vietnamese) {
        return muse_font_vi(px);
    }
    for (size_t i = 0; i < FONT_COUNT; i++) {
        if (FONTS[i].px == px) {
            return FONTS[i].plain;
        }
    }
    return &lv_font_montserrat_28;
}

const lv_font_t *muse_font_pixel(void)
{
    if (!s_vietnamese) {
        return &lv_font_unscii_16;
    }
    return s_short ? &muse_font_mono_vi_16 : &muse_font_mono_vi_20;
}

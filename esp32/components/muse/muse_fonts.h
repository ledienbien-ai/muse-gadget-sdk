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

#pragma once

#include <stdbool.h>

#include "lvgl.h"

/*
 * The screen's fonts by language (muse_lang.h). In English they're LVGL's own
 * Montserrat and unscii, which stop at ASCII. In Vietnamese, Montserrat gets
 * the accented letters from fonts/ and taller lines to fit two accents on a
 * capital, and in place of the pixel font, which has no such letters, comes a
 * bold monospace one that does (JetBrains Mono): 12 px a letter to unscii_16's
 * 16, so a line holds a third more of them. Its lines are half as tall again
 * as unscii_16's, which is more than a screen 240 px tall can give up: there
 * it comes a size smaller, 10 px a letter.
 */

/* Picks the fonts; call before the first screen is built. short_screen: the
 * full layout on a rectangle too short for it (muse_ui.c). */
void muse_fonts_init(bool vietnamese, bool short_screen);

/* Montserrat at 14, 16, 20 or 28 px for the current language. */
const lv_font_t *muse_font(int px);

/* The same with the Vietnamese letters whatever the language, for the one
 * place that needs them in English: the language's own name. */
const lv_font_t *muse_font_vi(int px);

/* The face's font, a letter to a column: the state word, captions and page
 * titles. */
const lv_font_t *muse_font_pixel(void);

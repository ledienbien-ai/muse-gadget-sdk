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
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The screen's language. The code writes its text in English; muse_tr() gives
 * the same text in the language the screen is set to, or the English back if
 * it has none. The language is picked once, before the screen is built
 * (muse_app.c, from muse_settings_lang()), and changing it restarts the device.
 */

typedef enum {
    MUSE_LANG_EN,
    MUSE_LANG_VI,
    MUSE_LANG_COUNT,
} muse_lang_t;

/* Sets the screen's language. In Vietnamese, text keeps its Vietnamese
 * letters (muse_text.h); in English they're shown as plain ones. */
void muse_lang_set(muse_lang_t lang);
muse_lang_t muse_lang(void);

/* "en", "vi": for the setup commands and the text-to-speech request. */
const char *muse_lang_code(muse_lang_t lang);
/* The language from its code; false if it isn't one. */
bool muse_lang_from_code(const char *code, muse_lang_t *lang);
/* The language's name in the language itself: "English", "Tiếng Việt". */
const char *muse_lang_name(muse_lang_t lang);

/*
 * `en` in the screen's language. A format string keeps its conversions in the
 * same order, so it can be passed on to printf.
 */
const char *muse_tr(const char *en) __attribute__((format_arg(1)));

/* Entry i of the texts, for the host tests; false past the last one. */
bool muse_lang_text(size_t i, const char **en, const char **vi);

#ifdef __cplusplus
}
#endif

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

/* Drives the screen's language and the text of spoken replies for
 * test_muse_lang.py: muse_lang.c, muse_text.c, muse_chat_text.c and
 * muse_tts_text.c. The mode is argv[1], the language "en" or "vi" argv[2],
 * and the text comes on stdin.
 *   texts            every text muse_lang.c has, English then Vietnamese, each ending in a NUL
 *   tr               stdin in the language
 *   show             stdin as a caption shows it (muse_text_to_ascii)
 *   tail             the transcript's tail (muse_hatch_tail_words) into argv[3] bytes
 *   page COLS LINES  each page of stdin in turn, ending in a NUL
 *   clean            stdin as it's read out
 *   chunks           the pieces stdin is fetched in, each ending in a NUL
 *   url              the request for stdin spoken in the language
 *   vietnamese       1 if stdin is written in Vietnamese, else 0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "muse_chat_priv.h"
#include "muse_lang.h"
#include "muse_state.h"
#include "muse_text.h"
#include "muse_tts.h"

static int s_cols = 16, s_lines = 2;

void muse_state_page(int *cols, int *lines)
{
    *cols = s_cols;
    *lines = s_lines;
}

static char *read_all(void)
{
    size_t cap = 1 << 16, n = 0, got;
    char *buf = malloc(cap + 1);
    while (buf && (got = fread(buf + n, 1, cap - n, stdin)) > 0) {
        n += got;
        if (n == cap) {
            cap *= 2;
            buf = realloc(buf, cap + 1);
        }
    }
    if (!buf) {
        exit(2);
    }
    buf[n] = '\0';
    return buf;
}

int main(int argc, char **argv)
{
    const char *mode = argc > 1 ? argv[1] : "";
    muse_lang_t lang = MUSE_LANG_EN;
    if (argc > 2 && !muse_lang_from_code(argv[2], &lang)) {
        return 2;
    }
    muse_lang_set(lang);
    char *in = read_all();
    size_t cap = strlen(in) * 3 + 4096;
    char *out = malloc(cap);
    if (!out) {
        return 2;
    }

    if (!strcmp(mode, "texts")) {
        const char *en, *vi;
        for (size_t i = 0; muse_lang_text(i, &en, &vi); i++) {
            fwrite(en, 1, strlen(en) + 1, stdout);
            fwrite(vi, 1, strlen(vi) + 1, stdout);
        }
    } else if (!strcmp(mode, "tr")) {
        fputs(muse_tr(in), stdout);
    } else if (!strcmp(mode, "show")) {
        strcpy(out, in);
        muse_text_to_ascii(out, cap);
        fputs(out, stdout);
    } else if (!strcmp(mode, "tail") && argc > 3) {
        muse_hatch_tail_words(in, out, (size_t)atoi(argv[3]));
        fputs(out, stdout);
    } else if (!strcmp(mode, "page") && argc > 4) {
        s_cols = atoi(argv[3]);
        s_lines = atoi(argv[4]);
        char last[1024] = "";
        for (size_t at = 0; at < strlen(in); at++) {
            char page[400];   /* MUSE_CAPTION_MAX */
            if (muse_hatch_caption_at(in, at, page, sizeof(page)) && strcmp(page, last)) {
                fwrite(page, 1, strlen(page) + 1, stdout);
                strcpy(last, page);
            }
        }
    } else if (!strcmp(mode, "clean")) {
        muse_tts_clean(in, out, cap);
        fputs(out, stdout);
    } else if (!strcmp(mode, "chunks")) {
        muse_tts_clean(in, out, cap);
        const char *p = out;
        char chunk[MUSE_TTS_CHUNK_CHARS * 4 + 1];
        while (muse_tts_next_chunk(&p, chunk, sizeof(chunk))) {
            fwrite(chunk, 1, strlen(chunk) + 1, stdout);
        }
    } else if (!strcmp(mode, "url")) {
        char url[MUSE_TTS_URL_MAX];
        if (!muse_tts_url(in, muse_lang_code(lang), url, sizeof(url))) {
            return 3;
        }
        fputs(url, stdout);
    } else if (!strcmp(mode, "vietnamese")) {
        putchar(muse_tts_is_vietnamese(in) ? '1' : '0');
    } else {
        return 2;
    }
    return 0;
}

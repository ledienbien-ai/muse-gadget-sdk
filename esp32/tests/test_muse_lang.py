#!/usr/bin/env python3
# Copyright (c) 2026 ledienbien-ai
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""The screen in Vietnamese (muse_lang.c, muse_text.c, muse_chat_text.c) and the
text of spoken replies (muse_tts_text.c)."""

from __future__ import annotations

import os
import re
import shlex
import shutil
import subprocess
import tempfile
import unittest
import urllib.parse
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MUSE = ROOT / "components" / "muse"

CAPTION_MAX = 400     # muse_state.h
CHUNK_CHARS = 180     # muse_tts.h
ENCODED_MAX = 1400    # muse_tts.h

# Every letter Vietnamese writes with an accent, lower case and upper.
BASES = {
    "a": "àáảãạăằắẳẵặâầấẩẫậ", "e": "èéẻẽẹêềếểễệ", "i": "ìíỉĩị", "o": "òóỏõọôồốổỗộơờớởỡợ",
    "u": "ùúủũụưừứửữự", "y": "ỳýỷỹỵ", "d": "đ",
}
LOWER = "".join(BASES.values())
LETTERS = LOWER + LOWER.upper()

REPLY = (
    "Chào bạn! Hôm nay trời Hà Nội khá đẹp, nhiệt độ khoảng 27 độ, có nắng nhẹ và gió mát. "
    "Bạn có thể đi dạo quanh hồ Gươm, ghé phố cổ ăn một bát phở rồi uống cà phê trứng. "
    "Buổi chiều có thể có mưa rào, nên bạn nhớ mang theo ô nhé. Chúc bạn một ngày thật vui vẻ!"
)


def conversions(fmt: str) -> list[str]:
    """The conversions of a printf format, in order ("%%" isn't one)."""
    return [m for m in re.findall(r"%(?:[-+#0]*\d*(?:\.\d+)?[a-zA-Z]|%)", fmt) if m != "%%"]


class LangTest(unittest.TestCase):
    binary: Path

    @classmethod
    def setUpClass(cls) -> None:
        cc = shlex.split(os.environ.get("CC", "cc"))
        if not cc or shutil.which(cc[0]) is None:
            raise unittest.SkipTest("C compiler not available")
        cls.tmp = tempfile.TemporaryDirectory()
        cls.binary = Path(cls.tmp.name) / "muse_lang_harness"
        proc = subprocess.run(
            [
                *cc,
                "-include",
                str(ROOT / "tests" / "host_compat.h"),
                "-std=gnu11",
                "-Wall",
                "-Wextra",
                "-Werror",
                "-I",
                str(MUSE),
                str(ROOT / "tests" / "muse_lang_harness.c"),
                str(MUSE / "muse_lang.c"),
                str(MUSE / "muse_text.c"),
                str(MUSE / "muse_chat_text.c"),
                str(MUSE / "muse_tts_text.c"),
                "-o",
                str(cls.binary),
            ],
            cwd=ROOT,
            text=True,
            capture_output=True,
        )
        if proc.returncode:
            raise AssertionError(proc.stdout + proc.stderr)

    @classmethod
    def tearDownClass(cls) -> None:
        cls.tmp.cleanup()

    def run_harness(self, *args: str, text: str = "") -> str:
        proc = subprocess.run([str(self.binary), *args], input=text.encode(), capture_output=True)
        self.assertEqual(proc.returncode, 0, msg=proc.stderr.decode())
        return proc.stdout.decode()

    def texts(self) -> list[tuple[str, str]]:
        parts = self.run_harness("texts").split("\0")[:-1]
        self.assertEqual(len(parts) % 2, 0)
        return list(zip(parts[::2], parts[1::2]))

    # ---- The screen's texts ----

    def test_every_text_has_one_translation(self) -> None:
        texts = self.texts()
        self.assertGreater(len(texts), 150)
        seen = set()
        for en, vi in texts:
            self.assertTrue(en and vi, msg=en)
            self.assertNotIn(en, seen, msg="twice: " + en)
            seen.add(en)

    def test_formats_keep_their_conversions(self) -> None:
        for en, vi in self.texts():
            with self.subTest(en=en):
                self.assertEqual(conversions(en), conversions(vi))
                self.assertEqual(en.count("\n"), vi.count("\n"))
                # Spaces at either end glue the text to what's next to it.
                self.assertEqual(en[0] == " ", vi[0] == " ")
                self.assertEqual(en[-1] == " ", vi[-1] == " ")

    def test_face_texts_stay_in_capitals(self) -> None:
        # The face's words are written in capitals, and so are theirs. The
        # status line's are the exception: Vietnamese sets it in another font.
        status = {"USB POWER", "CHARGING %d%%", "BATTERY %d%%"}
        for en, vi in self.texts():
            plain = re.sub(r"%[a-z.0-9]+", "", en)
            if plain.upper() == plain and re.search(r"[A-Z]{2}", plain) and en not in status:
                with self.subTest(en=en):
                    self.assertEqual(re.sub(r"%[a-z.0-9]+", "", vi).upper(), re.sub(r"%[a-z.0-9]+", "", vi))

    def test_vietnamese_uses_only_letters_the_fonts_have(self) -> None:
        allowed = set(LETTERS) | {chr(c) for c in range(0x20, 0x7F)} | {"\n"}
        for en, vi in self.texts():
            with self.subTest(en=en):
                self.assertFalse(set(vi) - allowed, msg=vi)

    def test_tr_follows_the_language(self) -> None:
        self.assertEqual(self.run_harness("tr", "vi", text="READY"), "SẴN SÀNG")
        self.assertEqual(self.run_harness("tr", "en", text="READY"), "READY")
        self.assertEqual(self.run_harness("tr", "vi", text="Joining %s\n%s"), "Đang vào %s\n%s")
        # No translation: the text as it came, such as a network's name.
        self.assertEqual(self.run_harness("tr", "vi", text="MyHomeWiFi"), "MyHomeWiFi")
        self.assertEqual(self.run_harness("tr", "vi", text=""), "")

    # ---- Vietnamese letters on the screen ----

    def test_vietnamese_letters_are_kept_in_vietnamese(self) -> None:
        text = "Tiếng Việt: " + LETTERS
        self.assertEqual(self.run_harness("show", "vi", text=text), text)

    def test_vietnamese_letters_become_plain_in_english(self) -> None:
        for base, accented in BASES.items():
            with self.subTest(base=base):
                self.assertEqual(self.run_harness("show", "en", text=accented), base * len(accented))
                self.assertEqual(self.run_harness("show", "en", text=accented.upper()), base.upper() * len(accented))
        self.assertEqual(self.run_harness("show", "en", text="Xin chào, tôi là trợ lý của bạn."),
                         "Xin chao, toi la tro ly cua ban.")

    def test_other_stand_ins_stay_in_both_languages(self) -> None:
        for lang in ("en", "vi"):
            with self.subTest(lang=lang):
                self.assertEqual(self.run_harness("show", lang, text="“Phở” — 5€… ñ ü \U0001F35C"),
                                 "\"Ph%s\" -- 5EUR... n u " % ("ở" if lang == "vi" else "o"))

    def test_transcript_tail_counts_characters(self) -> None:
        text = "hôm nay trời đẹp quá mình đi dạo quanh hồ một vòng rồi về nhà nấu cơm nhé"
        tail = self.run_harness("tail", "vi", "96", text=text)
        self.assertTrue(text.endswith(tail))
        self.assertLessEqual(len(tail), 32)
        self.assertGreater(len(tail), 20)
        self.assertIn(text[-len(tail) - 1], " ")   # it starts on a word
        # Cut short for the room, it still ends on a whole character.
        for cap in range(2, 40):
            cut = self.run_harness("tail", "vi", str(cap), text="ệ" * 40)
            self.assertEqual(cut, "ệ" * min((cap - 1) // 3, 32))
        self.assertEqual(self.run_harness("tail", "en", "96", text="a" * 50), "a" * 32)

    def test_reply_pages_wrap_by_character(self) -> None:
        # The pages the three boards' Vietnamese layouts ask for: columns, lines.
        for cols, lines in ((18, 3), (21, 2), (22, 4), (26, 5), (32, 6)):
            with self.subTest(cols=cols, lines=lines):
                pages = self.run_harness("page", "vi", str(cols), str(lines), text=REPLY).split("\0")[:-1]
                self.assertGreater(len(pages), 1)
                words: list[str] = []
                for i, page in enumerate(pages):
                    self.assertLess(len(page.encode()), CAPTION_MAX)
                    rows = page.split("\n")
                    self.assertLessEqual(len(rows), lines)
                    for row in rows:
                        self.assertLessEqual(len(row), cols, msg=row)
                    # A page's last line starts the next one.
                    words += " ".join(rows if i == 0 else rows[1:]).split()
                self.assertEqual(words, REPLY.split())

    # ---- Spoken replies ----

    def test_clean_drops_what_shouldnt_be_read_out(self) -> None:
        clean = lambda s: self.run_harness("clean", text=s)  # noqa: E731
        self.assertEqual(clean("**Xin chào!**  Đây là\n\n# Tiêu đề\n- `mã` ~~cũ~~"), "Xin chào! Đây là Tiêu đề - mã cũ")
        self.assertEqual(clean("Xem [trang này](https://example.com/a?b=1) nhé \U0001F600\U0001F44D"), "Xem trang này nhé")
        self.assertEqual(clean("“Trích dẫn” — hết… 5€"), "\"Trích dẫn\" -- hết... 5EUR")
        self.assertEqual(clean("  \n\t "), "")
        self.assertEqual(clean("\U0001F600"), "")
        self.assertEqual(clean(LETTERS), LETTERS)
        # The screen's language doesn't change what's said.
        self.assertEqual(self.run_harness("clean", "vi", text="Phở “ngon”"), "Phở \"ngon\"")
        self.assertEqual(self.run_harness("clean", "en", text="Phở “ngon”"), "Phở \"ngon\"")

    def test_chunks_fit_a_request_and_lose_nothing(self) -> None:
        for text in (REPLY, REPLY * 3, "ệ" * 700, "word " * 300, "x" * 500, "Ngắn thôi.", "Một. Hai! Ba? " * 40):
            with self.subTest(text=text[:20], length=len(text)):
                chunks = self.run_harness("chunks", text=text).split("\0")[:-1]
                clean = self.run_harness("clean", text=text)
                self.assertEqual("".join(chunks).replace(" ", ""), clean.replace(" ", ""))
                for chunk in chunks:
                    self.assertTrue(chunk and chunk == chunk.strip())
                    self.assertLessEqual(len(chunk), CHUNK_CHARS)
                    self.assertLessEqual(len(urllib.parse.quote(chunk, safe="-_.~")), ENCODED_MAX)

    def test_chunks_end_at_sentences_when_they_can(self) -> None:
        chunks = self.run_harness("chunks", text=REPLY).split("\0")[:-1]
        self.assertGreater(len(chunks), 1)
        for chunk in chunks:
            self.assertIn(chunk[-1], ".!?")
        # No sentence end in reach: at a word.
        chunks = self.run_harness("chunks", text="từ " * 200).split("\0")[:-1]
        self.assertTrue(all(set(c.split()) == {"từ"} for c in chunks))
        self.assertEqual(self.run_harness("chunks", text=" \n "), "")

    def test_url_carries_the_text_and_language(self) -> None:
        for lang, text in (("vi", "Xin chào, bạn khỏe không? 100% & a+b=c #1"), ("en", "Hello there; it's 5 o'clock/3 PM?")):
            with self.subTest(lang=lang):
                url = self.run_harness("url", lang, text=text)
                parts = urllib.parse.urlsplit(url)
                self.assertEqual((parts.scheme, parts.netloc, parts.path), ("https", "translate.google.com", "/translate_tts"))
                self.assertRegex(parts.query, r"^[A-Za-z0-9%&=._~-]+$")
                query = urllib.parse.parse_qs(parts.query, strict_parsing=True)
                self.assertEqual(query, {"ie": ["UTF-8"], "client": ["tw-ob"], "tl": [lang], "q": [text]})

    def test_vietnamese_is_told_from_english(self) -> None:
        for text, vietnamese in (
            (REPLY, True),
            ("Xin chào", True),
            ("Đường", True),
            ("MƯA", True),
            ("Hello! It's a lovely day, isn't it?", False),
            ("Xin chao ban", False),     # typed without accents: nothing to tell it by
            ("Costs 5€ — “cheap”… \U0001F600", False),
            ("naïve façade", False),
            ("", False),
        ):
            with self.subTest(text=text[:20]):
                self.assertEqual(self.run_harness("vietnamese", text=text), "1" if vietnamese else "0")


if __name__ == "__main__":
    unittest.main()

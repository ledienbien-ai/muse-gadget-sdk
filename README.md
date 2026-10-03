<!--
Copyright (c) Meta Platforms, Inc. and affiliates.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
-->
<!-- Modified by ledienbien-ai (2026): describes this fork and the ESP32-S3 boards it adds. -->

# Muse Gadgets for the ESP32-S3 Device

**English** | [Tiếng Việt](README.vi.md) | [简体中文](README.zh-CN.md) | [日本語](README.ja.md) | [한국어](README.ko.md)

> An unofficial community fork of
> [facebookincubator/muse-gadget-sdk](https://github.com/facebookincubator/muse-gadget-sdk).
> It is not affiliated with or endorsed by Meta, Waveshare or any other board
> maker.

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/images/muse-gadgets-dark.png">
    <img src=".github/images/muse-gadgets-light.png" width="900" alt="Muse gadgets: a Waveshare round AMOLED, an M5Stack StickS3, Muse Home Link, a Raspberry Pi and a Seeed reTerminal e-ink display">
  </picture>
</p>

Muse gadgets are open source devices you build yourself: program an
off-the-shelf ESP32 board or set up a Raspberry Pi, then connect Muse to your
displays, buttons, sensors and actuators. This fork adds ESP32-S3 boards that
the upstream ESP32 Device SDK doesn't support yet, and leaves the rest of the
SDK as it is upstream.

Built by hackers, for hackers, just for fun. Flashing custom firmware can brick
boards and void warranties. Proceed at your own risk!

## Boards in this fork

Each board runs the full on-screen UI: the animated avatar, push-to-talk,
settings by touch, and images from Muse.

| Board | Screen | Name | Profile | Status |
|---|---|---|---|---|
| [Waveshare ESP32-S3-Touch-LCD-1.85C](#waveshare-esp32-s3-touch-lcd-185c) | 1.85" round 360×360, touch | `s3lcd` | `waveshare-s3-185c` | Flashed on a board: it boots and shows the UI. Talking to Muse hasn't been tried |
| [OSTB-3ST](#ostb-3st) | 1.83" 296×240, touch | `ostb` | `ostb-3st` | Builds; not yet run on the board |

The name is what `tools/muse/board.sh` calls the board; the profile names its
build settings and build directory. The boards upstream supports are still
here, listed in [`esp32/devices/README.md`](esp32/devices/README.md).

## Waveshare ESP32-S3-Touch-LCD-1.85C

<p align="center">
  <img src="doc/image/Wareshare%20Touch%20LCD%201.85C.png" width="720" alt="The Muse UI at 360 px round, drawn by the simulator: ready, pairing, listening, thinking, error and speaking">
</p>

| Part | Details |
|---|---|
| Chip | ESP32-S3R8, 16 MB flash, 8 MB octal PSRAM, native USB |
| Display | 1.85" round 360×360 ST77916 LCD on QSPI, PWM backlight |
| Touch | CST816 |
| Audio, V1 board | PCM5101 DAC and a digital I2S microphone |
| Audio, V2 board ("Rev2.0") | ES8311 DAC and ES7210 ADC with two microphones |
| Button | BOOT: hold to talk |
| Battery | Voltage through a divider on GPIO8 |

One firmware serves both audio versions: it checks for the ES8311 at boot.

**Status:** builds with ESP-IDF v6.0.1 and has been flashed on a board, where
it boots and shows the UI. Talking to Muse hasn't been tried on it yet, and the
V2 audio path is written from Waveshare's example code without a V2 board to
try it on.

Known limits: BOOT is the only button, so the screen sleeps on its timer and
powering off is on the Power page in Settings (the board deep-sleeps until BOOT
is pressed; the slide switch cuts power). The charger's status reaches no pin,
so the board shows "charging" only while a computer is attached.

Code: [`board_waveshare_s3_185c.c`](esp32/components/muse/boards/board_waveshare_s3_185c.c),
build settings: [`sdkconfig.muse-waveshare-s3-185c`](esp32/devices/sdkconfig.muse-waveshare-s3-185c).
Hardware reference: [Waveshare's documentation](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C).

## OSTB-3ST

<p align="center">
  <img src="doc/image/ostb-3st.png" width="720" alt="The Muse UI at 296×240, drawn by the simulator: ready, pairing, listening, thinking, error and speaking">
</p>
<p align="center">
  <img src="doc/image/ostb-3st-settings.png" width="720" alt="The settings pages at 296×240, drawn by the simulator">
</p>

Ported from the source of the board's xiaozhi-esp32 firmware
(`ostb-xiaozhi-3st`). The UI is laid out for its 296×240 landscape screen.

| Part | Details |
|---|---|
| Chip | ESP32-S3, 16 MB flash, 8 MB octal PSRAM, native USB |
| Display | 1.83" 240×296 NV3023 LCD on SPI, used in landscape, PWM backlight |
| Touch | CST816, read by polling: it has no interrupt line |
| Audio | ES8311 DAC and ES7210 ADC |
| Keys | Upper (volume up): hold to talk. Lower (volume down): press to sleep the screen, hold to power off |
| Battery | Level from the firmware's ADC table, and the charger's status pin |

**Status:** builds with ESP-IDF v6.0.1. It has not been run on the board. The
pins, the panel's setup and its orientation come from that firmware's source,
with no documentation to check them against. If the picture comes out rotated
or mirrored, or touches land in the wrong place, change `LCD_MADCTL` or
`TP_SWAP_XY`, `TP_MIRROR_X` and `TP_MIRROR_Y` at the top of the board's code.

Known limits: the 4G modem and the LED aren't used. The battery shows a level
but no voltage. Powering off drives the board's power-off pin; on USB power the
board may stay up, with the screen off until the upper key is pressed. Once
the battery is full, the board knows it's on USB power only while a computer
is attached.

Code: [`board_ostb_3st.c`](esp32/components/muse/boards/board_ostb_3st.c),
build settings: [`sdkconfig.muse-ostb-3st`](esp32/devices/sdkconfig.muse-ostb-3st).

## Build and flash

The steps are the same for every board. Take its name and profile from the
[table above](#boards-in-this-fork).

1. Get an [SDK token](https://gadgets.muse.ai/settings/sdk-tokens) and read the
   [Gadget SDK Terms](https://gadgets.muse.ai/sdk-terms). Every gadget needs a
   token to pair.
2. Install ESP-IDF **v6.0.1** (see [`esp32/README.md`](esp32/README.md)).
3. Build from `esp32/`. On macOS or Linux, with the board's name:

   ```sh
   tools/muse/board.sh build ostb
   ```

   On Windows, in an ESP-IDF PowerShell, with the board's profile:

   ```powershell
   $P = "ostb-3st"
   idf.py -B build-muse-$P -DIDF_TARGET=esp32s3 `
     "-DSDKCONFIG=build-muse-$P/sdkconfig" `
     "-DSDKCONFIG_DEFAULTS=sdkconfig.defaults;devices/sdkconfig.muse;devices/sdkconfig.muse-$P" build
   ```

   Set the token with `idf.py -B build-muse-$P menuconfig`
   (ESP32 Device SDK > Muse Gadgets SDK token), then build again.
4. Flash over USB-C, replacing `PORT` with your board's port:

   ```powershell
   idf.py -B build-muse-$P -p PORT flash
   ```

   If it can't connect, put the board in download mode and try again: hold
   **BOOT**, tap **RESET**, release **BOOT**. On a board without a RESET
   button, hold BOOT while you plug the cable in.

   For a web flasher, make one file and write it at address `0x0`:

   ```powershell
   idf.py -B build-muse-$P merge-bin -o muse-gadget-$P-merged.bin
   ```

   A build made with your SDK token carries that token, so think before you
   publish the file.
5. In the Muse app, turn on **Settings > Devices > Developer mode**, add the
   device named `MuseGadget-XXXXXX`, and press the board's talk button when
   asked (BOOT on the 1.85C, the upper key on the OSTB-3ST).

## Adding another device

A board with a screen, a speaker and a microphone needs one driver file and a
few lines of registration. This is what each board in this fork consists of:

1. `esp32/components/muse/boards/board_<id>.c` fills in `muse_board_t`
   (`muse_board.h`): display, touch, audio, buttons, battery and power. Start
   from the board closest to yours.
2. `esp32/devices/sdkconfig.muse-<profile>` holds its build settings: chip,
   flash, PSRAM.
3. `esp32/components/muse/Kconfig`, `CMakeLists.txt` and `idf_component.yml`
   register it, with any driver component it needs.
4. `esp32/tools/muse/board.sh`, `ports.py` and `avatar.py` get its short name.
5. The documentation gets it too: a row and a section in each README, rows in
   [`esp32/devices/README.md`](esp32/devices/README.md), a picture in
   `doc/image/`, and its sources and their licenses in
   [`CREDITS.md`](CREDITS.md).

The UI fits itself to round and rectangular screens. The pictures above come
from the simulator in `esp32/simulator`, with the screen size in its
`src/sim_board.c` changed to the board's: a way to see a new size before the
board is in hand. [`esp32/devices/AGENTS.md`](esp32/devices/AGENTS.md) has the
full recipe, from gathering the board's facts to checking the result.

## The rest of the SDK

| | |
|---|---|
| [**ESP32 Device SDK**](esp32) | Connect your ESP32 board to Muse. Throw in a screen to show images, add audio in and out, or wire up other sensors. |
| [**Linux Device SDK**](linux) | Turn that spare Raspberry Pi or Linux box into a Muse gadget. Hack in your own commands to let Muse handle sysadmin chores or your Home Assistant setup. |

ESP32 and Linux gadgets pair with the Muse app on iOS and Android, via
Settings > Devices. Turn on Developer mode there first, then look for devices
prefixed with "MuseGadget". Each directory has a `README.md` to get started and
an `AGENTS.md` for coding agents.

## Community

The upstream project's community meets on
[Discord](https://discord.gg/3bhjCkZdd6). For the boards this fork adds, open
an issue on this repository.

## License and credits

- The Muse Gadget SDK is copyright Meta Platforms, Inc. and affiliates, under
  the Apache License, Version 2.0, in [`LICENSE`](LICENSE).
- The changes in this fork are copyright 2026 ledienbien-ai, under the same
  license.
- The 1.85C port draws on Waveshare's documentation and
  [example code](https://github.com/waveshareteam/ESP32-S3-Touch-LCD-1.85C)
  (Apache-2.0) and on
  [xiaozhi-esp32](https://github.com/78/xiaozhi-esp32)'s board for it (MIT).
- The OSTB-3ST port takes its pins, panel setup table and battery table from
  the board's xiaozhi-esp32 firmware source, which carries no license notice of
  its own; xiaozhi-esp32 is MIT.
- Two upstream files keep their own licenses: `minimp3.h` (CC0-1.0) and
  `pixel_font.c` (BSD-2-Clause). Components fetched at build time are under
  their own licenses.
- The Apache License does not cover the [Jollybot avatar](esp32/avatar), nor
  the Meta, Muse and Waveshare names and marks.

[`CREDITS.md`](CREDITS.md) has the full list: every source, its license, the
files this fork added or changed, and what to keep in order in later versions.

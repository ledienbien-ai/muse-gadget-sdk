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
<!-- Modified by ledienbien-ai (2026): describes this fork, which adds the Waveshare ESP32-S3-Touch-LCD-1.85C. -->

# Muse Gadgets for the Waveshare ESP32-S3-Touch-LCD-1.85C

**English** | [Tiếng Việt](README.vi.md) | [简体中文](README.zh-CN.md) | [日本語](README.ja.md) | [한국어](README.ko.md)

> An unofficial community fork of
> [facebookincubator/muse-gadget-sdk](https://github.com/facebookincubator/muse-gadget-sdk).
> It is not affiliated with or endorsed by Meta or Waveshare.

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/images/muse-gadgets-dark.png">
    <img src=".github/images/muse-gadgets-light.png" width="900" alt="Muse gadgets: a Waveshare round AMOLED, an M5Stack StickS3, Muse Home Link, a Raspberry Pi and a Seeed reTerminal e-ink display">
  </picture>
</p>

Muse gadgets are open source devices you build yourself: program an
off-the-shelf ESP32 board or set up a Raspberry Pi, then connect Muse to your
displays, buttons, sensors and actuators. This fork adds one more board to the
ESP32 Device SDK, the round
[Waveshare ESP32-S3-Touch-LCD-1.85C](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C),
and leaves the rest of the SDK as it is upstream.

Built by hackers, for hackers, just for fun. Flashing custom firmware can brick
boards and void warranties. Proceed at your own risk!

## What this fork adds

The board runs the full on-screen UI: the animated avatar, push-to-talk,
settings by touch, and images from Muse.

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

**Status:** builds with ESP-IDF v6.0.1. It has not been verified on hardware
yet, and the V2 audio path is written from Waveshare's example code without a
V2 board to try it on.

Known limits: BOOT is the only button, so the screen sleeps on its timer and
powering off is on the Power page in Settings (the board deep-sleeps until BOOT
is pressed; the slide switch cuts power). The charger's status reaches no pin,
so the board shows "charging" only while a computer is attached.

## Build and flash

1. Get an [SDK token](https://gadgets.muse.ai/settings/sdk-tokens) and read the
   [Gadget SDK Terms](https://gadgets.muse.ai/sdk-terms). Every gadget needs a
   token to pair.
2. Install ESP-IDF **v6.0.1** (see [`esp32/README.md`](esp32/README.md)).
3. Build from `esp32/`. On macOS or Linux:

   ```sh
   tools/muse/board.sh build s3lcd
   ```

   On Windows, in an ESP-IDF PowerShell:

   ```powershell
   idf.py -B build-muse-waveshare-s3-185c -DIDF_TARGET=esp32s3 `
     -DSDKCONFIG=build-muse-waveshare-s3-185c/sdkconfig `
     "-DSDKCONFIG_DEFAULTS=sdkconfig.defaults;devices/sdkconfig.muse;devices/sdkconfig.muse-waveshare-s3-185c" build
   ```

   Set the token with `idf.py -B build-muse-waveshare-s3-185c menuconfig`
   (ESP32 Device SDK > Muse Gadgets SDK token), then build again.
4. Flash over USB-C, replacing `PORT` with your board's port. If it can't
   connect, hold **BOOT**, tap **RESET**, release **BOOT**, and try again.

   ```sh
   idf.py -B build-muse-waveshare-s3-185c -p PORT flash
   ```

   For a web flasher, make one file and write it at address `0x0`:

   ```sh
   idf.py -B build-muse-waveshare-s3-185c merge-bin -o muse-gadget-185c-merged.bin
   ```

   A build made with your SDK token carries that token, so think before you
   publish the file.
5. In the Muse app, turn on **Settings > Devices > Developer mode**, add the
   device named `MuseGadget-XXXXXX`, and press **BOOT** when asked.

The board's code is in
[`board_waveshare_s3_185c.c`](esp32/components/muse/boards/board_waveshare_s3_185c.c)
and its build settings in
[`sdkconfig.muse-waveshare-s3-185c`](esp32/devices/sdkconfig.muse-waveshare-s3-185c).
[`esp32/devices/AGENTS.md`](esp32/devices/AGENTS.md) explains how a board is added.

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
[Discord](https://discord.gg/3bhjCkZdd6). For this board, open an issue on this
repository.

## License and credits

- The Muse Gadget SDK is copyright Meta Platforms, Inc. and affiliates, under
  the Apache License, Version 2.0, in [`LICENSE`](LICENSE).
- The changes in this fork are copyright 2026 ledienbien-ai, under the same
  license.
- The 1.85C port draws on Waveshare's documentation and
  [example code](https://github.com/waveshareteam/ESP32-S3-Touch-LCD-1.85C)
  (Apache-2.0) and on
  [xiaozhi-esp32](https://github.com/78/xiaozhi-esp32)'s board for it (MIT).
- Two upstream files keep their own licenses: `minimp3.h` (CC0-1.0) and
  `pixel_font.c` (BSD-2-Clause). Components fetched at build time are under
  their own licenses.
- The Apache License does not cover the [Jollybot avatar](esp32/avatar), nor
  the Meta, Muse and Waveshare names and marks.

[`CREDITS.md`](CREDITS.md) has the full list: every source, its license, the
files this fork added or changed, and what to keep in order in later versions.

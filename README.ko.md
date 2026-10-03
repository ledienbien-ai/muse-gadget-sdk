<!--
Copyright (c) Meta Platforms, Inc. and affiliates.
Copyright (c) 2026 ledienbien-ai (translation and fork-specific text)

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

# Waveshare ESP32-S3-Touch-LCD-1.85C용 Muse Gadgets

[English](README.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh-CN.md) | [日本語](README.ja.md) | **한국어**

> 이 저장소는 [facebookincubator/muse-gadget-sdk](https://github.com/facebookincubator/muse-gadget-sdk)의
> 비공식 커뮤니티 포크입니다. Meta 및 Waveshare와 관련이 없으며, 두 회사의
> 승인을 받은 것도 아닙니다. 번역과 [영어판](README.md)이 다를 경우 영어판이
> 기준입니다.

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/images/muse-gadgets-dark.png">
    <img src=".github/images/muse-gadgets-light.png" width="900" alt="Muse gadget: Waveshare 원형 AMOLED, M5Stack StickS3, Muse Home Link, Raspberry Pi, Seeed reTerminal 전자잉크 디스플레이">
  </picture>
</p>

Muse gadget은 직접 만드는 오픈 소스 기기입니다. 시중에서 파는 ESP32 보드에
프로그램을 올리거나 Raspberry Pi를 설정한 다음, 디스플레이, 버튼, 센서,
액추에이터를 Muse에 연결합니다. 이 포크는 ESP32 Device SDK에 보드 하나를
추가합니다. 원형 화면을 가진
[Waveshare ESP32-S3-Touch-LCD-1.85C](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C)입니다.
SDK의 나머지 부분은 업스트림과 같습니다.

해커가 해커를 위해 재미로 만든 프로젝트입니다. 커스텀 펌웨어를 올리면 보드가
벽돌이 되거나 보증이 무효가 될 수 있습니다. 위험은 본인이 감수해야 합니다!

## 이 포크에서 추가한 것

이 보드는 화면 UI 전체를 실행합니다. 움직이는 아바타, 눌러서 말하기, 터치로
하는 설정, Muse가 보내는 이미지 표시입니다.

| 항목 | 내용 |
|---|---|
| 칩 | ESP32-S3R8, 16 MB 플래시, 8 MB 옥탈 PSRAM, 네이티브 USB |
| 디스플레이 | 1.85인치 원형 360×360 ST77916 LCD(QSPI), PWM 백라이트 |
| 터치 | CST816 |
| 오디오, V1 보드 | PCM5101 DAC와 디지털 I2S 마이크 |
| 오디오, V2 보드("Rev2.0") | ES8311 DAC와 ES7210 ADC, 마이크 2개 |
| 버튼 | BOOT: 누르고 있는 동안 말하기 |
| 배터리 | GPIO8의 분압 회로로 전압 측정 |

펌웨어 하나로 두 오디오 버전을 모두 지원합니다. 부팅할 때 ES8311이 있는지
확인합니다.

**상태:** ESP-IDF v6.0.1로 빌드됩니다. 아직 실제 하드웨어에서 검증하지
않았으며, V2 오디오 부분은 V2 보드 없이 Waveshare의 예제 코드를 바탕으로
작성했습니다.

알려진 제한: 버튼이 BOOT 하나뿐이어서 화면은 타이머에 따라 꺼지고, 전원 끄기는
Settings의 Power 페이지에 있습니다(보드는 딥 슬립에 들어가며 BOOT를 누르면
깨어납니다. 전원을 완전히 끊는 것은 슬라이드 스위치입니다). 충전 IC의 상태가
어느 핀에도 연결되어 있지 않아서, 컴퓨터에 연결되어 있을 때만 "충전 중"으로
표시됩니다.

## 빌드와 플래시

1. [SDK 토큰](https://gadgets.muse.ai/settings/sdk-tokens)을 받고
   [Gadget SDK 약관](https://gadgets.muse.ai/sdk-terms)을 읽습니다. 모든
   gadget은 페어링하려면 토큰이 필요합니다.
2. ESP-IDF **v6.0.1**을 설치합니다([`esp32/README.md`](esp32/README.md) 참고).
3. `esp32/`에서 빌드합니다. macOS 또는 Linux:

   ```sh
   tools/muse/board.sh build s3lcd
   ```

   Windows에서는 ESP-IDF PowerShell에서:

   ```powershell
   idf.py -B build-muse-waveshare-s3-185c -DIDF_TARGET=esp32s3 `
     -DSDKCONFIG=build-muse-waveshare-s3-185c/sdkconfig `
     "-DSDKCONFIG_DEFAULTS=sdkconfig.defaults;devices/sdkconfig.muse;devices/sdkconfig.muse-waveshare-s3-185c" build
   ```

   토큰은 `idf.py -B build-muse-waveshare-s3-185c menuconfig`
   (ESP32 Device SDK > Muse Gadgets SDK token)에서 설정한 뒤 다시 빌드합니다.
4. USB-C로 플래시합니다. `PORT`는 보드의 포트로 바꾸세요. 연결되지 않으면
   **BOOT**를 누른 채 **RESET**을 눌렀다 떼고, **BOOT**를 뗀 다음 다시
   시도합니다.

   ```sh
   idf.py -B build-muse-waveshare-s3-185c -p PORT flash
   ```

   웹 플래시 도구를 쓰려면 파일 하나로 합쳐서 주소 `0x0`에 씁니다.

   ```sh
   idf.py -B build-muse-waveshare-s3-185c merge-bin -o muse-gadget-185c-merged.bin
   ```

   본인의 SDK 토큰으로 빌드한 펌웨어에는 그 토큰이 들어 있으니, 파일을
   공개하기 전에 신중히 생각하세요.
5. Muse 앱에서 **Settings > Devices > Developer mode**를 켜고,
   `MuseGadget-XXXXXX`라는 이름의 기기를 추가한 다음, 요청이 나오면 **BOOT**를
   누릅니다.

보드 코드는
[`board_waveshare_s3_185c.c`](esp32/components/muse/boards/board_waveshare_s3_185c.c)에,
빌드 설정은
[`sdkconfig.muse-waveshare-s3-185c`](esp32/devices/sdkconfig.muse-waveshare-s3-185c)에
있습니다. 보드를 추가하는 방법은
[`esp32/devices/AGENTS.md`](esp32/devices/AGENTS.md)(영어)에 설명되어 있습니다.

## SDK의 나머지 부분

| | |
|---|---|
| [**ESP32 Device SDK**](esp32) | ESP32 보드를 Muse에 연결합니다. 화면을 달아 이미지를 보여 주거나, 오디오 입출력을 추가하거나, 다른 센서를 연결할 수 있습니다. |
| [**Linux Device SDK**](linux) | 남는 Raspberry Pi나 Linux 컴퓨터를 Muse gadget으로 만듭니다. 직접 만든 명령을 넣어 시스템 관리 잡무나 Home Assistant 설정을 Muse에게 맡길 수 있습니다. |

ESP32와 Linux gadget은 iOS와 Android의 Muse 앱에서 Settings > Devices를 통해
페어링합니다. 먼저 거기서 Developer mode를 켠 다음, 이름이 "MuseGadget"으로
시작하는 기기를 찾으세요. 각 디렉터리에는 시작 방법을 담은 `README.md`와 코딩
에이전트를 위한 `AGENTS.md`가 있습니다(둘 다 영어).

## 커뮤니티

업스트림 프로젝트의 커뮤니티는 [Discord](https://discord.gg/3bhjCkZdd6)에
있습니다. 이 보드에 관한 내용은 이 저장소에 이슈를 등록해 주세요.

## 라이선스와 크레딧

- Muse Gadget SDK의 저작권은 Meta Platforms, Inc. 및 그 계열사에 있으며,
  Apache License, Version 2.0으로 제공됩니다. [`LICENSE`](LICENSE)를
  참고하세요.
- 이 포크의 변경 사항은 ledienbien-ai(2026)에게 저작권이 있으며, 같은
  라이선스로 제공됩니다.
- 1.85C 포팅은 Waveshare의 문서와
  [예제 코드](https://github.com/waveshareteam/ESP32-S3-Touch-LCD-1.85C)
  (Apache-2.0), 그리고 [xiaozhi-esp32](https://github.com/78/xiaozhi-esp32)의
  해당 보드 코드(MIT)를 참고했습니다.
- 업스트림 파일 중 두 개는 자체 라이선스를 유지합니다: `minimp3.h`(CC0-1.0)와
  `pixel_font.c`(BSD-2-Clause). 빌드할 때 내려받는 컴포넌트에는 각자의
  라이선스가 적용됩니다.
- Apache License는 [Jollybot 아바타](esp32/avatar)에 적용되지 않으며, Meta,
  Muse, Waveshare의 이름과 상표에도 적용되지 않습니다.

전체 목록은 [`CREDITS.md`](CREDITS.md)(영어)에 있습니다. 각 출처와 그
라이선스, 이 포크에서 추가하거나 변경한 파일, 이후 버전에서 지켜야 할 사항을
담고 있습니다.

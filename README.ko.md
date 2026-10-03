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

# ESP32-S3 기기용 Muse Gadgets

[English](README.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh-CN.md) | [日本語](README.ja.md) | **한국어**

> 이 저장소는 [facebookincubator/muse-gadget-sdk](https://github.com/facebookincubator/muse-gadget-sdk)의
> 비공식 커뮤니티 포크입니다. Meta, Waveshare를 비롯한 어떤 보드 제조사와도
> 관련이 없으며, 그 회사들의 승인을 받은 것도 아닙니다. 번역과
> [영어판](README.md)이 다를 경우 영어판이 기준입니다.

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/images/muse-gadgets-dark.png">
    <img src=".github/images/muse-gadgets-light.png" width="900" alt="Muse gadget: Waveshare 원형 AMOLED, M5Stack StickS3, Muse Home Link, Raspberry Pi, Seeed reTerminal 전자잉크 디스플레이">
  </picture>
</p>

Muse gadget은 직접 만드는 오픈 소스 기기입니다. 시중에서 파는 ESP32 보드에
프로그램을 올리거나 Raspberry Pi를 설정한 다음, 디스플레이, 버튼, 센서,
액추에이터를 Muse에 연결합니다. 이 포크는 업스트림 ESP32 Device SDK가 아직
지원하지 않는 ESP32-S3 보드를 추가합니다. SDK의 나머지 부분은 업스트림과
같습니다.

해커가 해커를 위해 재미로 만든 프로젝트입니다. 커스텀 펌웨어를 올리면 보드가
벽돌이 되거나 보증이 무효가 될 수 있습니다. 위험은 본인이 감수해야 합니다!

## 이 포크의 보드

모든 보드가 화면 UI 전체를 실행합니다. 움직이는 아바타, 눌러서 말하기, 터치로
하는 설정, Muse가 보내는 이미지 표시입니다.

| 보드 | 화면 | 이름 | 프로필 | 상태 |
|---|---|---|---|---|
| [Waveshare ESP32-S3-Touch-LCD-1.85C](#waveshare-esp32-s3-touch-lcd-185c) | 1.85인치 원형 360×360, 터치 | `s3lcd` | `waveshare-s3-185c` | 실제 보드에 플래시함: 부팅되고 UI가 표시됩니다. Muse와의 대화는 아직 확인하지 않았습니다 |
| [OSTB-3ST](#ostb-3st) | 1.83인치 296×240, 터치 | `ostb` | `ostb-3st` | 빌드됩니다. 실제 보드에서는 아직 실행하지 않았습니다 |

이름은 `tools/muse/board.sh`가 보드를 부르는 이름이고, 프로필은 빌드 설정과
빌드 디렉터리의 이름입니다. 업스트림이 지원하는 보드도 그대로 남아 있으며,
[`esp32/devices/README.md`](esp32/devices/README.md)(영어)에 정리되어 있습니다.

## Waveshare ESP32-S3-Touch-LCD-1.85C

<p align="center">
  <img src="doc/image/Wareshare%20Touch%20LCD%201.85C.png" width="720" alt="시뮬레이터로 그린 360 px 원형 화면의 Muse UI: 대기, 페어링, 듣는 중, 생각 중, 오류, 말하는 중">
</p>

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

**상태:** ESP-IDF v6.0.1로 빌드되며, 실제 보드에 플래시해서 부팅되고 UI가
표시되는 것을 확인했습니다. 이 보드에서 Muse와의 대화는 아직 해 보지 않았으며,
V2 오디오 부분은 V2 보드 없이 Waveshare의 예제 코드를 바탕으로 작성했습니다.

알려진 제한: 버튼이 BOOT 하나뿐이어서 화면은 타이머에 따라 꺼지고, 전원 끄기는
Settings의 Power 페이지에 있습니다(보드는 딥 슬립에 들어가며 BOOT를 누르면
깨어납니다. 전원을 완전히 끊는 것은 슬라이드 스위치입니다). 충전 IC의 상태가
어느 핀에도 연결되어 있지 않아서, 컴퓨터에 연결되어 있을 때만 "충전 중"으로
표시됩니다.

코드: [`board_waveshare_s3_185c.c`](esp32/components/muse/boards/board_waveshare_s3_185c.c),
빌드 설정: [`sdkconfig.muse-waveshare-s3-185c`](esp32/devices/sdkconfig.muse-waveshare-s3-185c).
하드웨어 자료: [Waveshare 문서](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C).

## OSTB-3ST

<p align="center">
  <img src="doc/image/ostb-3st.png" width="720" alt="시뮬레이터로 그린 296×240 Muse UI: 대기, 페어링, 듣는 중, 생각 중, 오류, 말하는 중">
</p>
<p align="center">
  <img src="doc/image/ostb-3st-settings.png" width="720" alt="시뮬레이터로 그린 296×240 설정 페이지">
</p>

이 보드의 xiaozhi-esp32 펌웨어 소스(`ostb-xiaozhi-3st`)를 바탕으로
포팅했습니다. UI는 296×240 가로 화면에 맞춰 배치됩니다.

| 부품 | 내용 |
|---|---|
| 칩 | ESP32-S3, 플래시 16 MB, 옥탈 PSRAM 8 MB, 네이티브 USB |
| 디스플레이 | 1.83인치 240×296 NV3023 LCD(SPI), 가로로 사용, PWM 백라이트 |
| 터치 | CST816. 인터럽트 선이 없어 폴링으로 읽습니다 |
| 오디오 | ES8311 DAC와 ES7210 ADC |
| 키 | 위 키(볼륨 +): 누르고 있는 동안 말하기. 아래 키(볼륨 −): 누르면 화면 끄기, 길게 누르면 전원 끄기 |
| 배터리 | 원래 펌웨어의 ADC 표로 계산한 잔량과 충전 상태 핀 |

**상태:** ESP-IDF v6.0.1로 빌드됩니다. 실제 보드에서는 아직 실행하지
못했습니다. 핀, 패널 초기화 표와 화면 방향은 그 펌웨어의 소스에서 가져온
것이며, 대조해 볼 문서가 없습니다. 화면이 돌아가거나 뒤집혀 나오거나 터치
위치가 맞지 않으면 보드 코드 파일 맨 위의 `LCD_MADCTL` 또는 `TP_SWAP_XY`,
`TP_MIRROR_X`, `TP_MIRROR_Y`를 바꾸세요.

알려진 제한: 4G 모뎀과 LED는 쓰지 않습니다. 배터리는 잔량만 표시하고 전압은
표시하지 않습니다. 전원 끄기는 보드의 전원 차단 핀을 구동합니다. USB 전원에서는
보드가 꺼지지 않을 수 있으며, 그때는 위 키를 누를 때까지 화면이 꺼진 상태로
있습니다. 배터리가 가득 찬 뒤에는 컴퓨터에 연결되어 있는 동안에만 USB 전원으로
인식합니다.

코드: [`board_ostb_3st.c`](esp32/components/muse/boards/board_ostb_3st.c),
빌드 설정: [`sdkconfig.muse-ostb-3st`](esp32/devices/sdkconfig.muse-ostb-3st).

## 빌드와 플래시

절차는 모든 보드에서 같습니다. 이름과 프로필은 [위의 표](#이-포크의-보드)에서
확인하세요.

1. [SDK 토큰](https://gadgets.muse.ai/settings/sdk-tokens)을 받고
   [Gadget SDK 약관](https://gadgets.muse.ai/sdk-terms)을 읽습니다. 모든
   gadget은 페어링하려면 토큰이 필요합니다.
2. ESP-IDF **v6.0.1**을 설치합니다([`esp32/README.md`](esp32/README.md) 참고).
3. `esp32/`에서 빌드합니다. macOS 또는 Linux에서는 보드의 이름을 씁니다.

   ```sh
   tools/muse/board.sh build ostb
   ```

   Windows에서는 ESP-IDF PowerShell에서 보드의 프로필을 씁니다.

   ```powershell
   $P = "ostb-3st"
   idf.py -B build-muse-$P -DIDF_TARGET=esp32s3 `
     "-DSDKCONFIG=build-muse-$P/sdkconfig" `
     "-DSDKCONFIG_DEFAULTS=sdkconfig.defaults;devices/sdkconfig.muse;devices/sdkconfig.muse-$P" build
   ```

   토큰은 `idf.py -B build-muse-$P menuconfig`
   (ESP32 Device SDK > Muse Gadgets SDK token)에서 설정한 뒤 다시 빌드합니다.
4. USB-C로 플래시합니다. `PORT`는 보드의 포트로 바꾸세요.

   ```powershell
   idf.py -B build-muse-$P -p PORT flash
   ```

   연결되지 않으면 보드를 다운로드 모드로 만든 뒤 다시 시도합니다. **BOOT**를
   누른 채 **RESET**을 눌렀다 떼고, **BOOT**를 뗍니다. RESET 버튼이 없는
   보드에서는 BOOT를 누른 채 케이블을 꽂습니다.

   웹 플래시 도구를 쓰려면 파일 하나로 합쳐서 주소 `0x0`에 씁니다.

   ```powershell
   idf.py -B build-muse-$P merge-bin -o muse-gadget-$P-merged.bin
   ```

   본인의 SDK 토큰으로 빌드한 펌웨어에는 그 토큰이 들어 있으니, 파일을
   공개하기 전에 신중히 생각하세요.
5. Muse 앱에서 **Settings > Devices > Developer mode**를 켜고,
   `MuseGadget-XXXXXX`라는 이름의 기기를 추가한 다음, 요청이 나오면 보드의
   말하기 버튼을 누릅니다(1.85C는 BOOT, OSTB-3ST는 위 키).

## 시작 로고

<p align="center">
  <img src="doc/image/boot-logo.png" width="560" alt="시뮬레이터로 그린 시작 로고: 1.85C의 원형 화면과 OSTB-3ST">
</p>

전체 UI를 쓰는 보드는 모두 시작할 때 로고를 2.5초 동안 보여 준 다음 평소
화면으로 넘어갑니다. 로고는
[`esp32/components/muse/logo/logo.c`](esp32/components/muse/logo)이며, 투명도가
있는 240×240 이미지입니다.
[LVGL 이미지 변환기](https://lvgl.io/tools/imageconverter)가 출력하는 형식
그대로이고, 색 형식은 RGB565A8, 이름은 `logo`입니다. 자신의 로고를 쓰려면 같은
이름으로 새로 내보낸 파일로 바꾸면 됩니다. 화면보다 큰 이미지는 맞게 축소되고,
작은 이미지는 원래 크기 그대로 표시됩니다.

`idf.py -B build-muse-$P menuconfig`(Component config > Muse > Show a logo
while starting up)에서 로고를 끄거나 표시 시간을 바꿀 수 있습니다.

## 다른 기기 추가하기

화면, 스피커, 마이크가 있는 보드라면 드라이버 파일 하나와 등록 몇 줄이면
됩니다. 이 포크의 각 보드는 다음으로 이루어져 있습니다.

1. `esp32/components/muse/boards/board_<id>.c`가 `muse_board_t`
   (`muse_board.h`)를 채웁니다. 디스플레이, 터치, 오디오, 버튼, 배터리,
   전원입니다. 자기 보드와 가장 비슷한 보드에서 시작하세요.
2. `esp32/devices/sdkconfig.muse-<profile>`에 빌드 설정을 둡니다. 칩, 플래시,
   PSRAM입니다.
3. `esp32/components/muse/Kconfig`, `CMakeLists.txt`, `idf_component.yml`에
   보드와, 필요한 드라이버 컴포넌트를 등록합니다.
4. `esp32/tools/muse/board.sh`, `ports.py`, `avatar.py`에 짧은 이름을
   추가합니다.
5. 문서에도 추가합니다. 각 README의 표에 한 줄과 절 하나,
   [`esp32/devices/README.md`](esp32/devices/README.md)의 줄들, `doc/image/`의
   그림, 그리고 [`CREDITS.md`](CREDITS.md)에 참고한 출처와 그 라이선스입니다.

UI는 원형 화면과 직사각형 화면에 맞춰 스스로 배치됩니다. 위의 그림은
`esp32/simulator`의 시뮬레이터에서 `src/sim_board.c`의 화면 크기를 보드의
크기로 바꿔 그린 것입니다. 보드를 손에 넣기 전에 새 크기에서 어떻게 보이는지
확인할 수 있습니다. 보드 정보를 모으는 일부터 결과를 확인하는 일까지의 전체
절차는 [`esp32/devices/AGENTS.md`](esp32/devices/AGENTS.md)(영어)에 있습니다.

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
있습니다. 이 포크에서 추가한 보드에 관한 내용은 이 저장소에 이슈를 등록해
주세요.

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
- OSTB-3ST 포팅은 핀, 패널 초기화 표, 배터리 표를 이 보드의 xiaozhi-esp32
  펌웨어 소스에서 가져왔습니다. 그 파일 자체에는 라이선스 표기가 없습니다.
  xiaozhi-esp32는 MIT입니다.
- 업스트림 파일 중 두 개는 자체 라이선스를 유지합니다: `minimp3.h`(CC0-1.0)와
  `pixel_font.c`(BSD-2-Clause). 빌드할 때 내려받는 컴포넌트에는 각자의
  라이선스가 적용됩니다.
- Apache License는 [Jollybot 아바타](esp32/avatar)와 DB_ROBOT 시작 로고에
  적용되지 않으며, Meta, Muse, Waveshare의 이름과 상표에도 적용되지 않습니다.

전체 목록은 [`CREDITS.md`](CREDITS.md)(영어)에 있습니다. 각 출처와 그
라이선스, 이 포크에서 추가하거나 변경한 파일, 이후 버전에서 지켜야 할 사항을
담고 있습니다.

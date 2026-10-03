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

# Waveshare ESP32-S3-Touch-LCD-1.85C 向け Muse Gadgets

[English](README.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh-CN.md) | **日本語** | [한국어](README.ko.md)

> これは [facebookincubator/muse-gadget-sdk](https://github.com/facebookincubator/muse-gadget-sdk)
> の非公式なコミュニティフォークです。Meta および Waveshare とは関係がなく、
> 両社の承認を受けたものでもありません。翻訳と[英語版](README.md)に違いがある
> 場合は、英語版が正となります。

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/images/muse-gadgets-dark.png">
    <img src=".github/images/muse-gadgets-light.png" width="900" alt="Muse gadget：Waveshare の円形 AMOLED、M5Stack StickS3、Muse Home Link、Raspberry Pi、Seeed reTerminal の電子ペーパー">
  </picture>
</p>

Muse gadget は、自分で作るオープンソースのデバイスです。市販の ESP32 ボードに
プログラムを書き込むか、Raspberry Pi をセットアップして、ディスプレイ、ボタン、
センサー、アクチュエーターを Muse につなぎます。このフォークは ESP32 Device SDK
にボードを 1 つ追加します。円形ディスプレイの
[Waveshare ESP32-S3-Touch-LCD-1.85C](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C)
です。SDK のそれ以外の部分は上流のままです。

ハッカーがハッカーのために、楽しみで作ったものです。カスタムファームウェアの
書き込みはボードを壊したり、保証を無効にしたりすることがあります。自己責任で
どうぞ。

## このフォークで追加したもの

このボードでは画面 UI がすべて動きます。アニメーションするアバター、
プッシュトゥトーク、タッチでの設定、Muse から届く画像の表示です。

| 項目 | 内容 |
|---|---|
| チップ | ESP32-S3R8、16 MB フラッシュ、8 MB オクタル PSRAM、ネイティブ USB |
| ディスプレイ | 1.85 インチ円形 360×360 ST77916 LCD（QSPI）、PWM バックライト |
| タッチ | CST816 |
| オーディオ（V1 ボード） | PCM5101 DAC とデジタル I2S マイク |
| オーディオ（V2 ボード、「Rev2.0」） | ES8311 DAC と ES7210 ADC、マイク 2 個 |
| ボタン | BOOT：押している間だけ話す |
| バッテリー | GPIO8 の分圧回路で電圧を測定 |

1 つのファームウェアで両方のオーディオ版に対応します。起動時に ES8311 の有無を
調べます。

**状況：** ESP-IDF v6.0.1 でビルドできます。実機での動作はまだ確認していません。
V2 のオーディオ部分は Waveshare のサンプルコードをもとに書いたもので、V2 ボード
では試していません。

既知の制限：ボタンは BOOT だけなので、画面はタイマーで消灯し、電源オフは
Settings の Power ページから行います（ボードはディープスリープに入り、BOOT を
押すと復帰します。電源を完全に切るのはスライドスイッチです）。充電 IC の状態は
どのピンにもつながっていないため、「充電中」と表示されるのはパソコンに接続して
いる間だけです。

## ビルドと書き込み

1. [SDK トークン](https://gadgets.muse.ai/settings/sdk-tokens)を取得し、
   [Gadget SDK 規約](https://gadgets.muse.ai/sdk-terms)を読みます。ペアリング
   にはどの gadget でもトークンが必要です。
2. ESP-IDF **v6.0.1** をインストールします（[`esp32/README.md`](esp32/README.md)
   を参照）。
3. `esp32/` でビルドします。macOS または Linux：

   ```sh
   tools/muse/board.sh build s3lcd
   ```

   Windows では ESP-IDF PowerShell で：

   ```powershell
   idf.py -B build-muse-waveshare-s3-185c -DIDF_TARGET=esp32s3 `
     -DSDKCONFIG=build-muse-waveshare-s3-185c/sdkconfig `
     "-DSDKCONFIG_DEFAULTS=sdkconfig.defaults;devices/sdkconfig.muse;devices/sdkconfig.muse-waveshare-s3-185c" build
   ```

   トークンは `idf.py -B build-muse-waveshare-s3-185c menuconfig`
   （ESP32 Device SDK > Muse Gadgets SDK token）で設定し、もう一度ビルドします。
4. USB-C で書き込みます。`PORT` はボードのポートに置き換えてください。接続
   できない場合は、**BOOT** を押したまま **RESET** を押して離し、**BOOT** を
   離してから、もう一度試します。

   ```sh
   idf.py -B build-muse-waveshare-s3-185c -p PORT flash
   ```

   Web の書き込みツールを使う場合は、1 つのファイルにまとめてアドレス `0x0` に
   書き込みます。

   ```sh
   idf.py -B build-muse-waveshare-s3-185c merge-bin -o muse-gadget-185c-merged.bin
   ```

   自分の SDK トークンでビルドしたファームウェアにはそのトークンが含まれます。
   ファイルを公開する前によく考えてください。
5. Muse アプリで **Settings > Devices > Developer mode** をオンにし、
   `MuseGadget-XXXXXX` という名前のデバイスを追加して、求められたら **BOOT** を
   押します。

ボードのコードは
[`board_waveshare_s3_185c.c`](esp32/components/muse/boards/board_waveshare_s3_185c.c)、
ビルド設定は
[`sdkconfig.muse-waveshare-s3-185c`](esp32/devices/sdkconfig.muse-waveshare-s3-185c)
にあります。ボードの追加方法は
[`esp32/devices/AGENTS.md`](esp32/devices/AGENTS.md)（英語）に書かれています。

## SDK のそれ以外の部分

| | |
|---|---|
| [**ESP32 Device SDK**](esp32) | ESP32 ボードを Muse につなぎます。画面を足して画像を表示したり、音声の入出力を加えたり、ほかのセンサーをつないだりできます。 |
| [**Linux Device SDK**](linux) | 余っている Raspberry Pi や Linux マシンを Muse gadget にします。独自のコマンドを組み込めば、システム管理の雑用や Home Assistant の操作を Muse に任せられます。 |

ESP32 と Linux の gadget は、iOS と Android の Muse アプリの
Settings > Devices からペアリングします。先にそこで Developer mode をオンにし、
名前が「MuseGadget」で始まるデバイスを探してください。各ディレクトリには、
始め方を書いた `README.md` と、コーディングエージェント向けの `AGENTS.md`
があります（どちらも英語）。

## コミュニティ

上流プロジェクトのコミュニティは [Discord](https://discord.gg/3bhjCkZdd6)
にあります。このボードについては、このリポジトリに issue を立ててください。

## ライセンスとクレジット

- Muse Gadget SDK の著作権は Meta Platforms, Inc. およびその関連会社にあり、
  Apache License, Version 2.0 で提供されています。[`LICENSE`](LICENSE) を参照
  してください。
- このフォークでの変更の著作権は ledienbien-ai（2026）にあり、同じライセンスで
  提供します。
- 1.85C への移植は、Waveshare のドキュメントと
  [サンプルコード](https://github.com/waveshareteam/ESP32-S3-Touch-LCD-1.85C)
  （Apache-2.0）、および
  [xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) の同ボード用コード（MIT）
  を参考にしています。
- 上流のファイルのうち 2 つは独自のライセンスのままです：`minimp3.h`
  （CC0-1.0）と `pixel_font.c`（BSD-2-Clause）。ビルド時に取得する
  コンポーネントには、それぞれのライセンスが適用されます。
- Apache License は [Jollybot アバター](esp32/avatar)には適用されません。Meta、
  Muse、Waveshare の名称と商標にも適用されません。

完全な一覧は [`CREDITS.md`](CREDITS.md)（英語）にあります。各ソースとその
ライセンス、このフォークで追加または変更したファイル、今後のバージョンで守る
べきことをまとめています。

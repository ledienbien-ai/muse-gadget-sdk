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

<p align="center">
  <a href="https://dbrobot.vn/"><img src="doc/image/db-robot-logo.png" width="160" alt="DB_ROBOT"></a>
</p>
<p align="center">
  開発者: <b>ledienbien-ai</b> · ホームページ: <a href="https://dbrobot.vn/">https://dbrobot.vn/</a>
</p>

# ESP32-S3 デバイス向け Muse Gadgets

[English](README.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh-CN.md) | **日本語** | [한국어](README.ko.md)

> これは [facebookincubator/muse-gadget-sdk](https://github.com/facebookincubator/muse-gadget-sdk)
> の非公式なコミュニティフォークです。Meta、Waveshare、その他のボードメーカーとは
> 関係がなく、各社の承認を受けたものでもありません。翻訳と[英語版](README.md)に
> 違いがある場合は、英語版が正となります。

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/images/muse-gadgets-dark.png">
    <img src=".github/images/muse-gadgets-light.png" width="900" alt="Muse gadget：Waveshare の円形 AMOLED、M5Stack StickS3、Muse Home Link、Raspberry Pi、Seeed reTerminal の電子ペーパー">
  </picture>
</p>

Muse gadget は、自分で作るオープンソースのデバイスです。市販の ESP32 ボードに
プログラムを書き込むか、Raspberry Pi をセットアップして、ディスプレイ、ボタン、
センサー、アクチュエーターを Muse につなぎます。このフォークは、上流の
ESP32 Device SDK がまだ対応していない ESP32-S3 ボードを追加します。SDK の
それ以外の部分は上流のままです。

ハッカーがハッカーのために、楽しみで作ったものです。カスタムファームウェアの
書き込みはボードを壊したり、保証を無効にしたりすることがあります。自己責任で
どうぞ。

## このフォークのボード

どのボードでも画面 UI がすべて動きます。アニメーションするアバター、
プッシュトゥトーク、タッチでの設定、Muse から届く画像の表示です。

| ボード | 画面 | 名前 | プロファイル | 状況 |
|---|---|---|---|---|
| [Waveshare ESP32-S3-Touch-LCD-1.85C](#waveshare-esp32-s3-touch-lcd-185c) | 1.85 インチ円形 360×360、タッチ | `s3lcd` | `waveshare-s3-185c` | ファームウェアのオンライン書き込み：https://dbrobot.vn/firmware.html |
| [OSTB-3ST](#ostb-3st) | 1.83 インチ 296×240、タッチ | `ostb` | `ostb-3st` | ファームウェアのオンライン書き込み：https://dbrobot.vn/firmware.html |
| [LCDWIKI 2.8inch ESP32-S3 Display](#lcdwiki-28inch-esp32-s3-display) | 2.8 インチ 320×240、ES3C28P はタッチ付き | `lcd28` | `lcdwiki-s3-28` | ファームウェアのオンライン書き込み：https://dbrobot.vn/firmware.html |

名前は `tools/muse/board.sh` でのボードの呼び名、プロファイルはビルド設定と
ビルドディレクトリの名前です。上流が対応しているボードもそのまま残っていて、
[`esp32/devices/README.md`](esp32/devices/README.md)（英語）に一覧があります。

## Waveshare ESP32-S3-Touch-LCD-1.85C

<p align="center">
  <img src="doc/image/waveshare-s3-185c-device.jpg" width="300" alt="ケースに入った Waveshare ESP32-S3-Touch-LCD-1.85C">
  <img src="doc/image/waveshare-s3-185c-muse.jpg" width="300" alt="1.85C に表示した Muse の設定画面">
</p>
<p align="center">
  <img src="doc/image/Wareshare%20Touch%20LCD%201.85C.png" width="720" alt="シミュレーターで描いた 360 px 円形画面の Muse UI：待機、ペアリング、聞き取り、考え中、エラー、発話">
</p>

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

**状況：** ESP-IDF v6.0.1 でビルドでき、実機に書き込んで、起動して UI が表示
されることを確認しています。このボードで Muse との会話はまだ試していません。
V2 のオーディオ部分は Waveshare のサンプルコードをもとに書いたもので、V2 ボード
では試していません。

既知の制限：ボタンは BOOT だけなので、画面はタイマーで消灯し、電源オフは
Settings の Power ページから行います（ボードはディープスリープに入り、BOOT を
押すと復帰します。電源を完全に切るのはスライドスイッチです）。充電 IC の状態は
どのピンにもつながっていないため、「充電中」と表示されるのはパソコンに接続して
いる間だけです。

コード：[`board_waveshare_s3_185c.c`](esp32/components/muse/boards/board_waveshare_s3_185c.c)、
ビルド設定：[`sdkconfig.muse-waveshare-s3-185c`](esp32/devices/sdkconfig.muse-waveshare-s3-185c)。
ハードウェアの資料：[Waveshare のドキュメント](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C)。

## OSTB-3ST

<p align="center">
  <img src="doc/image/ostb-3st-device.jpg" width="560" alt="OSTB-3ST の前面と背面">
</p>
<p align="center">
  <img src="doc/image/ostb-3st.png" width="720" alt="シミュレーターで描いた 296×240 の Muse UI：待機、ペアリング、聞き取り、考え中、エラー、発話">
</p>
<p align="center">
  <img src="doc/image/ostb-3st-settings.png" width="720" alt="シミュレーターで描いた 296×240 の設定ページ">
</p>

このボード用の xiaozhi-esp32 ファームウェアのソース（`ostb-xiaozhi-3st`）を
もとに移植しました。UI は 296×240 の横長画面に合わせたレイアウトです。

| 部品 | 内容 |
|---|---|
| チップ | ESP32-S3、フラッシュ 16 MB、オクタル PSRAM 8 MB、ネイティブ USB |
| ディスプレイ | 1.83 インチ 240×296 NV3023 LCD（SPI）、横向きで使用、PWM バックライト |
| タッチ | CST816。割り込み線がないため、ポーリングで読みます |
| オーディオ | ES8311 DAC と ES7210 ADC |
| キー | 上面にあります。**+**（音量 +）：押している間話す。**−**（音量 −）：押すと画面オフ、長押しで電源オフ。中央のキーは使いません |
| バッテリー | 元のファームウェアの ADC テーブルによる残量と、充電状態のピン |

**状況：** ESP-IDF v6.0.1 でビルドできます。実機ではまだ動かしていません。
ピン、パネルの初期化テーブルと向きは、そのファームウェアのソースによるもので、
照らし合わせる資料はありません。画面が回転または反転している場合や、タッチの
位置がずれる場合は、ボードのコードの先頭にある `LCD_MADCTL`、または
`TP_SWAP_XY`、`TP_MIRROR_X`、`TP_MIRROR_Y` を変えてください。

既知の制限：4G モデムと LED は使いません。バッテリーは残量だけを表示し、電圧は
表示しません。電源オフはボードの電源オフのピンを駆動します。USB 給電中は電源が
切れないことがあり、その場合は + キーを押すまで画面が消えたままになります。
満充電になった後は、パソコンにつないでいる間だけ USB 給電中と判定します。

コード：[`board_ostb_3st.c`](esp32/components/muse/boards/board_ostb_3st.c)、
ビルド設定：[`sdkconfig.muse-ostb-3st`](esp32/devices/sdkconfig.muse-ostb-3st)。

## LCDWIKI 2.8inch ESP32-S3 Display

<p align="center">
  <img src="doc/image/lcdwiki-s3-28-device.jpg" width="330" alt="LCDWIKI 2.8inch ESP32-S3 Display の前面">
  <img src="doc/image/lcdwiki-s3-28-back.jpg" width="330" alt="背面。コネクターとボタンの説明付き">
</p>
<p align="center">
  <img src="doc/image/lcdwiki-s3-28.png" width="720" alt="シミュレーターで描いた 320×240 の Muse UI">
</p>

さまざまな名前で売られている 2.8 インチのディスプレイボードです。LCDWIKI には
静電容量タッチ付きの **ES3C28P** と、タッチなしの **ES3N28P** の 2 モデルが
あります。1 つのファームウェアで両方に対応します。起動時にタッチコントローラー
の有無を調べます。

| 項目 | 内容 |
|---|---|
| チップ | ESP32-S3R8、16 MB フラッシュ、8 MB オクタル PSRAM、ネイティブ USB |
| ディスプレイ | 2.8 インチ 240×320 ILI9341V LCD（SPI、40 MHz）、横向きで使用、PWM バックライト |
| タッチ | ES3C28P は FT6336G。ES3N28P にはありません |
| オーディオ | ES8311 コーデックとマイク 1 個、FM8002E スピーカーアンプ |
| ボタン | BOOT：押している間だけ話す |
| バッテリー | 元のファームウェアの ADC 範囲による残量（GPIO9） |

**状況：** ESP-IDF v6.0.1 でビルドできます。実機ではまだ動かしていません。
ピンは [LCDWIKI のページ](https://www.lcdwiki.com/2.8inch_ESP32-S3_Display)とこのボードの xiaozhi-esp32 のファイルによる
ものです。タッチパネルの向きと SPI の 40 MHz という上限は、
[esphome-es3c28p-light-panel](https://github.com/jvduuren/esphome-es3c28p-light-panel) が実機で測定した結果に従っています。タッチの
位置がずれる場合は、ボードのコードの先頭にある `TP_SWAP_XY`、`TP_MIRROR_X`、
`TP_MIRROR_Y` を変えてください。

既知の制限：ボタンは BOOT だけなので、画面はタイマーで消灯し、電源オフは
Settings の Power ページから行います（ボードはディープスリープに入り、BOOT を
押すと復帰します）。ES3N28P はタッチがなくボタンも 1 つなので、デバイス上に設定
画面はありません。プッシュトゥトークだけができ、設定は Muse アプリから行います。
バッテリーは残量だけを表示し、電圧は表示しません。充電 IC の状態はどのピンにも
つながっていないため、「充電中」と表示されるのはパソコンに接続している間だけ
です。SD カードスロットと RGB LED は使いません。

コード：[`board_lcdwiki_s3_28.c`](esp32/components/muse/boards/board_lcdwiki_s3_28.c)、
ビルド設定：[`sdkconfig.muse-lcdwiki-s3-28`](esp32/devices/sdkconfig.muse-lcdwiki-s3-28)。

## ビルドと書き込み

手順はどのボードでも同じです。名前とプロファイルは
[上の表](#このフォークのボード)を見てください。

1. [SDK トークン](https://gadgets.muse.ai/settings/sdk-tokens)を取得し、
   [Gadget SDK 規約](https://gadgets.muse.ai/sdk-terms)を読みます。ペアリング
   にはどの gadget でもトークンが必要です。
2. ESP-IDF **v6.0.1** をインストールします（[`esp32/README.md`](esp32/README.md)
   を参照）。
3. `esp32/` でビルドします。macOS または Linux では、ボードの名前を使います。

   ```sh
   tools/muse/board.sh build ostb
   ```

   Windows では ESP-IDF PowerShell で、ボードのプロファイルを使います。

   ```powershell
   $P = "ostb-3st"
   idf.py -B build-muse-$P -DIDF_TARGET=esp32s3 `
     "-DSDKCONFIG=build-muse-$P/sdkconfig" `
     "-DSDKCONFIG_DEFAULTS=sdkconfig.defaults;devices/sdkconfig.muse;devices/sdkconfig.muse-$P" build
   ```

   トークンは `idf.py -B build-muse-$P menuconfig`
   （ESP32 Device SDK > Muse Gadgets SDK token）で設定し、もう一度ビルドします。
4. USB-C で書き込みます。`PORT` はボードのポートに置き換えてください。

   ```powershell
   idf.py -B build-muse-$P -p PORT flash
   ```

   接続できない場合は、ボードをダウンロードモードにしてからもう一度試します。
   **BOOT** を押したまま **RESET** を押して離し、**BOOT** を離します。RESET
   ボタンのないボードでは、BOOT を押したままケーブルを差し込みます。

   Web の書き込みツールを使う場合は、1 つのファイルにまとめてアドレス `0x0` に
   書き込みます。

   ```powershell
   idf.py -B build-muse-$P merge-bin -o muse-gadget-$P-merged.bin
   ```

   自分の SDK トークンでビルドしたファームウェアにはそのトークンが含まれます。
   ファイルを公開する前によく考えてください。
5. Muse アプリで **Settings > Devices > Developer mode** をオンにし、
   `MuseGadget-XXXXXX` という名前のデバイスを追加して、求められたらボードの
   トークボタンを押します（1.85C と LCDWIKI のボードでは BOOT、OSTB-3ST では + キー）。

## ほかのデバイスを追加する

画面、スピーカー、マイクのあるボードなら、ドライバーのファイル 1 つと数行の登録で
足ります。このフォークの各ボードは次のものでできています。

1. `esp32/components/muse/boards/board_<id>.c` が `muse_board_t`
   （`muse_board.h`）を埋めます。ディスプレイ、タッチ、オーディオ、ボタン、
   バッテリー、電源です。自分のボードにいちばん近いボードから始めてください。
2. `esp32/devices/sdkconfig.muse-<profile>` にビルド設定を置きます。チップ、
   フラッシュ、PSRAM です。
3. `esp32/components/muse/Kconfig`、`CMakeLists.txt`、`idf_component.yml` に
   ボードと、必要なドライバーコンポーネントを登録します。
4. `esp32/tools/muse/board.sh`、`ports.py`、`avatar.py` に短い名前を加えます。
5. ドキュメントにも加えます。各 README の表の行とセクション、
   [`esp32/devices/README.md`](esp32/devices/README.md) の行、`doc/image/` の
   画像、そして [`CREDITS.md`](CREDITS.md) に参考にしたソースとそのライセンス
   です。

UI は円形の画面にも長方形の画面にも合わせて配置されます。上の画像は
`esp32/simulator` のシミュレーターで、`src/sim_board.c` の画面サイズをボードの
サイズに変えて描いたものです。ボードが手元に届く前に、新しいサイズでの見え方を
確かめられます。ボードの情報集めから結果の確認までの手順は
[`esp32/devices/AGENTS.md`](esp32/devices/AGENTS.md)（英語）にあります。

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
にあります。このフォークで追加したボードについては、このリポジトリに issue を
立ててください。

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
- OSTB-3ST への移植では、ピン、パネルの初期化テーブル、バッテリーのテーブルを
  このボードの xiaozhi-esp32 ファームウェアのソースから取っています。その
  ファイル自体にライセンス表記はありません。xiaozhi-esp32 は MIT です。
- LCDWIKI 2.8inch への移植は、[LCDWIKI のページ](https://www.lcdwiki.com/2.8inch_ESP32-S3_Display)、このボードの
  xiaozhi-esp32 のファイル、および
  [esphome-es3c28p-light-panel](https://github.com/jvduuren/esphome-es3c28p-light-panel) で公開されている実測結果を参考にして
  います。コードはコピーしていません。
- 上流のファイルのうち 2 つは独自のライセンスのままです：`minimp3.h`
  （CC0-1.0）と `pixel_font.c`（BSD-2-Clause）。ビルド時に取得する
  コンポーネントには、それぞれのライセンスが適用されます。
- DB_ROBOT の起動ロゴは ledienbien-ai のもので、このフォークのほかの変更と同じ
  ライセンスで自由に使えます。
- Apache License は [Jollybot アバター](esp32/avatar)には適用されません。Meta、
  Muse、Waveshare の名称と商標にも適用されません。

完全な一覧は [`CREDITS.md`](CREDITS.md)（英語）にあります。各ソースとその
ライセンス、このフォークで追加または変更したファイル、今後のバージョンで守る
べきことをまとめています。

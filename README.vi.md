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

# Muse Gadgets cho Waveshare ESP32-S3-Touch-LCD-1.85C

[English](README.md) | **Tiếng Việt** | [简体中文](README.zh-CN.md) | [日本語](README.ja.md) | [한국어](README.ko.md)

> Đây là bản fork không chính thức do cộng đồng thực hiện từ
> [facebookincubator/muse-gadget-sdk](https://github.com/facebookincubator/muse-gadget-sdk).
> Bản này không liên kết với Meta hay Waveshare và không được hai bên bảo trợ.
> Nếu bản dịch khác với [bản tiếng Anh](README.md), bản tiếng Anh là bản chuẩn.

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/images/muse-gadgets-dark.png">
    <img src=".github/images/muse-gadgets-light.png" width="900" alt="Các Muse gadget: màn AMOLED tròn của Waveshare, M5Stack StickS3, Muse Home Link, Raspberry Pi và màn e-ink Seeed reTerminal">
  </picture>
</p>

Muse gadget là những thiết bị mã nguồn mở do bạn tự làm: nạp chương trình cho
một bo ESP32 có sẵn trên thị trường hoặc cài đặt một chiếc Raspberry Pi, rồi
kết nối Muse với màn hình, nút bấm, cảm biến và cơ cấu chấp hành của bạn. Bản
fork này thêm vào ESP32 Device SDK một bo nữa là bo màn tròn
[Waveshare ESP32-S3-Touch-LCD-1.85C](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C),
còn phần còn lại của SDK giữ nguyên như bản gốc.

Dự án do dân vọc vạch làm cho dân vọc vạch, chỉ để cho vui. Nạp firmware tùy
biến có thể làm hỏng bo và mất bảo hành. Bạn tự chịu rủi ro!

## Bản fork này thêm gì

Bo chạy đầy đủ giao diện trên màn hình: avatar động, nhấn giữ để nói, cài đặt
bằng cảm ứng và hiển thị ảnh do Muse gửi.

| Thành phần | Chi tiết |
|---|---|
| Chip | ESP32-S3R8, flash 16 MB, PSRAM octal 8 MB, USB gốc |
| Màn hình | LCD tròn 1,85" 360×360 ST77916 qua QSPI, đèn nền PWM |
| Cảm ứng | CST816 |
| Âm thanh, bo V1 | DAC PCM5101 và micro số I2S |
| Âm thanh, bo V2 ("Rev2.0") | DAC ES8311 và ADC ES7210 với hai micro |
| Nút | BOOT: nhấn giữ để nói |
| Pin | Đo điện áp qua cầu chia trên GPIO8 |

Một firmware dùng cho cả hai phiên bản âm thanh: lúc khởi động nó dò xem có
ES8311 hay không.

**Tình trạng:** biên dịch được với ESP-IDF v6.0.1. Firmware chưa được kiểm
chứng trên bo thật, và đường âm thanh V2 được viết theo mã mẫu của Waveshare
khi chưa có bo V2 để thử.

Giới hạn đã biết: BOOT là nút duy nhất, nên màn hình tự tắt theo hẹn giờ và
việc tắt máy nằm ở trang Power trong Settings (bo vào deep sleep cho tới khi
nhấn BOOT; công tắc trượt mới ngắt nguồn). Trạng thái của mạch sạc không nối về
chân nào, nên bo chỉ báo "đang sạc" khi đang cắm vào máy tính.

## Biên dịch và nạp

1. Lấy [SDK token](https://gadgets.muse.ai/settings/sdk-tokens) và đọc
   [Điều khoản Gadget SDK](https://gadgets.muse.ai/sdk-terms). Gadget nào cũng
   cần token mới ghép đôi được.
2. Cài ESP-IDF **v6.0.1** (xem [`esp32/README.md`](esp32/README.md)).
3. Biên dịch từ thư mục `esp32/`. Trên macOS hoặc Linux:

   ```sh
   tools/muse/board.sh build s3lcd
   ```

   Trên Windows, trong ESP-IDF PowerShell:

   ```powershell
   idf.py -B build-muse-waveshare-s3-185c -DIDF_TARGET=esp32s3 `
     -DSDKCONFIG=build-muse-waveshare-s3-185c/sdkconfig `
     "-DSDKCONFIG_DEFAULTS=sdkconfig.defaults;devices/sdkconfig.muse;devices/sdkconfig.muse-waveshare-s3-185c" build
   ```

   Đặt token bằng `idf.py -B build-muse-waveshare-s3-185c menuconfig`
   (ESP32 Device SDK > Muse Gadgets SDK token), rồi biên dịch lại.
4. Nạp qua USB-C, thay `PORT` bằng cổng của bo. Nếu không kết nối được, giữ
   **BOOT**, nhấn **RESET**, thả **BOOT** rồi thử lại.

   ```sh
   idf.py -B build-muse-waveshare-s3-185c -p PORT flash
   ```

   Để nạp qua web, tạo một file duy nhất và ghi ở địa chỉ `0x0`:

   ```sh
   idf.py -B build-muse-waveshare-s3-185c merge-bin -o muse-gadget-185c-merged.bin
   ```

   Bản build có SDK token của bạn sẽ mang theo token đó, nên hãy cân nhắc trước
   khi đăng file công khai.
5. Trong ứng dụng Muse, bật **Settings > Devices > Developer mode**, thêm thiết
   bị tên `MuseGadget-XXXXXX`, rồi nhấn **BOOT** khi được hỏi.

Mã của bo nằm trong
[`board_waveshare_s3_185c.c`](esp32/components/muse/boards/board_waveshare_s3_185c.c),
còn cấu hình build nằm trong
[`sdkconfig.muse-waveshare-s3-185c`](esp32/devices/sdkconfig.muse-waveshare-s3-185c).
[`esp32/devices/AGENTS.md`](esp32/devices/AGENTS.md) giải thích cách thêm một bo
mới (tiếng Anh).

## Phần còn lại của SDK

| | |
|---|---|
| [**ESP32 Device SDK**](esp32) | Kết nối bo ESP32 của bạn với Muse. Gắn thêm màn hình để hiện ảnh, thêm âm thanh vào ra, hoặc nối các cảm biến khác. |
| [**Linux Device SDK**](linux) | Biến chiếc Raspberry Pi hay máy Linux để không thành một Muse gadget. Tự thêm lệnh của bạn để Muse lo việc quản trị hệ thống hoặc hệ Home Assistant. |

Gadget ESP32 và Linux ghép đôi với ứng dụng Muse trên iOS và Android, trong
Settings > Devices. Bật Developer mode ở đó trước, rồi tìm thiết bị có tên bắt
đầu bằng "MuseGadget". Mỗi thư mục có một `README.md` để bắt đầu và một
`AGENTS.md` dành cho các trợ lý lập trình (đều bằng tiếng Anh).

## Cộng đồng

Cộng đồng của dự án gốc trao đổi trên
[Discord](https://discord.gg/3bhjCkZdd6). Với riêng bo này, hãy mở issue trên
repo này.

## Giấy phép và ghi công

- Muse Gadget SDK thuộc bản quyền của Meta Platforms, Inc. và các công ty liên
  kết, phát hành theo Giấy phép Apache, Phiên bản 2.0, trong
  [`LICENSE`](LICENSE).
- Các thay đổi trong bản fork này thuộc bản quyền 2026 của ledienbien-ai, theo
  cùng giấy phép đó.
- Bản port 1.85C dựa trên tài liệu và
  [mã mẫu](https://github.com/waveshareteam/ESP32-S3-Touch-LCD-1.85C) của
  Waveshare (Apache-2.0) và trên phần board tương ứng của
  [xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) (MIT).
- Hai file của bản gốc giữ giấy phép riêng: `minimp3.h` (CC0-1.0) và
  `pixel_font.c` (BSD-2-Clause). Các component tải về lúc build theo giấy phép
  của chính chúng.
- Giấy phép Apache không bao gồm [avatar Jollybot](esp32/avatar), cũng không
  bao gồm tên và nhãn hiệu Meta, Muse và Waveshare.

[`CREDITS.md`](CREDITS.md) (tiếng Anh) có danh sách đầy đủ: từng nguồn, giấy
phép của nguồn đó, các file bản fork này thêm hoặc sửa, và những việc cần giữ
đúng ở các phiên bản sau.

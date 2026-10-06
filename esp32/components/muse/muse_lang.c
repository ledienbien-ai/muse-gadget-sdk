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

/* The screen's text in Vietnamese (muse_lang.h). Nothing here touches the
 * hardware or LVGL, so the host tests run it. */
#include "muse_lang.h"

#include <stddef.h>
#include <string.h>

#include "muse_text.h"

/*
 * English as the code writes it, and the Vietnamese shown for it. Text in
 * capitals is the face's (the state word and captions); a format string keeps
 * its conversions in the same order. tests/test_muse_lang.py checks that.
 */
static const struct {
    const char *en;
    const char *vi;
} TEXTS[] = {
    /* The state word. */
    { "WAKING UP", "KHỞI ĐỘNG" },
    { "READY", "SẴN SÀNG" },
    { "LISTENING", "ĐANG NGHE" },
    { "THINKING", "ĐANG NGHĨ" },
    { "SPEAKING", "ĐANG NÓI" },
    { "ERROR", "LỖI" },
    { "GOODBYE", "TẠM BIỆT" },
    { "WI-FI OFF", "WI-FI TẮT" },
    { "SET UP WI-FI", "CÀI WI-FI" },
    { "NO WI-FI", "KHÔNG CÓ WI-FI" },
    { "RECONNECTING", "KẾT NỐI LẠI" },
    { "CONNECTING", "ĐANG KẾT NỐI" },

    /* Captions. */
    { "WAKING UP...", "ĐANG KHỞI ĐỘNG..." },
    { "LISTENING...", "ĐANG NGHE..." },
    { "RECORDING...", "ĐANG GHI ÂM..." },
    { "RECORDING", "GHI ÂM" },
    { "SENDING VOICE NOTE", "ĐANG GỬI LỜI NHẮN" },
    { "SENDING SAVED NOTE", "ĐANG GỬI LỜI NHẮN ĐÃ LƯU" },
    { "SAVED NOTE SENT", "ĐÃ GỬI LỜI NHẮN ĐÃ LƯU" },
    { "COULDN'T SEND A SAVED NOTE", "KHÔNG GỬI ĐƯỢC LỜI NHẮN ĐÃ LƯU" },
    { "SAVED NOTE: WILL TRY AGAIN", "LỜI NHẮN ĐÃ LƯU: SẼ THỬ LẠI" },
    { "COULDN'T SAVE THE NOTE", "KHÔNG LƯU ĐƯỢC LỜI NHẮN" },
    { "SAVED, WILL TRY AGAIN", "ĐÃ LƯU, SẼ THỬ LẠI" },
    { "SAVED, SENDS WHEN ONLINE", "ĐÃ LƯU, GỬI KHI CÓ MẠNG" },
    { "NOTES STILL WAITING TO SEND", "CÒN LỜI NHẮN CHỜ GỬI" },
    { "HOLD LONGER TO TALK", "GIỮ LÂU HƠN ĐỂ NÓI" },
    { "SET UP MUSE FIRST", "HÃY CÀI ĐẶT MUSE TRƯỚC" },
    { "CAN'T REACH MUSE", "KHÔNG KẾT NỐI ĐƯỢC MUSE" },
    { "MUSE NOT SET UP", "CHƯA CÀI ĐẶT MUSE" },
    { "INTERRUPTED", "BỊ NGẮT" },
    { "DIDN'T CATCH THAT", "CHƯA NGHE RÕ" },
    { "NO REPLY FROM MUSE", "MUSE KHÔNG TRẢ LỜI" },
    { "MUSE DIDN'T TAKE IT", "MUSE KHÔNG NHẬN" },
    { "MUSE STOPPED LISTENING", "MUSE ĐÃ NGỪNG NGHE" },
    { "MUSE COULDN'T LISTEN", "MUSE KHÔNG NGHE ĐƯỢC" },
    { "LOST CONNECTION TO MUSE", "MẤT KẾT NỐI VỚI MUSE" },
    { "SETTINGS CHANGED", "CÀI ĐẶT ĐÃ ĐỔI" },
    { "CANCELLED", "ĐÃ HỦY" },
    { "AUDIO INIT FAILED", "LỖI KHỞI TẠO ÂM THANH" },
    { "GOODBYE!", "TẠM BIỆT!" },
    { "COULDN'T POWER OFF", "KHÔNG TẮT NGUỒN ĐƯỢC" },
    { "HOLD TO POWER OFF", "GIỮ ĐỂ TẮT NGUỒN" },
    { "PHONE SETUP: %s", "CÀI TỪ ĐIỆN THOẠI: %s" },
    { "PHONE SETUP %s", "CÀI TỪ ĐIỆN THOẠI %s" },
    { "ON", "BẬT" },
    { "OFF", "TẮT" },
    { "SPEAKER ON", "ĐÃ BẬT LOA" },
    { "SPEAKER OFF", "ĐÃ TẮT LOA" },
    { "HOLD TO MUTE", "GIỮ ĐỂ TẮT LOA" },
    { "HOLD TO UNMUTE", "GIỮ ĐỂ BẬT LOA" },
    { "RESETTING...", "ĐANG ĐẶT LẠI..." },

    /* The status line. */
    { "USB POWER", "Nguồn USB" },
    { "CHARGING %d%%", "Sạc %d%%" },
    { "BATTERY %d%%", "Pin %d%%" },

    /* The pairing card. */
    { "Pairing code", "Mã ghép đôi" },
    { "Enter it on your phone", "Nhập mã trên điện thoại" },
    { "Press button", "Nhấn nút" },
    { "Press the %s button", "Nhấn nút %s" },
    { "Pair with Muse app", "Ghép với ứng dụng Muse" },

    /* Settings: home. */
    { "SETTINGS", "CÀI ĐẶT" },
    { "Sound", "Âm thanh" },
    { "Sleep", "Tắt màn hình" },
    { "Battery", "Pin" },
    { "Language", "Ngôn ngữ" },
    { "Power off", "Tắt nguồn" },
    { "Off", "Tắt" },
    { "On", "Bật" },
    { "Not set", "Chưa đặt" },
    { "Joining", "Đang vào" },
    { "Failed", "Lỗi" },
    { "Not nearby", "Không ở gần" },
    { "Connected", "Đã kết nối" },
    { "Vol %d%%", "Âm lượng %d%%" },
    { "Muted", "Tắt tiếng" },
    { "offline", "ngoại tuyến" },

    /* Settings: Wi-Fi. */
    { "Saved networks", "Mạng đã lưu" },
    { "Tap to forget", "Chạm để quên" },
    { "Hidden", "Ẩn" },
    { "saved  ", "đã lưu  " },
    { "open  ", "mở  " },
    { "No networks found", "Không tìm thấy mạng nào" },
    { "Scan for networks", "Quét mạng" },
    { "Scanning...", "Đang quét..." },
    { "Other network...", "Mạng khác..." },
    { "Other network", "Mạng khác" },
    { "Network name", "Tên mạng" },
    { "Password", "Mật khẩu" },
    { "Empty if it's open", "Để trống nếu mạng mở" },
    { "MAC address", "Địa chỉ MAC" },
    { "Muse remembers up to 8 networks and joins the strongest one in range. Tap a saved one twice to forget it.",
      "Muse nhớ tối đa 8 mạng và tự vào mạng mạnh nhất trong tầm. Chạm hai lần vào một mạng đã lưu để quên nó." },
    { "Wi-Fi is off", "Wi-Fi đang tắt" },
    { "No saved networks. Scan and pick one.", "Chưa lưu mạng nào. Hãy quét rồi chọn một mạng." },
    { "Joining %s\n%s", "Đang vào %s\n%s" },
    { "Connected to %s\n%s  -  %d dBm", "Đã kết nối %s\n%s  -  %d dBm" },
    { "No saved network nearby\nLooking again within a minute",
      "Không có mạng đã lưu nào ở gần\nSẽ tìm lại trong vòng một phút" },
    { "Couldn't join %s\n%s", "Không vào được %s\n%s" },
    { "Joining...", "Đang vào..." },
    { "No saved network nearby", "Không có mạng đã lưu nào ở gần" },
    { "Can't join; retrying", "Không vào được; đang thử lại" },

    /* Settings: Muse. */
    { "Reset pairing", "Đặt lại ghép đôi" },
    { "Resetting...", "Đang đặt lại..." },
    { "Tap again to reset", "Chạm lần nữa để đặt lại" },
    { "Server", "Máy chủ" },
    { "Device token", "Token thiết bị" },
    { "Test connection", "Thử kết nối" },
    { "Pair with the Muse app to use your account; a device token here overrides it, and a long one is "
      "easier to send over Bluetooth. The VM ID picks one of your VMs. "
      "Reset pairing forgets Wi-Fi and the app pairing, then restarts.",
      "Ghép đôi với ứng dụng Muse để dùng tài khoản của bạn; token thiết bị nhập ở đây sẽ được ưu tiên, "
      "token dài thì gửi qua Bluetooth dễ hơn. VM ID chọn một trong các VM của bạn. "
      "Đặt lại ghép đôi sẽ quên Wi-Fi và việc ghép với ứng dụng, rồi khởi động lại." },
    { "Muse app: %s\n%s", "Ứng dụng Muse: %s\n%s" },
    { "paired", "đã ghép" },
    { "not paired", "chưa ghép" },
    { "Set (%u chars)", "Đã đặt (%u ký tự)" },
    { "Muse server", "Máy chủ Muse" },
    { "Empty for the default", "Để trống để dùng mặc định" },
    { "Optional", "Không bắt buộc" },
    { "Empty keeps the current one", "Để trống để giữ token cũ" },
    /* The states of the app pairing and of Muse's own session, and their details. */
    { "Starting", "Đang khởi động" },
    { "Starting...", "Đang khởi động..." },
    { "Ready to pair", "Sẵn sàng ghép đôi" },
    { "App connected", "Ứng dụng đã kết nối" },
    { "Confirm pairing", "Xác nhận ghép đôi" },
    { "Connecting", "Đang kết nối" },
    { "Connecting...", "Đang kết nối..." },
    { "Online", "Trực tuyến" },
    { "Offline", "Ngoại tuyến" },
    { "Error", "Lỗi" },
    { "Not set up", "Chưa cài đặt" },
    { "Saved", "Đã lưu" },
    { "Can't connect", "Không kết nối được" },
    { "Pair in the Muse app", "Hãy ghép đôi trong ứng dụng Muse" },
    { "Waiting for Wi-Fi", "Đang chờ Wi-Fi" },
    { "Connects when you talk", "Sẽ kết nối khi bạn nói" },
    { "Connected to ", "Đã kết nối tới " },
    { "Can't reach Muse's server", "Không tới được máy chủ Muse" },
    { "Not paired", "Chưa ghép đôi" },
    { "Out of memory", "Hết bộ nhớ" },
    { "Token rejected", "Token bị từ chối" },
    { "VM refused the token", "VM từ chối token" },
    { "Noise handshake failed", "Bắt tay Noise thất bại" },
    { "Socket error", "Lỗi kết nối" },
    { "Subscribe failed", "Không đăng ký nhận tin được" },

    /* Settings: Bluetooth. */
    { "Phone setup", "Cài từ điện thoại" },
    { "Forget paired phones", "Quên điện thoại đã ghép" },
    { "When on, Muse is visible to phones nearby. Open tools/ble_setup.html in Chrome, "
      "connect, and enter the code Muse shows to pair.",
      "Khi bật, điện thoại ở gần sẽ thấy Muse. Mở tools/ble_setup.html bằng Chrome, "
      "kết nối rồi nhập mã Muse hiển thị để ghép đôi." },
    { "Visible as %s", "Hiển thị với tên %s" },
    { "Phone connected\n%s", "Điện thoại đã kết nối\n%s" },
    { "Paired", "Đã ghép đôi" },
    { "Waiting for pairing", "Đang chờ ghép đôi" },

    /* Settings: sound. */
    { "SOUND", "ÂM THANH" },
    { "Speaker", "Loa" },
    { "Volume", "Âm lượng" },
    { "Mic gain", "Độ nhạy mic" },
    { "Mic level", "Mức mic" },
    { "Brightness", "Độ sáng" },
    { "Speaker on, Muse's replies are read aloud by Google Translate, which is sent their text. "
      "Speaker off, they're only shown.",
      "Khi bật loa, câu trả lời của Muse được đọc thành tiếng bằng Google Dịch (văn bản câu trả lời "
      "được gửi tới Google). Khi tắt loa, câu trả lời chỉ hiện chữ." },
    { "Talk at arm's length: the bar should reach green (-30 to -15 dBFS) without going orange.",
      "Nói cách một cánh tay: thanh nên lên tới màu xanh lá (-30 đến -15 dBFS) mà không chuyển sang cam." },

    /* Settings: sleep. */
    { "AUTO-SLEEP", "TẮT MÀN HÌNH" },
    { "Turn the screen off after Muse has been idle for:", "Tắt màn hình sau khi Muse nghỉ được:" },
    { "Never", "Không bao giờ" },
    { "30 seconds", "30 giây" },
    { "1 minute", "1 phút" },
    { "2 minutes", "2 phút" },
    { "5 minutes", "5 phút" },
    { "10 minutes", "10 phút" },
    { "Custom", "Tùy chỉnh" },
    { "Sleep now", "Tắt màn hình ngay" },
    { "Tap the screen or press either button to wake.", "Chạm màn hình hoặc nhấn nút bất kỳ để đánh thức." },

    /* Settings: battery. */
    { "BATTERY", "PIN" },
    { "Used", "Đã dùng" },
    { "A full charge", "Sạc đầy" },
    { "Screen off", "Màn hình tắt" },
    { "Chip asleep", "Chip ngủ" },
    { "Wakes", "Số lần thức" },
    { "CPU busy", "CPU bận" },
    { "Start over", "Đo lại từ đầu" },
    { "Measures from unplugging USB until it's plugged back in. The gauge moves in 1% steps, so give it a "
      "few hours. Chip asleep is time in light sleep; CPU busy is time a core was running a task.",
      "Đo từ lúc rút USB tới khi cắm lại. Mức pin nhảy từng 1%, nên hãy chờ vài giờ. "
      "Chip ngủ là thời gian ngủ nhẹ; CPU bận là thời gian một nhân đang chạy tác vụ." },
    { "%d h %d min", "%d giờ %d phút" },
    { "%d min", "%d phút" },
    { "No battery", "Không có pin" },
    { "Unplug USB to start measuring.", "Rút USB để bắt đầu đo." },
    { "On battery for %s", "Đã chạy pin %s" },
    { "Last run: %s on battery", "Lần trước: chạy pin %s" },
    { "None", "Không có" },
    { "%d%%, %d.%d%%/h", "%d%%, %d.%d%%/giờ" },
    { "lasts ~%d h", "dùng ~%d giờ" },
    { "%d%% so far", "%d%% đến giờ" },
    { "measuring", "đang đo" },
    { "Also kept awake by: %s", "Còn bị giữ thức bởi: %s" },

    /* Settings: power. */
    { "POWER", "NGUỒN" },
    { "Power Muse off completely?", "Tắt hẳn Muse?" },
    { "Cancel", "Hủy" },
    { "Press the %s button to turn it back on. To just turn the screen off, press the %s button.",
      "Nhấn nút %s để bật lại. Nếu chỉ muốn tắt màn hình, hãy nhấn nút %s." },

    /* Settings: language. */
    { "LANGUAGE", "NGÔN NGỮ" },
    { "Muse restarts to change the language. In English, replies in Vietnamese are shown "
      "without their accents.",
      "Muse sẽ khởi động lại để đổi ngôn ngữ. Khi chọn English, câu trả lời tiếng Việt sẽ hiện không dấu." },
    { "A reply written in Vietnamese is spoken in a Vietnamese voice either way; any other reply in the "
      "language chosen.",
      "Câu trả lời viết bằng tiếng Việt luôn được đọc bằng giọng tiếng Việt; các câu trả lời khác được đọc "
      "theo ngôn ngữ đã chọn." },
    { "Restarting...", "Đang khởi động lại..." },

    /* Typing text. */
    { "Show", "Hiện" },
    { "Hide", "Ẩn" },
};

static muse_lang_t s_lang = MUSE_LANG_EN;

void muse_lang_set(muse_lang_t lang)
{
    s_lang = lang < MUSE_LANG_COUNT ? lang : MUSE_LANG_EN;
    muse_text_keep_vietnamese(s_lang == MUSE_LANG_VI);
}

muse_lang_t muse_lang(void)
{
    return s_lang;
}

const char *muse_lang_code(muse_lang_t lang)
{
    return lang == MUSE_LANG_VI ? "vi" : "en";
}

bool muse_lang_from_code(const char *code, muse_lang_t *lang)
{
    for (int i = 0; i < MUSE_LANG_COUNT; i++) {
        if (!strcmp(code, muse_lang_code((muse_lang_t)i))) {
            *lang = (muse_lang_t)i;
            return true;
        }
    }
    return false;
}

const char *muse_lang_name(muse_lang_t lang)
{
    return lang == MUSE_LANG_VI ? "Tiếng Việt" : "English";
}

const char *muse_tr(const char *en)
{
    if (s_lang != MUSE_LANG_VI || !en || !en[0]) {
        return en;
    }
    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++) {
        if (!strcmp(TEXTS[i].en, en)) {
            return TEXTS[i].vi;
        }
    }
    return en;
}

bool muse_lang_text(size_t i, const char **en, const char **vi)
{
    if (i >= sizeof(TEXTS) / sizeof(TEXTS[0])) {
        return false;
    }
    *en = TEXTS[i].en;
    *vi = TEXTS[i].vi;
    return true;
}

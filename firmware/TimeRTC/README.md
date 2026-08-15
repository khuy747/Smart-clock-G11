# TimeRTC

Thư viện quản lý thời gian cho ESP32 sử dụng module **DS3231 RTC**.

Thư viện được thiết kế để tách phần xử lý RTC khỏi các module khác trong hệ thống đồng hồ.

## Chức năng

- Khởi tạo và giao tiếp với DS3231 qua I2C.
- Nhận ngày giờ từ Web/App và ghi vào DS3231.
- Nhận ngày giờ sau khi người dùng chỉnh bằng núm xoay + nút bấm.
- Đọc thời gian thực liên tục từ DS3231.
- Cung cấp dữ liệu thời gian cho TFT Display, Alarm, Buzzer và các module khác.
- DS3231 tiếp tục đếm thời gian khi ESP32 không kết nối WiFi.

---

# 1. Phần cứng

## DS3231

| DS3231 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 8 |
| SCL | GPIO 9 |
| SQW | Không sử dụng |
| 32K | Không sử dụng |

> Chân SDA và SCL có thể thay đổi tùy theo cấu hình ESP32. 
> Trong ví dụ của thư viện sử dụng SDA = GPIO 8 và SCL = GPIO 9.

---

# 2. Thư viện phụ thuộc

Cần cài:

- RTClib by Adafruit
- Adafruit BusIO

Arduino IDE:

```text
Tools
→ Manage Libraries
→ Search: RTClib
→ Install

# TimeRTC

Thư viện quản lý thời gian cho **ESP32 + DS3231 RTC**.

## Chức năng

- Khởi tạo DS3231.
- Đọc thời gian thực.
- Cài đặt ngày giờ.
- Trả về ngày, tháng, năm, giờ, phút, giây.
- Có thể nhận thời gian từ Web/App hoặc Input rồi ghi vào DS3231.
- DS3231 tiếp tục đếm thời gian khi ESP32 hoặc WiFi bị ngắt.

---

## Phần cứng

### Kết nối DS3231 với ESP32

| DS3231 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 8 |
| SCL | GPIO 9 |
| 32K | Không sử dụng |
| SQW | Không sử dụng |

DS3231 có **pin backup**, vì vậy sau khi đã cài thời gian, RTC vẫn có thể tiếp tục đếm khi ESP32 mất nguồn.

---

## Thư viện cần cài

Cài thư viện:

```text
RTClib by Adafruit
```

Trong Arduino IDE:

```text
Tools
→ Manage Libraries
→ Search "RTClib"
→ Install
```

`Adafruit BusIO` sẽ được cài cùng nếu chưa có.

---

## Sử dụng

Include thư viện:

```cpp
#include <TimeRTC.h>
```

Tạo RTC:

```cpp
TimeRTC rtc;
```

---

## 1. Khởi tạo RTC

```cpp
rtc.begin(SDA, SCL);
```

Ví dụ:

```cpp
rtc.begin(8, 9);
```

Có thể kiểm tra DS3231:

```cpp
if (!rtc.begin(8, 9))
{
    Serial.println("DS3231 NOT FOUND!");
}
```

---

## 2. Cài đặt ngày giờ

```cpp
rtc.setDateTime(
    year,
    month,
    day,
    hour,
    minute,
    second
);
```

Ví dụ:

```cpp
rtc.setDateTime(
    2026,
    8,
    16,
    19,
    30,
    0
);
```

RTC sẽ bắt đầu chạy từ:

```text
16/08/2026 19:30:00
```

Sau đó DS3231 tự động đếm:

```text
19:30:00
19:30:01
19:30:02
19:30:03
...
```

---

## 3. Lấy thời gian

```cpp
TimeData t = rtc.getTime();
```

Sau đó lấy từng giá trị:

```cpp
t.second
t.minute
t.hour

t.day
t.month
t.year
```

Ví dụ:

```cpp
TimeData t = rtc.getTime();

Serial.printf(
    "%02d/%02d/%04d %02d:%02d:%02d\n",
    t.day,
    t.month,
    t.year,
    t.hour,
    t.minute,
    t.second
);
```

Output:

```text
16/08/2026 19:30:01
16/08/2026 19:30:02
16/08/2026 19:30:03
```

---

## 4. TimeData

`getTime()` trả về:

```cpp
struct TimeData
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;

    uint8_t day;
    uint8_t month;
    uint16_t year;
};
```

---

## 5. Cài thời gian từ Web hoặc nút bấm

Web/Input chỉ cần gọi:

```cpp
rtc.setDateTime(
    year,
    month,
    day,
    hour,
    minute,
    second
);
```

Ví dụ:

```cpp
rtc.setDateTime(
    selectedYear,
    selectedMonth,
    selectedDay,
    selectedHour,
    selectedMinute,
    selectedSecond
);
```

**Chỉ gọi `setDateTime()` khi đã chọn xong thời gian**, không cần gọi liên tục trong lúc xoay núm.

---

## 6. Ví dụ hoàn chỉnh

```cpp
#include <TimeRTC.h>

TimeRTC rtc;

void setup()
{
    Serial.begin(115200);

    if (!rtc.begin(8, 9))
    {
        Serial.println("DS3231 NOT FOUND!");
        while (1);
    }

    Serial.println("DS3231 OK");

    rtc.setDateTime(
        2026,
        8,
        16,
        19,
        30,
        0
    );
}

void loop()
{
    TimeData t = rtc.getTime();

    Serial.printf(
        "%02d/%02d/%04d %02d:%02d:%02d\n",
        t.day,
        t.month,
        t.year,
        t.hour,
        t.minute,
        t.second
    );

    delay(1000);
}
```

---

## API nhanh

| Hàm | Chức năng |
|---|---|
| `rtc.begin(8, 9)` | Khởi tạo DS3231 |
| `rtc.setDateTime(...)` | Cài ngày giờ |
| `rtc.getTime()` | Lấy thời gian hiện tại |
| `t.day` | Ngày |
| `t.month` | Tháng |
| `t.year` | Năm |
| `t.hour` | Giờ |
| `t.minute` | Phút |
| `t.second` | Giây |

### Cách dùng cơ bản

```cpp
#include <TimeRTC.h>

TimeRTC rtc;

rtc.begin(8, 9);

rtc.setDateTime(2026, 8, 16, 19, 30, 0);

TimeData t = rtc.getTime();
```

**Module khác chỉ cần gọi `setDateTime()` để cập nhật hoặc `getTime()` để lấy thời gian.**

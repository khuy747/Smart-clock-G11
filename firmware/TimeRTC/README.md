# TimeRTC

Module dùng để giao tiếp với RTC DS3231, cài đặt và lấy thời gian thực.

## Hardware

| DS3231 | ESP32-S3 |
|---|---|
| SDA | GPIO 8 |
| SCL | GPIO 9 |
| VCC | 3.3V |
| GND | GND |

## File Structure

```text
TimeRTC/
├── TimeRTC.h
└── TimeRTC.cpp
```

## Include

```cpp
#include "TimeRTC.h"
```

## Tạo đối tượng

```cpp
TimeRTC RTC;
```

Nên sử dụng tên `RTC` để tránh xung đột với biến `rtc` khác.

## Các hàm

### 1. Khởi tạo RTC

```cpp
RTC.begin(SDA, SCL);
```

Ví dụ:

```cpp
if (!RTC.begin(8, 9))
{
    Serial.println("RTC ERROR!");
}
```

Trả về:

```text
true  → RTC hoạt động
false → Không tìm thấy RTC
```

### 2. Cài đặt thời gian

```cpp
RTC.setDateTime(
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
RTC.setDateTime(
    2026,
    8,
    18,
    20,
    30,
    0
);
```

### 3. Lấy thời gian

```cpp
TimeData now = RTC.getTime();
```

Có thể lấy:

```cpp
now.second
now.minute
now.hour

now.day
now.month
now.year
```

Ví dụ:

```cpp
TimeData now = RTC.getTime();

Serial.printf(
    "%02d/%02d/%04d %02d:%02d:%02d",
    now.day,
    now.month,
    now.year,
    now.hour,
    now.minute,
    now.second
);
```

## Quick Commands

```cpp
TimeRTC RTC;

RTC.begin(8, 9);

RTC.setDateTime(2026, 8, 18, 20, 30, 0);

TimeData now = RTC.getTime();
```

## Lưu ý

- RTC DS3231 tiếp tục chạy độc lập sau khi ESP32 ngắt nguồn nếu pin RTC còn hoạt động.
- `setDateTime()` dùng để ghi thời gian mới vào DS3231.
- `getTime()` dùng để lấy thời gian hiện tại.
- Khi ghép với Alarm, truyền đối tượng `RTC` vào:

```cpp
myAlarm.update(RTC);
```

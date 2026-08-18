# Alarm

Module quản lý báo thức dựa trên thời gian từ TimeRTC và điều khiển Buzzer.

## Chức năng

- Đặt thời gian báo thức.
- Kiểm tra thời gian từ RTC.
- Đến giờ → Buzzer kêu.
- Buzzer kêu trong 2 phút.
- Sau 2 phút → Buzzer tắt.
- Chờ 2 phút.
- Nếu chưa STOP → Buzzer báo lại.
- Tiếp tục lặp lại cho đến khi người dùng nhấn STOP.
- Nhấn STOP → tắt Buzzer và hủy báo lại.

## Hardware

### STOP Button

| Button | ESP32-S3 |
|---|---|
| Chân 1 | GPIO 5 |
| Chân 2 | GND |

Button sử dụng:

```cpp
INPUT_PULLUP
```

Không cần điện trở ngoài.

## File Structure

```text
Alarm/
├── Alarm.h
└── Alarm.cpp
```

## Include

```cpp
#include "Alarm.h"
```

## Tạo Alarm

```cpp
Alarm myAlarm;
```

## Các hàm

### 1. Đặt Alarm

```cpp
myAlarm.setAlarm(hour, minute);
```

Ví dụ:

```cpp
myAlarm.setAlarm(7, 30);
```

Đặt báo thức lúc 07:30.

### 2. Cập nhật Alarm

Trong `loop()`:

```cpp
myAlarm.update(RTC);
```

Hàm này sẽ lấy thời gian từ `RTC` và kiểm tra xem đã đến thời gian báo thức hay chưa.

Ví dụ:

```cpp
void loop()
{
    myAlarm.update(RTC);
}
```

### 3. STOP Alarm

```cpp
myAlarm.stop();
```

Khi gọi hàm này:

```text
Buzzer OFF
    ↓
Hủy báo lại
    ↓
Alarm STOP
```

### 4. Tắt hoàn toàn Alarm

```cpp
myAlarm.disableAlarm();
```

Kết quả:

```text
Alarm OFF
Buzzer OFF
Repeat OFF
```

### 5. Kiểm tra Alarm đang bật

```cpp
myAlarm.isEnabled();
```

Trả về:

```text
true  → Alarm đang bật
false → Alarm đang tắt
```

### 6. Kiểm tra Alarm đang kêu

```cpp
myAlarm.isRinging();
```

Trả về:

```text
true  → Alarm đang kêu
false → Alarm không kêu
```

## Chu kỳ hoạt động

```text
Đến giờ
   ↓
Buzzer ON
   ↓
Kêu 2 phút
   ↓
Buzzer OFF
   ↓
Chờ 2 phút
   ↓
Buzzer ON
   ↓
Kêu 2 phút
   ↓
...
```

Nếu nhấn STOP:

```text
Button
   ↓
myAlarm.stop()
   ↓
Buzzer OFF
   ↓
Hủy báo lại
```

## Sử dụng Button STOP

Khai báo:

```cpp
#define STOP_BUTTON_PIN 5
```

Trong `setup()`:

```cpp
pinMode(STOP_BUTTON_PIN, INPUT_PULLUP);
```

Trong `loop()`:

```cpp
if (digitalRead(STOP_BUTTON_PIN) == LOW)
{
    myAlarm.stop();

    while (digitalRead(STOP_BUTTON_PIN) == LOW)
    {
        delay(10);
    }
}
```

## Quick Commands

```cpp
Alarm myAlarm;              // Tạo Alarm

myAlarm.setAlarm(7, 30);    // Đặt Alarm lúc 07:30

myAlarm.update(RTC);        // Cập nhật Alarm

myAlarm.stop();             // STOP Alarm

myAlarm.disableAlarm();     // Tắt hoàn toàn Alarm

myAlarm.isEnabled();        // Kiểm tra Alarm đang bật

myAlarm.isRinging();        // Kiểm tra Alarm đang kêu
```

## Pin

```text
STOP Button → GPIO 5
Button      → GND
```

## Module Relationship

```text
TimeRTC
   ↓
Alarm
   ↓
Buzzer
   ↑
STOP Button
```

Alarm không tự tạo thời gian. Nó sử dụng `TimeRTC` để biết thời gian hiện tại và sử dụng `Buzzer` để phát âm thanh.

# Buzzer

Module dùng để điều khiển buzzer bằng ESP32-S3.

## Hardware

| Buzzer | ESP32-S3 |
|---|---|
| Buzzer + | GPIO 4 |
| Buzzer - | GND |

## File Structure

```text
Buzzer/
├── Buzzer.h
└── Buzzer.cpp
```

## Include

```cpp
#include "Buzzer.h"
```

## Các hàm

### 1. Khởi tạo Buzzer

```cpp
buzzerBegin();
```

Gọi trong `setup()`.

### 2. Bật Buzzer

```cpp
playTone();
```

Buzzer phát tone 1000 Hz.

### 3. Tắt Buzzer

```cpp
stopTone();
```

## Ví dụ

```cpp
#include "Buzzer.h"

void setup()
{
    buzzerBegin();

    playTone();

    delay(2000);

    stopTone();
}

void loop()
{
}
```

Kết quả:

```text
Buzzer ON
    ↓
2 giây
    ↓
Buzzer OFF
```

## Quick Commands

```cpp
buzzerBegin();  // Khởi tạo Buzzer

playTone();     // Bật Buzzer

stopTone();     // Tắt Buzzer
```

## Pin

```text
Buzzer + → GPIO 4
Buzzer - → GND
```

## Lưu ý

Module Buzzer chỉ chịu trách nhiệm phát và tắt âm thanh.

Alarm sẽ sử dụng các hàm:

```cpp
playTone();
stopTone();
```

để điều khiển Buzzer khi báo thức.

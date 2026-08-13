#ifndef TIME_RTC_H
#define TIME_RTC_H

#include <Arduino.h>

struct TimeData {
    int day;
    int month;
    int year;

    int hour;
    int minute;
    int second;
};

class TimeRTC {
public:
    TimeRTC(uint8_t upPin, uint8_t downPin, uint8_t setPin);

    // Khởi tạo DS3231 + 3 nút
    bool begin(uint8_t sdaPin, uint8_t sclPin);

    // Nhập và lưu thời gian vào DS3231
    // Gọi 1 lần khi cần cài giờ
    void setTime();

    // Lấy thời gian hiện tại từ DS3231
    // Có thể gọi liên tục
    TimeData getTime();

private:
    uint8_t _upPin;
    uint8_t _downPin;
    uint8_t _setPin;

    int daysInMonth(int month, int year);
};

#endif
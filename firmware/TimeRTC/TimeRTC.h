#ifndef TIMERTC_H
#define TIMERTC_H

#include <Arduino.h>

struct TimeData
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;

    uint8_t day;
    uint8_t month;
    uint16_t year;
};

class TimeRTC
{
public:
    TimeRTC();

    // Khởi tạo DS3231
    bool begin(uint8_t sda, uint8_t scl);

    // Ghi thời gian mới vào DS3231
    // Có thể được gọi từ Web hoặc Input
    bool setDateTime(
        uint16_t year,
        uint8_t month,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t second
    );

    // Đọc thời gian hiện tại
    TimeData getTime();

private:
    bool isValidDateTime(
        uint16_t year,
        uint8_t month,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t second
    );
};

#endif

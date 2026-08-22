#ifndef TIMERTC_H
#define TIMERTC_H

#include <Arduino.h>

struct TimeData {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

class TimeRTC {
public:
    TimeRTC();
    bool begin(uint8_t sda, uint8_t scl);
    bool setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
    TimeData getTime();
    bool lostPower(); // Bổ sung kiểm tra mất nguồn

private:
    bool isValidDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
};

#endif
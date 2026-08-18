#include "TimeRTC.h"

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;


// ========================================
// CONSTRUCTOR
// ========================================

TimeRTC::TimeRTC()
{
}


// ========================================
// BEGIN
// ========================================

bool TimeRTC::begin(uint8_t sda, uint8_t scl)
{
    Wire.begin(sda, scl);

    if (!rtc.begin())
    {
        return false;
    }

    return true;
}


// ========================================
// SET DATETIME
// ========================================

bool TimeRTC::setDateTime(
    uint16_t year,
    uint8_t month,
    uint8_t day,
    uint8_t hour,
    uint8_t minute,
    uint8_t second
)
{
    // Kiểm tra dữ liệu trước khi ghi
    if (!isValidDateTime(
            year,
            month,
            day,
            hour,
            minute,
            second))
    {
        return false;
    }

    rtc.adjust(
        DateTime(
            year,
            month,
            day,
            hour,
            minute,
            second
        )
    );

    return true;
}


// ========================================
// GET TIME
// ========================================

TimeData TimeRTC::getTime()
{
    DateTime now = rtc.now();

    TimeData data;

    data.second = now.second();
    data.minute = now.minute();
    data.hour   = now.hour();

    data.day   = now.day();
    data.month = now.month();
    data.year  = now.year();

    return data;
}


// ========================================
// VALIDATE DATETIME
// ========================================

bool TimeRTC::isValidDateTime(
    uint16_t year,
    uint8_t month,
    uint8_t day,
    uint8_t hour,
    uint8_t minute,
    uint8_t second
)
{
    if (year < 2000 || year > 2099)
        return false;

    if (month < 1 || month > 12)
        return false;

    if (day < 1 || day > 31)
        return false;

    if (hour > 23)
        return false;

    if (minute > 59)
        return false;

    if (second > 59)
        return false;

    return true;
}

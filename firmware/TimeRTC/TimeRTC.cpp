#include "TimeRTC.h"
#include <Wire.h>
#include <RTClib.h>

static RTC_DS3231 rtc;

TimeRTC::TimeRTC() {}

bool TimeRTC::begin(uint8_t sda, uint8_t scl) {
    Wire.begin(sda, scl);
    return rtc.begin();
}

bool TimeRTC::lostPower() {
    return rtc.lostPower();
}

bool TimeRTC::setDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    if (!isValidDateTime(year, month, day, hour, minute, second)) return false;
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
    return true;
}

TimeData TimeRTC::getTime() {
    DateTime now = rtc.now();
    TimeData data;
    data.second = now.second();
    data.minute = now.minute();
    data.hour   = now.hour();
    data.day    = now.day();
    data.month  = now.month();
    data.year   = now.year();
    return data;
}

bool TimeRTC::isValidDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    if (year < 2000 || year > 2099) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (hour > 23 || minute > 59 || second > 59) return false;
    return true;
}
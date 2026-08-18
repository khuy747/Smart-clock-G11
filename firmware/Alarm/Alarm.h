#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>
#include "TimeRTC.h"

class Alarm
{
public:
    Alarm();

    void setAlarm(uint8_t hour, uint8_t minute);
    void disableAlarm();

    void update(TimeRTC& rtc);
    void stop();

    bool isEnabled();
    bool isRinging();

private:
    uint8_t alarmHour;
    uint8_t alarmMinute;

    bool enabled;
    bool ringing;
    bool stopped;

    unsigned long ringStartTime;
    unsigned long repeatStartTime;

    int lastTriggeredMinute;
};

#endif
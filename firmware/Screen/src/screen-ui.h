#ifndef SCREEN_UI_H
#define SCREEN_UI_H

#include <Arduino.h>
#include "TimeRTC.h"
#include "Alarm.h"

#define MAX_ALARMS 5

void screenUIBegin(TimeRTC& rtc, Alarm alarms[]);
void screenUIUpdate(TimeRTC& rtc, Alarm alarms[]);

#endif
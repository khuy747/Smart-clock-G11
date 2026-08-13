#include "TimeRTC.h"
#include <Wire.h>
#include <RTClib.h>

static RTC_DS3231 rtc;


// =====================================================
// CONSTRUCTOR
// =====================================================

TimeRTC::TimeRTC(
    uint8_t upPin,
    uint8_t downPin,
    uint8_t setPin
)
{
    _upPin = upPin;
    _downPin = downPin;
    _setPin = setPin;
}


// =====================================================
// BEGIN
// =====================================================

bool TimeRTC::begin(
    uint8_t sdaPin,
    uint8_t sclPin
)
{
    // I2C
    Wire.begin(sdaPin, sclPin);

    // Buttons
    pinMode(_upPin, INPUT_PULLUP);
    pinMode(_downPin, INPUT_PULLUP);
    pinMode(_setPin, INPUT_PULLUP);

    // RTC
    if (!rtc.begin())
        return false;

    return true;
}


// =====================================================
// SET TIME
// =====================================================

void TimeRTC::setTime()
{
    int day = 1;
    int month = 1;
    int year = 2026;

    int hour = 0;
    int minute = 0;

    // 0 = day
    // 1 = month
    // 2 = year
    // 3 = hour
    // 4 = minute
    int item = 0;


    while (item < 5)
    {
        // =================================================
        // UP
        // =================================================

        if (digitalRead(_upPin) == LOW)
        {
            switch (item)
            {
                case 0:
                    day++;

                    if (day > daysInMonth(month, year))
                        day = 1;

                    break;


                case 1:
                    month++;

                    if (month > 12)
                        month = 1;

                    if (day > daysInMonth(month, year))
                        day = daysInMonth(month, year);

                    break;


                case 2:
                    year++;

                    if (year > 2099)
                        year = 2000;

                    if (day > daysInMonth(month, year))
                        day = daysInMonth(month, year);

                    break;


                case 3:
                    hour++;

                    if (hour > 23)
                        hour = 0;

                    break;


                case 4:
                    minute++;

                    if (minute > 59)
                        minute = 0;

                    break;
            }

            delay(200);
        }


        // =================================================
        // DOWN
        // =================================================

        if (digitalRead(_downPin) == LOW)
        {
            switch (item)
            {
                case 0:
                    day--;

                    if (day < 1)
                        day = daysInMonth(month, year);

                    break;


                case 1:
                    month--;

                    if (month < 1)
                        month = 12;

                    if (day > daysInMonth(month, year))
                        day = daysInMonth(month, year);

                    break;


                case 2:
                    year--;

                    if (year < 2000)
                        year = 2099;

                    if (day > daysInMonth(month, year))
                        day = daysInMonth(month, year);

                    break;


                case 3:
                    hour--;

                    if (hour < 0)
                        hour = 23;

                    break;


                case 4:
                    minute--;

                    if (minute < 0)
                        minute = 59;

                    break;
            }

            delay(200);
        }


        // =================================================
        // SET / NEXT
        // =================================================

        if (digitalRead(_setPin) == LOW)
        {
            item++;

            delay(200);

            // Chờ thả nút
            while (digitalRead(_setPin) == LOW)
            {
                delay(10);
            }
        }
    }


    // =================================================
    // SAVE TO DS3231
    // =================================================

    rtc.adjust(
        DateTime(
            year,
            month,
            day,
            hour,
            minute,
            0
        )
    );
}


// =====================================================
// GET TIME
// =====================================================

TimeData TimeRTC::getTime()
{
    DateTime now = rtc.now();

    TimeData data;

    data.day = now.day();
    data.month = now.month();
    data.year = now.year();

    data.hour = now.hour();
    data.minute = now.minute();
    data.second = now.second();

    return data;
}


// =====================================================
// DAYS IN MONTH
// =====================================================

int TimeRTC::daysInMonth(
    int month,
    int year
)
{
    // February
    if (month == 2)
    {
        // Leap year
        if (
            (year % 4 == 0 && year % 100 != 0) ||
            (year % 400 == 0)
        )
        {
            return 29;
        }

        return 28;
    }


    // 30-day months
    if (
        month == 4 ||
        month == 6 ||
        month == 9 ||
        month == 11
    )
    {
        return 30;
    }


    // 31-day months
    return 31;
}
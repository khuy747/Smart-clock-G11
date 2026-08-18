#include "Alarm.h"
#include "Buzzer.h"

#define ALARM_DURATION 120000UL   // 2 phút
#define REPEAT_INTERVAL 120000UL  // nghỉ 2 phút


// ========================================
// CONSTRUCTOR
// ========================================

Alarm::Alarm()
{
    alarmHour = 0;
    alarmMinute = 0;

    enabled = false;
    ringing = false;
    stopped = false;

    ringStartTime = 0;
    repeatStartTime = 0;

    lastTriggeredMinute = -1;
}


// ========================================
// SET ALARM
// ========================================

void Alarm::setAlarm(uint8_t hour, uint8_t minute)
{
    if (hour > 23 || minute > 59)
        return;

    alarmHour = hour;
    alarmMinute = minute;

    enabled = true;
    ringing = false;
    stopped = false;

    ringStartTime = 0;
    repeatStartTime = 0;

    lastTriggeredMinute = -1;
}


// ========================================
// DISABLE ALARM
// ========================================

void Alarm::disableAlarm()
{
    enabled = false;
    ringing = false;
    stopped = true;

    stopTone();

    ringStartTime = 0;
    repeatStartTime = 0;
}


// ========================================
// UPDATE ALARM
// ========================================

void Alarm::update(TimeRTC& rtc)
{
    if (!enabled)
        return;

    // Đã nhấn STOP → không xử lý nữa
    if (stopped)
        return;

    TimeData now = rtc.getTime();

    unsigned long currentTime = millis();


    // ====================================
    // ĐANG KÊU
    // ====================================

    if (ringing)
    {
        if (currentTime - ringStartTime >= ALARM_DURATION)
        {
            stopTone();

            ringing = false;

            // Bắt đầu đếm thời gian nghỉ
            repeatStartTime = currentTime;

            Serial.println(">>> BUZZER OFF");
            Serial.println(">>> WAITING 2 MINUTES");
        }

        return;
    }


    // ====================================
    // ĐANG CHỜ BÁO LẠI
    // ====================================

    if (repeatStartTime != 0)
    {
        if (currentTime - repeatStartTime >= REPEAT_INTERVAL)
        {
            playTone();

            ringing = true;

            ringStartTime = currentTime;

            repeatStartTime = 0;

            Serial.println(">>> ALARM REPEAT");
        }

        return;
    }


    // ====================================
    // ALARM BAN ĐẦU
    // ====================================

    if (now.hour == alarmHour &&
        now.minute == alarmMinute &&
        lastTriggeredMinute != now.minute)
    {
        playTone();

        ringing = true;

        ringStartTime = currentTime;

        lastTriggeredMinute = now.minute;

        Serial.println(">>> ALARM START");
    }
}


// ========================================
// STOP ALARM
// ========================================

void Alarm::stop()
{
    stopTone();

    ringing = false;

    // Hủy chu kỳ báo lại
    repeatStartTime = 0;

    // Khóa alarm hiện tại
    stopped = true;

    Serial.println(">>> ALARM STOPPED");
}


// ========================================
// STATUS
// ========================================

bool Alarm::isEnabled()
{
    return enabled;
}


bool Alarm::isRinging()
{
    return ringing;
}
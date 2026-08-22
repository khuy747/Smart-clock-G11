#include <Arduino.h>
#include <TimeRTC.h>
#include <Buzzer.h>
#include <Alarm.h>
#include "screen-ui.h"

static TimeRTC rtc;
static Alarm alarms[MAX_ALARMS];

void setup() {
  Serial.begin(115200);

  // 1. Khởi tạo Còi
  buzzerBegin();

  // 2. Khởi tạo RTC DS3231 (SDA = GPIO5, SCL = GPIO6)
  if (!rtc.begin(5, 6)) {
    Serial.println("Loi: Khong tim thay DS3231!");
    while (1);
  }

  // 3. Khởi tạo Giao diện Màn hình
  screenUIBegin(rtc, alarms);

  Serial.println("SMART CLOCK READY");
}

void loop() {
  // Cập nhật trạng thái reo của 5 Báo thức
  for (int i = 0; i < MAX_ALARMS; i++) {
    alarms[i].update(rtc);
  }

  // Cập nhật Giao diện & Tương tác
  screenUIUpdate(rtc, alarms);
}

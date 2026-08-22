#include "screen-ui.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Buzzer.h"

#define TFT_CS     10
#define TFT_DC      9
#define TFT_RST     8
#define TFT_MOSI   11
#define TFT_SCLK   12
#define TFT_MISO   13
#define TFT_LED     7

#define BUTTON_PIN  4
#define POT_PIN     1

#define BLACK   ILI9341_BLACK
#define WHITE   ILI9341_WHITE
#define CYAN    ILI9341_CYAN
#define YELLOW  ILI9341_YELLOW
#define GREEN   ILI9341_GREEN
#define RED     ILI9341_RED
#define BLUE    ILI9341_BLUE
#define GREY    ILI9341_LIGHTGREY

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

enum ScreenMode {
  SET_TIME_SCREEN,
  TIME_SCREEN,
  TODO_SCREEN,
  ALARM_SCREEN,
  RINGING_SCREEN
};

static Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
static ScreenMode currentScreen = SET_TIME_SCREEN;

static bool displayOn = true;
static unsigned long lastActivityTime = 0;
static const unsigned long SCREEN_TIMEOUT = 60000;

static bool clockRunning = false;
static int timeEditHour = 0;
static int timeEditMinute = 0;
static int lastMinuteDrawn = -1;
static int timeEditField = 0;

static int currentAlarmIndex = 0;
static int alarmEditField = 0;
static int ringingAlarmId = -1;
static struct { int hour; int minute; } alarmTempData[MAX_ALARMS];

static bool blinkState = true;
static unsigned long lastBlinkTime = 0;
static const unsigned long BLINK_INTERVAL = 500;

static bool lastButtonState = HIGH;
static bool lastFlickerableState = HIGH;
static unsigned long lastDebounceTime = 0;
static const unsigned long DEBOUNCE_DELAY = 50;
static unsigned long buttonPressStartTime = 0;
static bool buttonIsPressed = false;

static bool waitingClickAction = false;
static unsigned long clickActionStartTime = 0;
static const unsigned long CLICK_TIMEOUT = 300;

struct Task {
  int hour;
  int minute;
  String name;
  bool completed;
};

static Task tasks[] = {
  { 7,  0, "An sang",        false },
  { 8,  0, "Hoc Toan",       false },
  { 9,  0, "Hoc Vat Ly",     false },
  {10, 30, "Nghi giai lao",  false },
  {11,  0, "Hoc Hoa",        false },
  {12,  0, "An trua",        false },
  {13,  0, "Nghi trua",      false },
  {14,  0, "Lam bai tap",    false },
  {15,  0, "Lam du an",      false },
  {16,  0, "Hoc tieng Anh",  false },
  {17,  0, "The thao",       false },
  {18, 30, "An toi",         false },
  {20,  0, "Hoc bai",        false },
  {21,  0, "Doc sach",       false }
};

static const int TASK_COUNT = sizeof(tasks) / sizeof(tasks[0]);
static int selectedTask = 0;
static int topTaskIndex = 0;
static const int MAX_VISIBLE_TASKS = 7;

static int getTextWidth(const char* text, uint8_t textSize) {
  return strlen(text) * 6 * textSize;
}

static void registerActivity() {
  if (!displayOn) return;
  lastActivityTime = millis();
}

static void print2(int value) {
  if (value < 10) tft.print("0");
  tft.print(value);
}

static void drawTitle(const char* title) {
  tft.setTextColor(CYAN);
  tft.setTextSize(3);
  int w = getTextWidth(title, 3);
  tft.setCursor((SCREEN_WIDTH - w) / 2, 10);
  tft.print(title);
  tft.drawFastHLine(15, 48, 290, CYAN);
}

static void drawSetTimeDigits() {
  tft.setTextSize(6);
  tft.setCursor(75, 90);
  if (timeEditField == 0 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(timeEditField == 0 ? YELLOW : WHITE, BLACK);
  }
  print2(timeEditHour);

  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(145, 90);
  tft.print(":");

  tft.setCursor(175, 90);
  if (timeEditField == 1 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(timeEditField == 1 ? YELLOW : WHITE, BLACK);
  }
  print2(timeEditMinute);
}

static void drawSetTimeScreen() {
  if (!displayOn) return;
  tft.fillScreen(BLACK);
  drawTitle("SET CLOCK");
  drawSetTimeDigits();

  tft.setTextSize(2);
  tft.setTextColor(GREY);
  tft.setCursor(110, 175);
  if (timeEditField == 0) tft.print("Chinh Gio");
  else tft.print("Chinh Phut");

  tft.setTextSize(1);
  tft.setCursor(30, 215);
  tft.print("XOAY BIEN TRO: CHINH | 1 CLICK: LUU");
}

static void drawTimeScreen(TimeRTC& rtc) {
  if (!displayOn) return;

  TimeData now = rtc.getTime();
  tft.fillScreen(BLACK);
  drawTitle("TIME");

  lastMinuteDrawn = now.minute;

  tft.setTextSize(5);
  tft.setTextColor(WHITE);

  char timeText[8];
  sprintf(timeText, "%02d:%02d", now.hour, now.minute);
  int w = getTextWidth(timeText, 5);
  tft.setCursor((SCREEN_WIDTH - w) / 2, 70);
  tft.print(timeText);

  tft.setTextSize(2);
  tft.setTextColor(GREY);
  tft.setCursor(105, 125);
  print2(now.day); tft.print("/");
  print2(now.month); tft.print("/");
  tft.print(now.year);

  tft.setTextSize(2);
  tft.setTextColor(YELLOW);
  tft.setCursor(15, 155);
  tft.print("TODO LIST");
  tft.drawFastHLine(15, 175, 290, YELLOW);

  int nearest[3] = { -1, -1, -1 };
  int distanceList[3] = { 99999, 99999, 99999 };
  int nowMinutes = now.hour * 60 + now.minute;

  for (int i = 0; i < TASK_COUNT; i++) {
    if (tasks[i].completed) continue;
    int taskMinutes = tasks[i].hour * 60 + tasks[i].minute;
    int distance = taskMinutes - nowMinutes;
    if (distance < 0) continue;
    for (int j = 0; j < 3; j++) {
      if (distance < distanceList[j]) {
        for (int k = 2; k > j; k--) {
          distanceList[k] = distanceList[k - 1];
          nearest[k] = nearest[k - 1];
        }
        distanceList[j] = distance;
        nearest[j] = i;
        break;
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    if (nearest[i] == -1) continue;
    int index = nearest[i];
    int y = 182 + i * 18;

    tft.setTextColor(i == 0 ? WHITE : GREY);
    tft.setTextSize(1);
    tft.setCursor(15, y);
    tft.print(i == 0 ? ">" : " ");

    tft.setCursor(30, y);
    print2(tasks[index].hour); tft.print(":"); print2(tasks[index].minute);
    tft.setCursor(75, y);
    tft.print(tasks[index].name);
  }
}

static void drawAlarmDigits() {
  tft.setTextSize(5);
  uint16_t savedColor = (alarmEditField == 2) ? GREEN : WHITE;

  tft.setCursor(85, 85);
  if (alarmEditField == 0 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(alarmEditField == 0 ? YELLOW : savedColor, BLACK);
  }
  print2(alarmTempData[currentAlarmIndex].hour);

  tft.setTextColor(savedColor, BLACK);
  tft.setCursor(150, 85);
  tft.print(":");

  tft.setCursor(180, 85);
  if (alarmEditField == 1 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(alarmEditField == 1 ? YELLOW : savedColor, BLACK);
  }
  print2(alarmTempData[currentAlarmIndex].minute);
}

static void drawAlarmScreen() {
  if (!displayOn) return;
  tft.fillScreen(BLACK);

  char titleBuf[30];
  sprintf(titleBuf, "SET ALARM (So %d)", currentAlarmIndex + 1);
  drawTitle(titleBuf);

  drawAlarmDigits();

  if (alarmEditField == 2) {
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.setCursor(55, 155);
    tft.print("DA LUU BAO THUC SO ");
    tft.print(currentAlarmIndex + 1);

    tft.setTextSize(1);
    tft.setTextColor(YELLOW);
    tft.setCursor(35, 195);
    tft.print("XOAY BIEN TRO: CHUYEN BAO THUC");

    tft.setTextSize(1);
    tft.setTextColor(GREY);
    tft.setCursor(75, 215);
    tft.print("1 LAN HOAC 2 LAN: THOAT");
  } else {
    tft.setTextSize(2);
    tft.setTextColor(GREY);
    tft.setCursor(110, 155);
    if (alarmEditField == 0) tft.print("Chinh Gio");
    else tft.print("Chinh Phut");

    tft.setTextSize(1);
    tft.setTextColor(GREY);
    tft.setCursor(20, 210);
    tft.print("XOAY: CHINH | 1 CLICK: LUU | 2 CLICK: OUT");
  }
}

static void drawTodoRow(int row, int index, bool isSelected) {
  int y = 55 + row * 25;
  tft.fillRect(5, y - 2, 310, 22, isSelected ? BLUE : BLACK);
  if (index >= TASK_COUNT) return;

  tft.setTextColor(isSelected ? WHITE : GREY);
  tft.setTextSize(1);
  tft.setCursor(12, y + 5);
  tft.print(isSelected ? ">" : " ");
  tft.setCursor(28, y + 5);
  print2(tasks[index].hour); tft.print(":"); print2(tasks[index].minute);
  tft.setCursor(75, y + 5);
  tft.print(tasks[index].name);
}

static void drawAllTodoItems() {
  for (int row = 0; row < MAX_VISIBLE_TASKS; row++) {
    int index = topTaskIndex + row;
    drawTodoRow(row, index, index == selectedTask);
  }
}

static void drawRingingScreen() {
  tft.fillScreen(RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.setCursor(75, 30);
  tft.print("ALARM!");

  tft.setCursor(75, 90);
  print2(alarmTempData[ringingAlarmId].hour); tft.print(":"); print2(alarmTempData[ringingAlarmId].minute);

  tft.setTextSize(2);
  tft.setCursor(55, 160);
  tft.print("Bao thuc so "); tft.print(ringingAlarmId + 1);

  tft.setCursor(60, 200);
  tft.print("Nhan nut de tat");
}

static void handlePotentiometer() {
  static unsigned long lastPotRead = 0;
  if (millis() - lastPotRead < 80) return;
  lastPotRead = millis();

  int rawPot = analogRead(POT_PIN);
  if (!displayOn) return;

  if (currentScreen == SET_TIME_SCREEN) {
    if (timeEditField == 0) {
      int newHour = constrain(map(rawPot, 0, 4095, 0, 23), 0, 23);
      if (newHour != timeEditHour) {
        timeEditHour = newHour;
        registerActivity();
        drawSetTimeDigits();
      }
    } else {
      int newMinute = constrain(map(rawPot, 0, 4095, 0, 59), 0, 59);
      if (newMinute != timeEditMinute) {
        timeEditMinute = newMinute;
        registerActivity();
        drawSetTimeDigits();
      }
    }
  } else if (currentScreen == TODO_SCREEN) {
    int newSelect = constrain(map(rawPot, 0, 4095, 0, TASK_COUNT - 1), 0, TASK_COUNT - 1);
    if (newSelect != selectedTask) {
      registerActivity();
      int oldSelectedTask = selectedTask;
      selectedTask = newSelect;

      bool needFullRedraw = false;
      if (selectedTask < topTaskIndex) {
        topTaskIndex = selectedTask;
        needFullRedraw = true;
      } else if (selectedTask >= topTaskIndex + MAX_VISIBLE_TASKS) {
        topTaskIndex = selectedTask - MAX_VISIBLE_TASKS + 1;
        needFullRedraw = true;
      }

      if (needFullRedraw) drawAllTodoItems();
      else {
        drawTodoRow(oldSelectedTask - topTaskIndex, oldSelectedTask, false);
        drawTodoRow(selectedTask - topTaskIndex, selectedTask, true);
      }
    }
  } else if (currentScreen == ALARM_SCREEN) {
    if (alarmEditField == 0) {
      int newHour = constrain(map(rawPot, 0, 4095, 0, 23), 0, 23);
      if (newHour != alarmTempData[currentAlarmIndex].hour) {
        alarmTempData[currentAlarmIndex].hour = newHour;
        registerActivity();
        drawAlarmDigits();
      }
    } else if (alarmEditField == 1) {
      int newMinute = constrain(map(rawPot, 0, 4095, 0, 59), 0, 59);
      if (newMinute != alarmTempData[currentAlarmIndex].minute) {
        alarmTempData[currentAlarmIndex].minute = newMinute;
        registerActivity();
        drawAlarmDigits();
      }
    } else if (alarmEditField == 2) {
      int newIdx = constrain(map(rawPot, 0, 4095, 0, MAX_ALARMS - 1), 0, MAX_ALARMS - 1);
      if (newIdx != currentAlarmIndex) {
        currentAlarmIndex = newIdx;
        alarmEditField = 0;
        blinkState = true;
        lastBlinkTime = millis();
        registerActivity();
        drawAlarmScreen();
      }
    }
  }
}

static void executeSingleClick(TimeRTC& rtc, Alarm alarms[]) {
  registerActivity();

  if (currentScreen == SET_TIME_SCREEN) {
    timeEditField++;
    if (timeEditField >= 2) {
      TimeData now = rtc.getTime();
      rtc.setDateTime(now.year > 2000 ? now.year : 2026, now.month > 0 ? now.month : 8, now.day > 0 ? now.day : 19, timeEditHour, timeEditMinute, 0);
      clockRunning = true;
      currentScreen = TIME_SCREEN;
      drawTimeScreen(rtc);
    } else {
      blinkState = true;
      lastBlinkTime = millis();
      drawSetTimeScreen();
    }
  } else if (currentScreen == TIME_SCREEN) {
    currentScreen = TODO_SCREEN;
    selectedTask = 0;
    topTaskIndex = 0;
    tft.fillScreen(BLACK);
    drawTitle("TODO LIST");
    drawAllTodoItems();
  } else if (currentScreen == TODO_SCREEN) {
    currentScreen = ALARM_SCREEN;
    currentAlarmIndex = 0;
    alarmEditField = 0;
    blinkState = true;
    lastBlinkTime = millis();
    drawAlarmScreen();
  } else if (currentScreen == ALARM_SCREEN) {
    alarmEditField++;
    if (alarmEditField == 2) {
      alarms[currentAlarmIndex].setAlarm(alarmTempData[currentAlarmIndex].hour, alarmTempData[currentAlarmIndex].minute);
      blinkState = true;
      drawAlarmScreen();
    } else if (alarmEditField >= 3) {
      currentScreen = TIME_SCREEN;
      drawTimeScreen(rtc);
    } else {
      blinkState = true;
      lastBlinkTime = millis();
      drawAlarmScreen();
    }
  } else if (currentScreen == RINGING_SCREEN) {
    if (ringingAlarmId != -1) {
      alarms[ringingAlarmId].stop();
      alarms[ringingAlarmId].disableAlarm();
    }
    stopTone();
    currentScreen = TIME_SCREEN;
    drawTimeScreen(rtc);
  }
}

static void executeDoubleClick(TimeRTC& rtc) {
  registerActivity();
  if (currentScreen == ALARM_SCREEN || currentScreen == TODO_SCREEN) {
    currentScreen = TIME_SCREEN;
    drawTimeScreen(rtc);
  }
}

static void handleButton(TimeRTC& rtc, Alarm alarms[]) {
  bool currentState = digitalRead(BUTTON_PIN);
  if (currentState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = currentState;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (currentState != lastButtonState) {
      lastButtonState = currentState;

      if (currentState == LOW) {
        buttonPressStartTime = millis();
        buttonIsPressed = true;
      } else if (currentState == HIGH && buttonIsPressed) {
        buttonIsPressed = false;
        unsigned long pressTime = millis() - buttonPressStartTime;

        if (!displayOn) {
          displayOn = true;
          digitalWrite(TFT_LED, HIGH);
          currentScreen = clockRunning ? TIME_SCREEN : SET_TIME_SCREEN;
          if (clockRunning) drawTimeScreen(rtc); else drawSetTimeScreen();
          lastActivityTime = millis();
          return;
        }

        if (pressTime >= 2000) {
          if (currentScreen == TIME_SCREEN) {
            waitingClickAction = false;
            timeEditField = 0;
            currentScreen = SET_TIME_SCREEN;
            drawSetTimeScreen();
          }
        } else if (pressTime < 1000) {
          unsigned long now = millis();
          if (waitingClickAction && (now - clickActionStartTime <= CLICK_TIMEOUT)) {
            waitingClickAction = false;
            executeDoubleClick(rtc);
          } else {
            waitingClickAction = true;
            clickActionStartTime = now;
          }
        }
      }
    }
  }

  if (waitingClickAction && (millis() - clickActionStartTime > CLICK_TIMEOUT)) {
    waitingClickAction = false;
    executeSingleClick(rtc, alarms);
  }
}

void screenUIBegin(TimeRTC& rtc, Alarm alarms[]) {
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  analogReadResolution(12);

  lastButtonState = digitalRead(BUTTON_PIN);
  lastFlickerableState = lastButtonState;

  if (rtc.lostPower()) {
    clockRunning = false;
    currentScreen = SET_TIME_SCREEN;
    drawSetTimeScreen();
  } else {
    clockRunning = true;
    currentScreen = TIME_SCREEN;
    drawTimeScreen(rtc);
  }
}

void screenUIUpdate(TimeRTC& rtc, Alarm alarms[]) {
  handleButton(rtc, alarms);
  handlePotentiometer();

  // Nhấp nháy số khi chỉnh giờ
  if ((currentScreen == ALARM_SCREEN && alarmEditField < 2) || currentScreen == SET_TIME_SCREEN) {
    if (millis() - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      if (currentScreen == ALARM_SCREEN) drawAlarmDigits();
      else drawSetTimeDigits();
    }
  }

  // Cập nhật phút trên màn hình chính
  if (displayOn && currentScreen == TIME_SCREEN) {
    TimeData now = rtc.getTime();
    if (now.minute != lastMinuteDrawn) {
      lastMinuteDrawn = now.minute;
      tft.fillRect(40, 65, 240, 50, BLACK);
      char timeText[8];
      sprintf(timeText, "%02d:%02d", now.hour, now.minute);
      tft.setTextSize(5);
      tft.setTextColor(WHITE);
      int w = getTextWidth(timeText, 5);
      tft.setCursor((SCREEN_WIDTH - w) / 2, 70);
      tft.print(timeText);
    }
  }

  // Kiểm tra chuông reo từ mảng báo thức
  for (int i = 0; i < MAX_ALARMS; i++) {
    if (alarms[i].isRinging() && currentScreen != RINGING_SCREEN) {
      ringingAlarmId = i;
      displayOn = true;
      digitalWrite(TFT_LED, HIGH);
      currentScreen = RINGING_SCREEN;
      drawRingingScreen();
      break;
    }
  }

  // Tắt màn hình do timeout
  if (displayOn && currentScreen != RINGING_SCREEN) {
    if (millis() - lastActivityTime >= SCREEN_TIMEOUT) {
      displayOn = false;
      waitingClickAction = false;
      buttonIsPressed = false;
      tft.fillScreen(BLACK);
      digitalWrite(TFT_LED, LOW);
    }
  }
}
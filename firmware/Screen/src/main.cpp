#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <RTClib.h>

// ============================================================
//                    PIN CONFIG (ESP32-S3)
// ============================================================
#define TFT_CS     10
#define TFT_DC      9
#define TFT_RST     8
#define TFT_MOSI   11
#define TFT_SCLK   12
#define TFT_MISO   13
#define TFT_LED     7  // Chân điều khiển đèn nền màn hình

#define BUTTON_PIN  4  // Nút bấm đơn chuyển trang/xác nhận
#define POT_PIN     1  // Chân đọc BIẾN TRỞ (Analog ADC GPIO 1)
#define BUZZER_PIN 41  // Còi báo thức

// Chân I2C cho DS3231 RTC
#define I2C_SDA     5
#define I2C_SCL     6

// Khởi tạo Display & RTC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
RTC_DS3231 rtc;

// ============================================================
//                         COLORS
// ============================================================
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

int getTextWidth(const char* text, uint8_t textSize) {
  return strlen(text) * 6 * textSize;
}

// ============================================================
//                      SCREEN MODE
// ============================================================
enum ScreenMode {
  SET_TIME_SCREEN,
  TIME_SCREEN,
  TODO_SCREEN,
  ALARM_SCREEN,
  RINGING_SCREEN
};

ScreenMode currentScreen = SET_TIME_SCREEN;

// ============================================================
//                    DISPLAY CONTROL
// ============================================================
bool displayOn = true;
unsigned long lastActivityTime = 0;
const unsigned long SCREEN_TIMEOUT = 60000;

// ============================================================
//                          CLOCK
// ============================================================
bool clockRunning = false;
int currentHour   = 0;
int currentMinute = 0;
int currentSecond = 0;
int lastMinuteDrawn = -1;

int currentDay   = 19;
int currentMonth = 8;
int currentYear  = 2026;
unsigned long lastClockUpdate = 0;

int timeEditField = 0; // 0 = Giờ, 1 = Phút

// ============================================================
//                  MULTI-ALARM SYSTEM
// ============================================================
struct AlarmItem {
  int hour;
  int minute;
  bool enabled;
};

const int MAX_ALARMS = 5;
AlarmItem alarms[MAX_ALARMS] = {
  {0, 0, false},
  {0, 0, false},
  {0, 0, false},
  {0, 0, false},
  {0, 0, false}
};

int currentAlarmIndex = 0;
int alarmEditField = 0;

bool alarmRinging = false;
int ringingAlarmId = -1;

// ============================================================
//                    BLINK CONTROL
// ============================================================
bool blinkState = true;
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 500;

// ============================================================
//                  BUTTON DEBOUNCE
// ============================================================
bool lastButtonState = HIGH;
bool lastFlickerableState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;

unsigned long buttonPressStartTime = 0;
bool buttonIsPressed = false;

bool waitingClickAction = false;
unsigned long clickActionStartTime = 0;
const unsigned long CLICK_TIMEOUT = 300;

// ============================================================
//                          TODO
// ============================================================
struct Task {
  int hour;
  int minute;
  String name;
  bool completed;
};

Task tasks[] = {
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

const int TASK_COUNT = sizeof(tasks) / sizeof(tasks[0]);
int selectedTask = 0;
int topTaskIndex = 0;
const int MAX_VISIBLE_TASKS = 7;

// ============================================================
//                    PROTOTYPES
// ============================================================
void drawTimeScreen();
void updateTimeOnly();
void drawAlarmScreen();
void drawAlarmDigits();
void drawSetTimeScreen();
void drawSetTimeDigits();

void registerActivity() {
  if (!displayOn) return;
  lastActivityTime = millis();
}

void checkDisplayTimeout() {
  if (!displayOn) return;
  if (currentScreen == RINGING_SCREEN) return;

  if (millis() - lastActivityTime >= SCREEN_TIMEOUT) {
    displayOn = false;
    waitingClickAction = false;
    buttonIsPressed = false;
    tft.fillScreen(BLACK);
    digitalWrite(TFT_LED, LOW); // Tắt đèn nền
  }
}

void print2(int value) {
  if (value < 10) tft.print("0");
  tft.print(value);
}

void drawTitle(const char* title) {
  tft.setTextColor(CYAN);
  tft.setTextSize(3);
  int w = getTextWidth(title, 3);
  tft.setCursor((SCREEN_WIDTH - w) / 2, 10);
  tft.print(title);
  tft.drawFastHLine(15, 48, 290, CYAN);
}

void drawDate() {
  tft.setTextSize(2);
  tft.setTextColor(GREY);
  tft.setCursor(105, 125);
  print2(currentDay); tft.print("/");
  print2(currentMonth); tft.print("/");
  tft.print(currentYear);
}

// ============================================================
//                SET TIME SCREEN (STARTUP/EDIT)
// ============================================================
void drawSetTimeDigits() {
  tft.setTextSize(6);

  // Cập nhật GIỜ
  tft.setCursor(75, 90);
  if (timeEditField == 0 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(timeEditField == 0 ? YELLOW : WHITE, BLACK);
  }
  print2(currentHour);

  // Cập nhật DẤU HAI CHẤM
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(145, 90);
  tft.print(":");

  // Cập nhật PHÚT
  tft.setCursor(175, 90);
  if (timeEditField == 1 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(timeEditField == 1 ? YELLOW : WHITE, BLACK);
  }
  print2(currentMinute);
}

void drawSetTimeScreen() {
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

// ============================================================
//                  TIME SCREEN (HH:MM ONLY)
// ============================================================
void drawTimeScreen() {
  if (!displayOn) return;

  tft.fillScreen(BLACK);
  drawTitle("TIME");

  lastMinuteDrawn = currentMinute;

  tft.setTextSize(5);
  tft.setTextColor(WHITE);

  char timeText[8];
  sprintf(timeText, "%02d:%02d", currentHour, currentMinute);
  int w = getTextWidth(timeText, 5);
  tft.setCursor((SCREEN_WIDTH - w) / 2, 70);
  tft.print(timeText);

  drawDate();

  tft.setTextSize(2);
  tft.setTextColor(YELLOW);
  tft.setCursor(15, 155);
  tft.print("TODO LIST");
  tft.drawFastHLine(15, 175, 290, YELLOW);

  int nearest[3] = { -1, -1, -1 };
  int distanceList[3] = { 99999, 99999, 99999 };
  int nowMinutes = currentHour * 60 + currentMinute;

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

    if (i == 0) tft.setTextColor(WHITE);
    else tft.setTextColor(GREY);

    tft.setTextSize(1);
    tft.setCursor(15, y);
    if (i == 0) tft.print(">"); else tft.print(" ");

    tft.setCursor(30, y);
    print2(tasks[index].hour); tft.print(":"); print2(tasks[index].minute);
    tft.setCursor(75, y);
    tft.print(tasks[index].name);
  }
}

void updateTimeOnly() {
  if (!displayOn || currentScreen != TIME_SCREEN) return;

  if (currentMinute != lastMinuteDrawn) {
    lastMinuteDrawn = currentMinute;

    tft.fillRect(40, 65, 240, 50, BLACK);
    char timeText[8];
    sprintf(timeText, "%02d:%02d", currentHour, currentMinute);
    tft.setTextSize(5);
    tft.setTextColor(WHITE);
    int w = getTextWidth(timeText, 5);
    tft.setCursor((SCREEN_WIDTH - w) / 2, 70);
    tft.print(timeText);
  }
}

// ============================================================
//                    TODO SCREEN
// ============================================================
void drawTodoRow(int row, int index, bool isSelected) {
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

void drawAllTodoItems() {
  for (int row = 0; row < MAX_VISIBLE_TASKS; row++) {
    int index = topTaskIndex + row;
    drawTodoRow(row, index, index == selectedTask);
  }
}

void initTodoScreen() {
  if (!displayOn) return;
  tft.fillScreen(BLACK);
  drawTitle("TODO LIST");
  drawAllTodoItems();
}

// ============================================================
//               ALARM SCREEN (MULTI-ALARM)
// ============================================================
void drawAlarmDigits() {
  tft.setTextSize(5);
  uint16_t savedColor = (alarmEditField == 2) ? GREEN : WHITE;

  // Cập nhật GIỜ
  tft.setCursor(85, 85);
  if (alarmEditField == 0 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(alarmEditField == 0 ? YELLOW : savedColor, BLACK);
  }
  print2(alarms[currentAlarmIndex].hour);

  // Cập nhật DẤU HAI CHẤM
  tft.setTextColor(savedColor, BLACK);
  tft.setCursor(150, 85);
  tft.print(":");

  // Cập nhật PHÚT
  tft.setCursor(180, 85);
  if (alarmEditField == 1 && !blinkState) {
    tft.setTextColor(BLACK, BLACK);
  } else {
    tft.setTextColor(alarmEditField == 1 ? YELLOW : savedColor, BLACK);
  }
  print2(alarms[currentAlarmIndex].minute);
}

void drawAlarmScreen() {
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

// ============================================================
//                    UPDATE BLINK
// ============================================================
void updateBlink() {
  if (currentScreen == ALARM_SCREEN && alarmEditField < 2) {
    if (millis() - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      drawAlarmDigits();
    }
  }
  else if (currentScreen == SET_TIME_SCREEN) {
    if (millis() - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      drawSetTimeDigits();
    }
  }
}

// ============================================================
//                    RINGING SCREEN
// ============================================================
void drawRingingScreen() {
  tft.fillScreen(RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.setCursor(75, 30);
  tft.print("ALARM!");

  tft.setCursor(75, 90);
  print2(alarms[ringingAlarmId].hour); tft.print(":"); print2(alarms[ringingAlarmId].minute);

  tft.setTextSize(2);
  tft.setCursor(55, 160);
  tft.print("Bao thuc so "); tft.print(ringingAlarmId + 1);

  tft.setCursor(60, 200);
  tft.print("Nhan nut de tat");
}

// ============================================================
//               HANDLE POTENTIOMETER (BIẾN TRỞ)
// ============================================================
void handlePotentiometer() {
  static unsigned long lastPotRead = 0;
  // Đọc biến trở mỗi 80ms để hạn chế giật màn hình và chống nhiễu
  if (millis() - lastPotRead < 80) return;
  lastPotRead = millis();

  int rawPot = analogRead(POT_PIN); // Đọc giá trị ADC từ 0 - 4095

  if (!displayOn) return;

  // --- 1. MÀN HÌNH CÀI ĐẶT THỜI GIAN ---
  if (currentScreen == SET_TIME_SCREEN) {
    if (timeEditField == 0) { // Chỉnh GIỜ
      int newHour = map(rawPot, 0, 4095, 0, 23);
      newHour = constrain(newHour, 0, 23);
      if (newHour != currentHour) {
        currentHour = newHour;
        registerActivity();
        drawSetTimeDigits();
      }
    } else { // Chỉnh PHÚT
      int newMinute = map(rawPot, 0, 4095, 0, 59);
      newMinute = constrain(newMinute, 0, 59);
      if (newMinute != currentMinute) {
        currentMinute = newMinute;
        registerActivity();
        drawSetTimeDigits();
      }
    }
  }
  // --- 2. MÀN HÌNH DANH SÁCH TODO ---
  else if (currentScreen == TODO_SCREEN) {
    int newSelect = map(rawPot, 0, 4095, 0, TASK_COUNT - 1);
    newSelect = constrain(newSelect, 0, TASK_COUNT - 1);

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
  }
  // --- 3. MÀN HÌNH CÀI BÁO THỨC ---
  else if (currentScreen == ALARM_SCREEN) {
    if (alarmEditField == 0) { // Chỉnh Giờ Báo Thức
      int newHour = map(rawPot, 0, 4095, 0, 23);
      newHour = constrain(newHour, 0, 23);
      if (newHour != alarms[currentAlarmIndex].hour) {
        alarms[currentAlarmIndex].hour = newHour;
        registerActivity();
        drawAlarmDigits();
      }
    }
    else if (alarmEditField == 1) { // Chỉnh Phút Báo Thức
      int newMinute = map(rawPot, 0, 4095, 0, 59);
      newMinute = constrain(newMinute, 0, 59);
      if (newMinute != alarms[currentAlarmIndex].minute) {
        alarms[currentAlarmIndex].minute = newMinute;
        registerActivity();
        drawAlarmDigits();
      }
    }
    else if (alarmEditField == 2) { // Chuyển chọn báo thức 1 - 5
      int newIdx = map(rawPot, 0, 4095, 0, MAX_ALARMS - 1);
      newIdx = constrain(newIdx, 0, MAX_ALARMS - 1);
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

// ============================================================
//                  BUTTON ACTIONS
// ============================================================
void executeSingleClick() {
  registerActivity();

  if (currentScreen == SET_TIME_SCREEN) {
    timeEditField++;
    if (timeEditField >= 2) {
      // Ghi cài đặt mới vào RTC DS3231
      rtc.adjust(DateTime(currentYear, currentMonth, currentDay, currentHour, currentMinute, 0));
      clockRunning = true;
      currentScreen = TIME_SCREEN;
      drawTimeScreen();
    } else {
      blinkState = true;
      lastBlinkTime = millis();
      drawSetTimeScreen();
    }
  }
  else if (currentScreen == TIME_SCREEN) {
    currentScreen = TODO_SCREEN;
    selectedTask = 0;
    topTaskIndex = 0;
    initTodoScreen();
  }
  else if (currentScreen == TODO_SCREEN) {
    currentScreen = ALARM_SCREEN;
    currentAlarmIndex = 0;
    alarmEditField = 0;
    blinkState = true;
    lastBlinkTime = millis();
    drawAlarmScreen();
  }
  else if (currentScreen == ALARM_SCREEN) {
    alarmEditField++;
    if (alarmEditField == 2) {
      alarms[currentAlarmIndex].enabled = true;
      blinkState = true;
      drawAlarmScreen();
    }
    else if (alarmEditField >= 3) {
      currentScreen = TIME_SCREEN;
      drawTimeScreen();
    } else {
      blinkState = true;
      lastBlinkTime = millis();
      drawAlarmScreen();
    }
  }
  else if (currentScreen == RINGING_SCREEN) {
    alarmRinging = false;
    alarms[ringingAlarmId].enabled = false;
    noTone(BUZZER_PIN);
    currentScreen = TIME_SCREEN;
    drawTimeScreen();
  }
}

void executeDoubleClick() {
  registerActivity();
  if (currentScreen == ALARM_SCREEN || currentScreen == TODO_SCREEN) {
    currentScreen = TIME_SCREEN;
    drawTimeScreen();
  }
}

void checkClickTimeout() {
  if (waitingClickAction && (millis() - clickActionStartTime > CLICK_TIMEOUT)) {
    waitingClickAction = false;
    executeSingleClick();
  }
}

void handleButton() {
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
      }
      else if (currentState == HIGH) {
        if (buttonIsPressed) {
          buttonIsPressed = false;
          unsigned long pressTime = millis() - buttonPressStartTime;

          if (!displayOn) {
            displayOn = true;
            digitalWrite(TFT_LED, HIGH); // Bật lại đèn nền
            currentScreen = clockRunning ? TIME_SCREEN : SET_TIME_SCREEN;
            if (clockRunning) drawTimeScreen(); else drawSetTimeScreen();
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
          }
          else if (pressTime < 1000) {
            unsigned long now = millis();
            if (waitingClickAction && (now - clickActionStartTime <= CLICK_TIMEOUT)) {
              waitingClickAction = false;
              executeDoubleClick();
            } else {
              waitingClickAction = true;
              clickActionStartTime = now;
            }
          }
        }
      }
    }
  }
}

// ============================================================
//            UPDATE CLOCK (DS3231 RTC SYNC)
// ============================================================
void updateClock() {
  if (!clockRunning) return;

  // Đọc RTC theo chu kỳ 500ms
  if (millis() - lastClockUpdate < 500) return;
  lastClockUpdate = millis();

  DateTime now = rtc.now();
  currentHour   = now.hour();
  currentMinute = now.minute();
  currentSecond = now.second();
  currentDay    = now.day();
  currentMonth  = now.month();
  currentYear   = now.year();

  if (currentScreen == TIME_SCREEN) {
    updateTimeOnly();
  }
}

// ============================================================
//                  CHECK MULTI-ALARM
// ============================================================
void checkAlarm() {
  if (!clockRunning || alarmRinging) return;
  for (int i = 0; i < MAX_ALARMS; i++) {
    if (alarms[i].enabled) {
      if (currentHour == alarms[i].hour && currentMinute == alarms[i].minute && currentSecond == 0) {
        alarmRinging = true;
        ringingAlarmId = i;
        displayOn = true;
        digitalWrite(TFT_LED, HIGH); // Sáng đèn nền nếu đang tắt
        currentScreen = RINGING_SCREEN;

        drawRingingScreen();
        tone(BUZZER_PIN, 1000);
        break;
      }
    }
  }
}

// ============================================================
//                          SETUP
// ============================================================
void setup() {
  Serial.begin(115200);

  // Cấu hình chân Đèn nền màn hình
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  // Khởi tạo SPI và I2C cho ESP32-S3
  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  Wire.begin(I2C_SDA, I2C_SCL);

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  // Khai báo chân nút bấm & Biến trở
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  analogReadResolution(12); // Thiết lập ADC 12-bit (0 - 4095)

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  lastButtonState = digitalRead(BUTTON_PIN);
  lastFlickerableState = lastButtonState;

  displayOn = true;
  lastActivityTime = millis();

  // Khởi tạo RTC DS3231 và kiểm tra báo lỗi
  if (!rtc.begin()) {
    Serial.println("Loi: Khong tim thấy DS3231!");
    tft.fillScreen(BLACK);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.setCursor(20, 100);
    tft.print("LOI: KHONG THAY RTC!");
    tft.setCursor(20, 130);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Kiem tra lai day SDA (GPIO5) & SCL (GPIO6)");
    while (1);
  }

  // Kiểm tra nếu RTC đã chạy hay bị mất nguồn
  if (rtc.lostPower()) {
    Serial.println("RTC bi mat nguon, yeu cau cai dat lai gio!");
    clockRunning = false;
    currentScreen = SET_TIME_SCREEN;
    drawSetTimeScreen();
  } else {
    DateTime now = rtc.now();
    currentHour   = now.hour();
    currentMinute = now.minute();
    currentSecond = now.second();
    currentDay    = now.day();
    currentMonth  = now.month();
    currentYear   = now.year();

    clockRunning = true;
    currentScreen = TIME_SCREEN;
    drawTimeScreen();
  }

  Serial.println("SMART CLOCK READY - POTENTIOMETER & RTC DS3231 RUNNING");
}

// ============================================================
//                          LOOP
// ============================================================
void loop() {
  updateClock();
  handleButton();
  checkClickTimeout();
  handlePotentiometer();
  updateBlink();
  checkAlarm();
  checkDisplayTimeout();
}

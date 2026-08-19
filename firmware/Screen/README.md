# ⏰ ESP32-S3 Smart Desktop Clock & To-Do Tracker

An interactive, multi-screen desktop clock built for the **ESP32-S3**. It features high-precision timekeeping via a **DS3231 RTC**, a 2.4" **ILI9341 TFT display (320x240)**, an analog potentiometer for smooth UI navigation, a multi-alarm system, and an integrated daily To-Do task preview.

---

## 🌟 Key Features

* **High-Precision RTC**: Synchronized with a hardware DS3231 RTC module with auto-recovery on power loss.
* **To-Do Schedule Preview**: Displays the top 3 upcoming tasks directly on the main screen, alongside a dedicated full-screen scrollable task manager.
* **Multi-Alarm System**: Supports up to 5 independent, configurable daily alarms with buzzer audio alerts.
* **Potentiometer Control**: Analog rotary control to set hours and minutes, navigate menus, and scroll through task lists smoothly.
* **Multi-Function Button**:
  * **Single Click**: Advance settings or cycle screens (`TIME` -> `TODO` -> `ALARM`).
  * **Double Click**: Quick return to the main clock screen.
  * **Long Press (≥ 2s)**: Enter time adjustment mode (`SET CLOCK`).
* **Auto Sleep Power Saving**: Backlight turns off automatically after 60 seconds of inactivity to save power and prevent screen burn-in. Wakes up instantly on button press or alarm event.

---

## 🛠️ Hardware Requirements

* **Microcontroller**: ESP32-S3 Dev Board
* **Display**: 2.4" or 2.8" SPI TFT LCD (ILI9341 Driver, 320x240)
* **RTC Module**: DS3231 I2C Module (with CR2032 backup battery)
* **Input**:
  * 1x Rotary Potentiometer (10kΩ recommended)
  * 1x Push Button (Tactile switch)
* **Audio**: 1x Active or Passive Buzzer
* Breadboard & Jumper Wires

---

## 🔌 Wiring & Pinout

### 1. ILI9341 TFT Display (SPI Interface)

| TFT Pin | ESP32-S3 GPIO | Description |
| :--- | :--- | :--- |
| **CS** | GPIO 10 | Chip Select |
| **DC** | GPIO 9 | Data / Command Selection |
| **RST** | GPIO 8 | Reset |
| **MOSI** | GPIO 11 | SPI Master Out Slave In |
| **SCK** | GPIO 12 | SPI Clock |
| **MISO** | GPIO 13 | SPI Master In Slave Out |
| **LED** | GPIO 7 | Backlight Control |
| **VCC** | 3.3V | Power |
| **GND** | GND | Ground |

### 2. DS3231 RTC Module (I2C Interface)

| DS3231 Pin | ESP32-S3 GPIO | Description |
| :--- | :--- | :--- |
| **SDA** | GPIO 5 | Serial Data Line |
| **SCL** | GPIO 6 | Serial Clock Line |
| **VCC** | 3.3V | Power |
| **GND** | GND | Ground |

### 3. Controls & Peripherals

| Component | ESP32-S3 GPIO | Note |
| :--- | :--- | :--- |
| **Potentiometer** | GPIO 1 | Middle Wiper Pin (Analog ADC) |
| **Push Button** | GPIO 4 | Connected to GND (Uses internal `INPUT_PULLUP`) |
| **Buzzer** | GPIO 41 | Positive pin (Negative pin to GND) |

---

## 📚 Required Libraries

Install these libraries via the **Arduino Library Manager** (`Ctrl+Shift+I` / `Cmd+Shift+I`):

1. **Adafruit GFX Library**
2. **Adafruit ILI9341**
3. **RTClib** (by Adafruit)
4. **Wire** *(Built-in)*
5. **SPI** *(Built-in)*

---

## 🕹️ Controls & Navigation

### Potentiometer (Knob)
* **Set Clock Mode**: Rotate to adjust Hour / Minute values.
* **To-Do Mode**: Rotate to scroll through daily tasks.
* **Alarm Mode**: Rotate to adjust alarm time or switch between Alarm slots (1 to 5).

### Push Button
* **Single Click**:
  * In `TIME_SCREEN`: Switch to `TODO_SCREEN`.
  * In `TODO_SCREEN`: Switch to `ALARM_SCREEN`.
  * In `SET_TIME` / `ALARM`: Confirm current field (Hour -> Minute -> Save).
  * In `RINGING_SCREEN`: Silence the alarm.
* **Double Click**: Quick return to `TIME_SCREEN` from any screen.
* **Long Press (≥ 2s)**: Hold on the main time screen to re-enter time configuration mode.
* **Press When Asleep**: Instantly turns the screen backlight back on.

---

## ⚙️ How to Upload

1. Open **Arduino IDE**.
2. Go to **Tools > Board** and select **ESP32S3 Dev Module**.
3. Configure settings:
   * **USB CDC On Boot**: Enabled
   * **Upload Speed**: 115200
4. Install all required dependencies listed above.
5. Paste the sketch code into Arduino IDE and click **Upload**.

---

## ✏️ Customizing Tasks

To update the default To-Do schedule, edit the `tasks[]` array in the main sketch:

```cpp
Task tasks[] = {
  { 7,  0, "An sang",       false },
  { 8,  0, "Hoc Toan",      false },
  { 9,  0, "Hoc Vat Ly",    false },
  // Add your own tasks here...
};

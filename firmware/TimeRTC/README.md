\# TimeRTC



Thư viện Arduino dùng để quản lý thời gian với module \*\*DS3231 RTC\*\*.



Thư viện hỗ trợ:



\- Đọc ngày, tháng, năm.

\- Đọc giờ, phút, giây.

\- Cài đặt thời gian bằng 3 nút bấm.

\- Tự kiểm tra số ngày trong tháng.

\- Hỗ trợ năm nhuận.

\- Đọc thời gian liên tục từ DS3231.

\- Đóng gói toàn bộ phần RTC để các module khác, ví dụ TFT Display, chỉ cần gọi `getTime()`.



\---



\## 1. Phần cứng



\### DS3231



| DS3231 | ESP32 |

|---|---|

| VCC | 3.3V |

| GND | GND |

| SDA | GPIO 8 |

| SCL | GPIO 9 |

| SQW | Không cần |

| 32K | Không cần |



\### 3 nút bấm



| Nút | ESP32 |

|---|---|

| UP | GPIO 4 |

| DOWN | GPIO 5 |

| SET | GPIO 6 |



Các nút sử dụng `INPUT\_PULLUP`.



\---



\## 2. Thư viện phụ thuộc



Cần cài:



\*\*RTClib by Adafruit\*\*



Arduino IDE:



```text

Tools

→ Manage Libraries

→ tìm "RTClib"

→ Install


# ⚙️ Firmware

> Firmware description for the project.

---

## Table of Contents

- [1. Information](#1-information)
- [2. Directory Structure](#2-directory-structure)
- [3. How to Build & Flash](#3-how-to-build--flash)
- [4. How to Test](#4-how-to-test)
- [5. Module README Template](#5-module-readme-template)

---

## 1. Information

| Attribute | Value |
|-----------|-------|
| MCU | (?) |
| IDE / Toolchain | (?) |
| HAL / Framework | (?) |
| RTOS | (?) *(bare-metal / FreeRTOS / ...)* |

---

## 2. Directory Structure

```
firmware/
├── <project_name>/             # IDE Project
│   ├── Core/                   # Application code
│   │   ├── Src/main.c
│   │   └── Inc/
│   ├── Drivers/
│   │   ├── peri_*/             # Peripheral drivers (calls HAL)
│   │   └── algo_*/             # Algorithms (does NOT call HAL)
│   └── ...
└── README.md                   # This file
```

> Layered architecture: `Application → algo_* → peri_* → HAL`
> Details at [PROJECT_RULES.md](../PROJECT_RULES.md#2-firmware-layered-architecture).

---

## 3. How to Build & Flash

(?)

---

## 4. How to Test

> Testing convention uses `#ifdef TEST_<MODULE>` in `main.c`.
> Details at [PROJECT_RULES.md](../PROJECT_RULES.md#4-firmware-testing-conventions).

---

## 5. Module README Template

> Each `peri_*/` and `algo_*/` folder **must have** a `README.md` when complete.
> Copy the template below into the module folder, save as `README.md`, fill in the content.

### Template:

````markdown
# `<module_name>` — *(one-line description)*

## Functionality

*(What does this module do? What problem does it solve in the system?)*

## Files

| File | Description |
|------|-------------|
| `<name>.c` | *(main implementation)* |
| `<name>.h` | *(public API + defines)* |

## API

```c
void    Module_Init(void);           // Initialize — call once
void    Module_SetX(type param);     // Control
type    Module_GetY(void);           // Read value
bool    Module_IsZ(void);            // Check state
```

## Hardware *(skip for algo)*

| Attribute | Value |
|-----------|-------|
| IC / Sensor | *(component name)* |
| Interface | *(I2C / SPI / ADC / TIM PWM)* |
| MCU Pins | *(PA0, PB6...)* |
| Notes | *(pull-up, PWM frequency, I2C address...)* |

## How to Modify / Extend

- **Change pins**: modify `XXX_PIN` in `.h`
- **Change parameters**: modify `XXX_CONFIG` in `Module_Init()`
- **Add features**: *(suggestions for extension if any)*

## Notes

- *(Gotchas, timing constraints, init order, common errors)*
- *(Things that will waste debug time if not read)*

## Test

1. Uncomment `#define TEST_<MODULE>` in main.c
2. Build → Flash → Serial Monitor
3. Expected result: *(describe correct output)*
````

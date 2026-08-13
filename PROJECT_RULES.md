# 📏 PROJECT_RULES — Project Conventions

> All project conventions are documented here. This is the **single source of truth**.
> Other README files only describe directory structures and link back to this file.

---

## 1. File Placement Rules

- **DO NOT** create `.c`, `.h`, `.py` files directly at the root.


- Third-party libraries: Place in `firmware/<project>/Drivers/` or create a `ThirdParty/` directory.
---
### Files allowed at root:

- `README.md`             
- `.gitignore`          
- `LICENSE`               
- `PROJECT_RULES.md`      

---

## 2. Firmware Layered Architecture

```
┌──────────────────────────────────────┐
│     Application  (Core/Src/main.c)   │  ← Calls driver + algorithm
├──────────────────────────────────────┤
│     algo_*/                          │  ← Logic layer
│     (does NOT call HAL directly)     │
├──────────────────────────────────────┤
│     peri_*/                          │  ← Wraps HAL into module-level API
│     (calls HAL_* internally)         │
├──────────────────────────────────────┤
│     HAL / CMSIS  (IDE-managed)       │  ← DO NOT modify
└──────────────────────────────────────┘
```

### Dependency Rules:
- Application (`Core/`) calls `peri_*/` and `algo_*/` — **avoid** calling `HAL_*` directly.
- `algo_*/` **MUST NOT** include HAL headers — receives data via parameters.
- `peri_*/` is the only layer that calls `HAL_*` functions directly.

### Directory Naming Conventions:

| Directory            | Folder Naming                 | Example                     |
| -------------------- | ----------------------------- | --------------------------- |
| Peripheral driver    | `peri_<MODULE_NAME>/`         | `peri_motor/`, `peri_ir/`   |
| Algorithm            | `algo_*/`                     | `algo_pid/`, `algo_floodfill/`|
| Application          | `core/src/` + `core/inc/`     | *(IDE-managed)*             |

### File Naming Conventions:

**All files use `snake_case` (lowercase, spaces = `_`).**

| Type | Rule | Correct ✅ | Wrong ❌ |
|------|------|-----------|---------|
| Source file | `<module_function>.c` | `drive_motor.c` | `Drive.c`, `motor.c` |
| Header file | `<module_function>.h` | `drive_motor.h` | `DriveMotor.h` |
| Python script | `<tool_function>.py` | `flash_tool.py` | `FlashTool.py` |
| Documentation | `<content_name>.md` | `task_tracking.md` | `TaskTracking.md` |

### C Code Naming Conventions:

| Element | Rule | Example |
|---------|------|---------|
| **Function** | `Module_Action()` — PascalCase with module prefix | `Motor_Init()`, `Motor_SetSpeed()`, `PID_Compute()` |
| **Local variable** | `snake_case` | `int32_t encoder_count;` |
| **Global variable** | `g_snake_case` (prefix `g_`) | `volatile uint32_t g_tick_count;` |
| **Constant / macro** | `UPPER_SNAKE_CASE` | `#define MAX_SPEED 100`, `#define IR_LEFT 0` |
| **Typedef struct** | `PascalCase_t` | `typedef struct { ... } PID_t;` |
| **Enum** | `UPPER_SNAKE_CASE` | `enum { MOTOR_LEFT, MOTOR_RIGHT };` |
| **Function parameter** | `snake_case` | `void Motor_SetSpeed(MotorID id, int8_t percent)` |

### Function Action Conventions:

Use **standard actions** so anyone reading the code can guess what a function does:

| Action | When to Use | Example |
|--------|-------------|---------|
| `_Init()` | Initialize module (called once at startup) | `Motor_Init()`, `IR_Init()` |
| `_DeInit()` | Deinitialize / shut down module | `Motor_DeInit()` |
| `_Start()` / `_Stop()` | Enable / disable continuous operation | `Motor_Start()`, `Motor_Stop()` |
| `_Get<X>()` | Read value (no side effects) | `Encoder_GetCount()` |
| `_Set<X>()` | Write / change value | `Motor_SetSpeed()` |
| `_Read()` / `_Write()` | Read/write hardware I/O (has side effects) | `IR_ReadRaw()`, `UART_Write()` |
| `_Is<X>()` | Check state → returns `bool` | `IR_IsWallDetected()` |
| `_Reset()` | Restore to initial state | `PID_Reset()`, `Encoder_Reset()` |
| `_Compute()` / `_Process()` | Calculate / process one cycle | `PID_Compute()` |
| `_IRQHandler()` | Interrupt handler (ISR) — HAL callback only | `Motor_IRQHandler()` |

## 3. Documentation Conventions

### Documentation for Major Directories

Each major directory (`firmware/`, `hardware/`, `software/`) has a `README.md` describing:
- Contents and directory structure
- Tools / toolchain used
- Setup, build, or usage instructions

> When starting work on a section → fill in the corresponding `README.md`.

### Documentation for Firmware Modules

Each `peri_*/` and `algo_*/` folder **must have** a `README.md` when the module is complete.

#### Required Content:
- **Functionality** — what the module does
- **API** — list of public functions
- **Hardware** — IC, interface, MCU pins *(skip for algo)*
- **How to modify** — where to change pins, configs
- **Notes** — gotchas, timing, common errors

#### Template:
See the template at the end of [firmware/README.md](firmware/README.md#module-readme-template) — copy it into the module folder, rename to `README.md`, fill in the content.

#### Workflow:
```
Write code → Test pass → Write README.md → Commit with code
```

> Module without README.md = not done.



## 4. Firmware Testing Conventions

### Approach: use `#ifdef TEST_<MODULE>`

In `main.c`, add a test block for each module. Change the `#define` line to select which module to test:

```c
// ===== TEST MODE — Uncomment 1 line to test =====
// #define TEST_MOTOR
// #define TEST_IR
// #define TEST_IMU

int main(void) {
    HAL_Init();
    SystemClock_Config();
    // ... init ...

#ifdef TEST_MOTOR
    Motor_Init();
    printf("=== TEST MOTOR ===\r\n");
    while(1) {
        Motor_SetSpeed(50);
        HAL_Delay(2000);
        Motor_Stop();
        printf("Enc: %ld\r\n", Encoder_GetCount());
        HAL_Delay(1000);
    }
#endif

    // Main run mode (when no TEST is defined)
    while(1) { /* state machine */ }
}
```

### New Module Testing Workflow:

```
1. Write driver (Drivers/peri_XXX/)
2. Add include path in IDE
3. Add #ifdef TEST_XXX block in main.c
4. Uncomment #define TEST_XXX
5. Build → Flash → Serial Monitor
6. PASS → comment out #define
   FAIL → fix driver → repeat
```

### Notes:
- **Keep test code in main.c** — don't delete it, you may need to retest later
---

## 5. Commit Message Conventions

```
[PREFIX] short description in English
```

### Prefix Table:

| Prefix    | When to Use                                    | Example                                       |
| --------- | ---------------------------------------------- | --------------------------------------------- |
| `[FW]`    | Firmware changes (driver, algorithm, app)      | `[FW] add MPU6050 driver`                     |
| `[HW]`    | Hardware file changes (schematic, PCB, BOM)    | `[HW] finalize schematic v1.0`               |
| `[MECH]`  | Mechanical file changes (CAD, STL)             | `[MECH] update chassis v2`                   |
| `[SW]`    | Host software changes (GUI, script)            | `[SW] add serial monitor`                    |
| `[TOOL]`  | Tools / scripts changes                        | `[TOOL] add auto-flash script`               |
| `[DOC]`   | Documentation changes (docs, README, specs)    | `[DOC] update pinout`                        |
| `[CFG]`   | Config changes (.gitignore, rules, CI)         | `[CFG] add Keil ignore rule`                 |
| `[FIX]`   | Bug fix                                        | `[FIX] fix PID reset on direction change`    |
| `[REFAC]` | Refactor / clean up code                       | `[REFAC] extract init function to separate file` |
|`[STRUC]`  | Project structure changes                      | `[STRUC] update README.md`                   |

### Additional Rules:

- You may add a phase reference: `[FW] P3.2 — add encoder driver`


---

*All rule changes must be approved by the project owner before committing.*

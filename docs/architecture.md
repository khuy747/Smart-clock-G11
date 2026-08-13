# 🏗️ System Architecture

> *Status: DRAFT.* This document describes the system architecture and design.
> Technical specifications (requirements, components, parameters) are in [specs.md](specs.md).
>
> Items marked with `(?)` are **undecided decisions** — must be resolved
> before the next phase begins.

---

## Table of Contents

- [1. Design Principles](#1-design-principles)
- [2. System Overview](#2-system-overview)
  - [2.1 Block Diagram](#21-block-diagram)
  - [2.2 Operational Flow Description](#22-operational-flow-description)
- [3. Interface Contracts — Agreements Between Modules](#3-interface-contracts--agreements-between-modules)
  - [3.1 Peri_MOTOR → Application](#31-peri_motor--application)
  - [3.2 Peri_IR → Application](#32-peri_ir--application)
  - [3.3 Algorithm → Application](#33-algorithm--application)

---

## 1. Design Principles

*(Core philosophies guiding all decisions throughout the project.)*

| # | Principle | Explanation |
|---|-----------|-------------|
| 1 | (?) | *(e.g., "Modular — each module has a clear, replaceable interface")* |
| 2 | (?) | *(e.g., "Simple first — make it work, then optimize")* |
| 3 | (?) | *(e.g., "Low cost — use common, easy-to-source components")* |

---

## 2. System Overview

### 2.1 Block Diagram

```
┌───────────────────────────────────────────────┐
│                  Host / PC                    │
└───────────────────────┬───────────────────────┘
                        │
┌───────────────────────┼───────────────────────┐
│                       ▼                       │
│                  MCU (?)                      │
│                                               │
│  ┌─────────────────────────────────────────┐  │
│  │  Application   (main.c, app logic)      │  │
│  ├─────────────────────────────────────────┤  │
│  │  Algorithm     (algo_*/)                │  │
│  ├─────────────────────────────────────────┤  │
│  │  Peri Drivers  (peri_*/)                │  │
│  ├─────────────────────────────────────────┤  │
│  │  HAL / CMSIS   (IDE-managed)            │  │
│  └─────────────────────────────────────────┘  │
│                       │                       │
│          ┌────────────┴────────────┐          │
│          │    Peripherals (?)      │          │
│          └─────────────────────────┘          │
│                                               │
└───────────────────────┬───────────────────────┘
                        │
┌───────────────────────┼───────────────────────┐
│                       ▼                       │
│                  Hardware                     │
└───────────────────────────────────────────────┘
```

> Replace with the actual diagram when hardware design is finalized.

### 2.2 Operational Flow Description

*(Describe in words: how does data flow from sensor → MCU → processing → actuator? Where does the Host intervene?)*

(?)

---

## 3. Interface Contracts — Agreements Between Modules

> Each module communicates only through its defined API.
> Modifying module internals → OK. Modifying interface → must notify all dependent modules.
>
> **Status:** ⬜ Not implemented · ✅ Implemented + test pass

### 3.1 Peri_MOTOR → Application

```c
// Interface that Application will call:
void    Motor_Init(void);                              // ⬜
void    Motor_SetSpeed(MotorID id, int8_t percent);    // ⬜  -100 to +100
void    Motor_Stop(void);                              // ⬜
int32_t Encoder_GetCount(EncoderID id);                // ⬜
void    Encoder_Reset(EncoderID id);                   // ⬜
```

> (?) — fill in when writing the actual driver. This is a template.

### 3.2 Peri_IR → Application

```c
void     IR_Init(void);                                           // ⬜
uint16_t IR_Read(IR_Channel ch);                                  // ⬜  Returns raw ADC value
bool     IR_IsWallDetected(IR_Channel ch, uint16_t threshold);    // ⬜
```

> (?)

### 3.3 Algorithm → Application

```c
// PID does not call HAL — receives data via parameters
void  PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd);         // ⬜
float PID_Compute(PID_Controller *pid, float setpoint, float measured);    // ⬜
void  PID_Reset(PID_Controller *pid);                                      // ⬜
```

> (?)

---

*Last updated: <!-- DATE -->*

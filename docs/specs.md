# 📋 System Specifications

> *Status: DRAFT.* This document defines the technical specifications — requirements, constraints,
> and key technical decisions throughout the project.
> Design architecture (layering, API, interface contracts) is in [architecture.md](architecture.md).
>
> Items marked with `(?)` are **undecided decisions** — must be resolved
> before the next phase begins.

---

## Table of Contents

- [1. Project Goals & Scope](#1-project-goals--scope)
- [2. Functional Requirements](#2-functional-requirements)
- [3. Non-Functional Requirements](#3-non-functional-requirements)
- [4. Hardware Specifications](#4-hardware-specifications)
- [5. Firmware Specifications](#5-firmware-specifications)
- [6. Software Specifications](#6-software-specifications)
- [7. Inter-Component Communication](#7-inter-component-communication)
- [8. References](#8-references)
- [9. Open Questions](#9-open-questions)

---

## 1. Project Goals & Scope

### 1.1 Goals

*(What problem does this project solve? Who is it for? Specific applications?)*

(?)

### 1.2 MVP Scope (Minimum Viable Product)

<!-- 
💡 MVP SCOPE GUIDE (lessons learned from the Gesture Air Piano project):

1. TIER features by development order:
   - Core MVP: CORE functionality — the system must work at the most basic level.
     Example: detect hand → count fingers → play music (no need to be pretty, no need to be smooth, just WORKING)
   - Optimization MVP: Optimization/polish features — NEEDED for the product to be USABLE.
     Example: gesture noise filtering, mute toggle (without filtering, notes jump erratically)
   - Out of Scope: Nice-to-have features NOT NEEDED in the first version.

2. DESCRIBE OPERATION specifically:
   - Main flow: Input → Processing → Output (e.g., camera → detect → count → play music)
   - Expected performance: Target FPS? Acceptable latency? Minimum accuracy?

3. DEBUG & TESTING:
   - Each module should be independently testable (running the file alone produces results)
   - Specify verification method: print log, visual output, unit test?

4. DEVELOPMENT ORDER LOGIC:
   - Module A must work → before developing module B that optimizes A.
   - Example: must detect hand first → then think about noise filtering.
   - Optimization features depend on core → separate them to avoid feature creep.
-->

**Core MVP — Core Functionality (MUST have):**
- (?) — List the most essential functions, system must work at the most basic level

> *Tip: Describe the main flow: `Input → Processing → Output`. No need to be pretty, no need to optimize, just WORKING.*

**Optimization MVP — Optimization & Polish (NEEDED to be usable):**
- (?) — List optimization/experience improvement features. Only develop AFTER core is working.

> *Tip: Noise filtering, smoothing, edge-case handling, UX improvement. These are things that if missing, the app still runs but is hard to use.*

**Out of MVP scope (do later):**
- (?) — list nice-to-have features NOT needed in MVP

**Expected Performance:**
- (?) — *(e.g., ≥ 20 FPS, latency ≤ 100ms, accuracy ≥ 90%)*

**Debug & Testing approach:**
- (?) — *(e.g., each module can run independently via `if __name__ == "__main__"`, print log to console, visual output via OpenCV)*

> Defining scope clearly helps avoid "feature creep" — doing too many things at once.
> Tiering Core → Optimization → Future helps prioritize development order correctly.

---

## 2. Functional Requirements

| ID | Requirement | Priority | Notes |
|----|-------------|----------|-------|
| FR-01 | (?) | High / Medium / Low | |
| FR-02 | (?) | | |

> Add rows as new requirements emerge.

---

## 3. Non-Functional Requirements

| ID | Requirement | Target Value | Notes |
|----|-------------|--------------|-------|
| NFR-01 | Response time | ≤ (?) ms | |
| NFR-02 | Size / Weight | (?) | *(if physical constraints exist)* |
| NFR-03 | Power supply / Energy | (?) | *(if battery-powered)* |

> Add / remove rows as needed. Not every project needs all of these.

---

## 4. Hardware Specifications

> *Remove this section if the project has no hardware.*

### 4.1 Overview

| Attribute | Value |
|-----------|-------|
| MCU / SoC | (?) *(family, package, Flash/RAM)* |
| Power supply | (?) *(battery / adapter / USB — voltage rails)* |
| Mechanical | (?) *(dimensions, material, manufacturing)* |

### 4.2 Main Component List

| # | Component | Function | Interface | Notes |
|---|-----------|----------|-----------|-------|
| 1 | (?) | | | |

### 4.3 Pinout

| MCU Pin | Function | Peripheral | Module |
|---------|----------|------------|--------|
| (?) | | | |

> Detailed schematic, PCB, BOM are in `hardware/`.

---

## 5. Firmware Specifications

> *Remove this section if the project has no firmware.*

| Attribute | Value |
|-----------|-------|
| IDE / Toolchain | (?) *(STM32CubeIDE / Keil / PlatformIO / …)* |
| HAL / Framework | (?) *(STM32 HAL / LL / Arduino / ESP-IDF / …)* |
| RTOS | (?) *(bare-metal / FreeRTOS / …)* |
| Main loop frequency | (?) *(1 kHz / event-driven / …)* |

> Firmware layered architecture (Application → algo → peri → HAL) see
> [architecture.md](architecture.md) and [PROJECT_RULES.md](../PROJECT_RULES.md).

---

## 6. Software Specifications

> *Remove this section if the project has no host/PC/mobile software.*

| Attribute | Value |
|-----------|-------|
| Platform | (?) *(PC / Web / Mobile / …)* |
| Language | (?) *(Python / C# / JS / …)* |
| Framework / UI | (?) *(PyQt / Electron / React / …)* |
| Main functionality | (?) *(GUI for data display / calibration / logging / …)* |

---

## 7. Inter-Component Communication

> Describe how components (hardware ↔ firmware ↔ software) communicate with each other.

| Connection | Protocol | Data format | Notes |
|------------|----------|-------------|-------|
| MCU ↔ Host | (?) *(UART / USB / BLE / WiFi)* | (?) *(text / binary / JSON)* | |
| Sensor ↔ MCU | (?) *(I2C / SPI / ADC)* | N/A — driver level | |

> Detailed protocol and API contracts see [architecture.md](architecture.md).

---

## 8. References

| Type | Name | Link / File |
|------|------|-------------|
| Datasheet | (?) | |
| App Note | (?) | |
| Library | (?) | |

> Place PDF files in `docs/datasheets/` if offline storage is needed.

---

## 9. Open Questions — Undecided Decisions

> Tracking design questions that need to be resolved. When resolved → record the result and move to the corresponding section.

| # | Question | Status | Result |
|---|----------|--------|--------|
| Q1 | (?) | ⬜ Open | |

> Legend: ⬜ Open · ✅ Resolved · ❌ Dropped

---

*Last updated: <!-- DATE -->*

# 🔌 Hardware

> Hardware design description for the project.

## Structure

```
hardware/
├── schematics/             # Schematic files
├── pcb/                    # PCB layout files
├── bom/                    # Bill of Materials
└── README.md               # This file
```

## Information

| Attribute | Value |
|-----------|-------|
| EDA tool | (?) *(KiCad / Altium / EasyEDA / ...)* |
| Board version | (?) |
| PCB layers | (?) |

## Board Versions

| Version | Date | Status | Notes |
|---------|------|--------|-------|
| v1.0 | (?) | (?) | |

## Test Checklist

> Use when receiving a new board or after soldering/repairing hardware.
> Mark `[x]` when test passes, `[!]` when there is an issue.

### 1. Power Check (BEFORE PLUGGING IN MCU)

- [ ] Check VCC-GND short circuit (must be open circuit)
- [ ] Apply power, measure VCC at MCU pin = *(record value)* V
- [ ] Measure no-load current consumption = *(record value)* mA

### 2. MCU Check

- [ ] Programmer detects MCU
- [ ] Successfully flash LED blink program

### 3. Peripheral Check

- [ ] *(Add each peripheral as design is completed)*

---

| Test Date  | Board Version | Result      | Notes              |
| ---------- | ------------- | ----------- | -------------------|
| *(date)*   | v1.0          | *(pass/fail)* | *(issues if any)* |

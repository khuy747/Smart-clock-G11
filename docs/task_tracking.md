# 📌 Task Tracking — Project Progress

> **This file tracks the overall phase + task-level progress.**
> Break down large tasks into smaller details → handle on scratch (Sheets, paper, scratch) — can be discarded.
> Results documentation → write in the `README.md` of the corresponding module / directory.

---

## 📊 Progress Overview

| Phase | Name                       | Status     | Notes                  |
| ----- | -------------------------- | ---------- | ---------------------- |
| 0     | Project initialization     | ✅ Done     | Structure + docs done  |
| 1     | Hardware design            | ⬜ Todo     |                        |
| 2     | Firmware                   | ⬜ Todo     |                        |
| 3     | Software                   | ⬜ Todo     |                        |
| 4     | Integration & Calibration  | ⬜ Todo     |                        |
| 5     | Test & Release             | ⬜ Todo     |                        |

**Legend**: ✅ Done · 🔨 In Progress · ⚠️ Blocked · ⬜ Todo

---

## 🔧 Phase Details

### Phase 0 — Project Initialization ✅

- [x] Create directory structure
- [x] Write PROJECT_RULES.md
- [x] Write specs.md (draft)
- [x] Write task_tracking.md

---

### Phase 1 — Hardware Design ⬜

- [ ] Select main components
- [ ] Design schematic
- [ ] Design PCB layout
- [ ] Manufacture & receive board

> *Add / modify tasks when starting this phase.*

---

### Phase 2 — Firmware ⬜

- [ ] Setup project (IDE, HAL config)
- [ ] Write peripheral drivers (`peri_*/`)
- [ ] Write algorithms (`algo_*/`)
- [ ] Integration & main state machine

> *When each module is done → write `README.md` in the module folder → tick `[x]` → commit with code.*

---

### Phase 3 — Software ⬜

- [ ] Setup project
- [ ] Communication with firmware
- [ ] UI / main functionality

> *Add / modify tasks when starting this phase.*

---

### Phase 4 — Integration & Calibration ⬜

- [ ] Connect firmware ↔ software end-to-end
- [ ] Calibration
- [ ] Full system test

---

### Phase 5 — Test & Release ⬜

- [ ] Fix remaining bugs
- [ ] Finalize documentation
- [ ] Release v1.0

---

## 🏁 Key Milestones

| #  | Milestone                              | Phase | Deadline   | Status     |
| -- | -------------------------------------- | ----- | ---------- | ---------- |
| M1 | Repo structure + specs complete        | 0     | *(date)*   | ✅         |
| M2 | Board manufactured                     | 1     | *(date)*   | ⬜         |
| M3 | Firmware drivers test pass             | 2     | *(date)*   | ⬜         |
| M4 | System running end-to-end              | 3     | *(date)*   | ⬜         |
| M5 | Release v1.0                           | 4     | *(date)*   | ⬜         |

---

## 📝 Project Log

| Date       | Phase | What was done                       | Result / Blockers         | ➡️ Next steps              |
| ---------- | ----- | ----------------------------------- | -------------------------- | -------------------------- |
| *(date)*   | 0     | Create project template structure   | ✅ OK                      | Start filling specs.md     |

---

## Update Guide

1. **When starting a task** → change `[ ]` to `[/]` (in progress), write 1 log entry.
2. **When task is done** → change to `[x]`, write `README.md` for the module, commit with code.
3. **When a phase changes status** → update the overview table.
4. **When a milestone is reached** → update the milestone table.
5. **Break down large tasks** → use scratch (Sheets / paper / scratch) — can be discarded.

> 💡 When committing, include a phase progress summary in the commit message.
> Example: `[FW] P2.1 — encoder driver done, test pass`

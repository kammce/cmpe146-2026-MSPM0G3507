# Lab 1 - GPIO Driver

In this lab you will:

1. Install the IDE (Code Composer Studio).
2. Write a GPIO driver from scratch using the reference material - TI
   DriverLib is not allowed.
3. Demonstrate control over an RGB LED based on user input.

This document is the complete assignment: what to build, how it's
graded, and how to get started.

## Overview

You will build a minimal GPIO driver for the MSPM0G3507 directly against its
registers. Do not use the TI DriverLib or no SysConfig-generated pin init. Use
the driver to read two push-buttons and drive an RGB LED. This is the
foundational bare-metal skill for the rest of the course: given a
datasheet and a Technical Reference Manual (TRM), you should be able to
find the memory-mapped registers that control a peripheral and drive them
correctly yourself, without depending on a vendor abstraction layer.

## Learning Objectives

- Import and build a Code Composer Studio (Theia) "nortos" project
  targeting the LP-MSPM0G3507 LaunchPad, and flash it over the on-board
  XDS110 debug probe.
- Read the MSPM0G350x TRM well enough to locate the GPIO and IOMUX
  register maps for a specific pin.
- Implement a concrete class against an instructor-provided abstract
  interface (pure virtual functions), using direct memory-mapped
  register access.
- Apply this repo's register-access conventions (`style.md` §S.10,
  Memory-Mapped I/O) to real hardware.

## Hardware

Board: [LP-MSPM0G3507 LaunchPad](https://www.ti.com/tool/LP-MSPM0G3507).

The on-board RGB LED (LED2) is wired to GPIO port B:

| Color | Pin  |
| ----- | ---- |
| Red   | PB26 |
| Green | PB27 |
| Blue  | PB22 |

The two push-buttons (S1, S2) are also wired to GPIO pins, but you'll need
to find exactly which ones yourself from the
[LP-MSPM0G3507 LaunchPad (Dev Board) User's Guide](https://www.ti.com/lit/slau873)
(TI literature number SLAU873).

## What's "from scratch" and what isn't

To keep this from turning into a research project, some things are given
to you and some things are not:

**Given (not graded):**

- The project's build/debug scaffolding (`.project`, `.cproject`,
  `.ccsproject`, `targetConfigs/`) - CCS-managed, don't hand-edit these.
- Clock tree setup and the SWD debug pins, via `lab1_gpio.syscfg`.
- Which physical pin each LED color lives on (a fixed hardware fact, not
  a software design decision).
- `main()`'s overall shape in `lab1_gpio.cpp`.
- The `gpio` pure-virtual interface in `gpio.hpp` - the contract your
  class must satisfy.

**Yours to write (graded):**

- A concrete class in `gpio.cpp` that implements every function in the
  `gpio` interface, storing the register pointers/addresses it needs as
  member variables.
- Finding the S1/S2 button pins from the LaunchPad User's Guide.
- Finding the actual register addresses/offsets and bit fields for GPIO
  and IOMUX from the TRM,
- The button → LED color logic in `lab1_gpio.cpp`'s `main()`.

## Grading Rubric

You have **3 weeks** to complete this lab, worth **50 points** total:

1. **Toolchain setup - 10 pts.** Install CCS along with the **MSPM0-SDK**
   (2.10.00.04) and **SysConfig** products - these are what let CCS
   resolve this project's `${COM_TI_MSPM0_SDK_*}` build variables. Then
   import the `cmpe146-2026-MSPM0G3507` repo as a new workspace folder,
   build this project, and flash/debug it as-is. Minimum required work
   for points: the application prints `"Hello, World"` - the starter
   code already does this via `SYSCFG_DL_init()` and `std::printf`.
1. **GPIO driver - prerequisite for the blinker demonstration below, not
   separately graded.** Write a concrete class that implements the
   `gpio` interface (`gpio.hpp`) using direct register access, storing
   the registers you need as member variables. See
   [Reference material](#reference-material) below for where to find the
   register definitions, and `style.md` §S.10 for how to structure the
   register-map code.
1. **Blinker demonstration - 20 pts.** Using your concrete `gpio`
   implementation in `lab1_gpio.cpp`:
   - Pressing S1 turns the RGB LED to a color (any color).
   - Pressing S2 turns the RGB LED to a different color than S1.
   - Holding both at the same time blinks the RGB LED.
   - The instructor will request that one of the colors be changed
     during the demo - you're responsible for changing it, rebuilding,
     and reflashing to show the new color works.
1. **Code review - 20 pts.** Submit a PR to your git repo targeting your
   `main` branch.

## Reference material

- **[MSPM0G350x Technical Reference Manual](https://www.ti.com/lit/ug/slau846/slau846.pdf)** -
  General Purpose Input/Output (GPIO) chapter and Input/Output Multiplexer
  (IOMUX) chapter. This is where the register offsets and bit fields you need
  actually live.
- **[MSPM0G3507 datasheet](https://www.ti.com/lit/ds/symlink/mspm0g3507.pdf)** -
  IOMUX pin-to-peripheral function table, electrical
  characteristics, peripheral memory map.
- **[LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873)** -
  on-board LED/button wiring and BoosterPack pinout.
- **`style.md` §S.10 (Memory-Mapped I/O)** - the pattern this codebase
  uses for declaring and accessing hardware registers safely
  (`volatile` register-map structs, `reinterpret_cast` from a named
  `constexpr` base address, scoped to an anonymous namespace).
- **Not allowed:** `ti/driverlib/dl_gpio.h`, any `DL_GPIO_*` call, or
  adding a GPIO module back into `lab1_gpio.syscfg`. If your solution
  includes any of those, it doesn't satisfy "from scratch."

## Project layout

```text
lab1_gpio/
├── .project, .cproject, .ccsproject, .settings/   CCS project metadata - generated/maintained by CCS, don't hand-edit
├── targetConfigs/                                 Debug probe + device configuration (XDS110, MSPM0G3507)
├── lab1_gpio.syscfg                                SysConfig: clock tree + SWD debug pins only (no GPIO - that's your job)
├── lab1_gpio.cpp                                   main() - calls into gpio.hpp to implement the lab behavior
├── gpio.hpp                                        Pure virtual gpio interface - given, don't modify
├── gpio.cpp                                        Your concrete class implementing gpio.hpp - graded
└── README.md                                       This file, including the grading rubric
```

## IOMux Hint

The IOMUX value for GPIO is `1`. Its only stated in one area in the datasheet
in section `6.4 Connections for Unused Pins` that the IOMUX value for the GPIO
is value `1`. The section with the IOMUX information only has values 2 and
higher. Sometimes this happens with datasheets where information is either
missing or in a strange place thats hard to find.

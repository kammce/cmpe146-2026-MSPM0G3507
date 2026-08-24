# Lab 6 - UART + CRC

For this lab, you will:

1. Implement a UART/serial driver with a DMA-backed circular receive buffer.
2. Implement a CRC-32 driver from scratch, and benchmark it against a
   supplied hardware-accelerated CRC-32 implementation.
3. Exchange IMU and ADC data with another microcontroller over UART, each
   frame protected by a CRC-32.

## Use cases

Consider this a component of a larger product. Products where a
CRC-protected UART data exchange like this (or one like it) would be used
include:

- Sensor nodes reporting readings to a central controller over a wired link
- Firmware update or configuration transfer between two boards
- Any point-to-point serial link where corrupted data must be detected
  before it's acted on

## Overview

Write a serial driver against the `serial` interface (`serial.hpp`) and a
CRC-32 driver against the `crc32` interface (`crc.hpp`), then use them to:

1. Gather the full IMU reading (X, Y, Z - not just roll/pitch) from your
   lab 4 accelerometer driver, and all 64 raw samples from your lab 5
   `adc16_stream` driver (not just the average).
2. Compute a CRC-32 over that data, using both your from-scratch
   `crc32_software` driver and the supplied hardware-accelerated CRC
   implementation. Time both with a `lab2::steady_clock` and print both
   durations to the console, so you can see which is faster.
3. Transmit the data with the CRC appended over UART. Two devices take
   turns exchanging their packets this way, one after the other, starting
   and stopping the exchange on a button press.
4. On reception, recompute the CRC over the received data and check it
   against the CRC that was sent, to confirm the packet arrived intact.

Each of these should be its own class function (or set of class functions)
added to the `serial_driver` and `crc32_software` classes within the
`lab6_uart_crc.cpp` file.

The UART driver may use DriverLib or be written from scratch - your choice.
Its receive side must be backed by a circular buffer that a DMA transfer (or
interrupt) fills continuously in the background; see `receive_cursor()`'s
docs in `serial.hpp` for how a caller is expected to consume it. The
software CRC-32 driver must be written entirely from scratch - no DriverLib
and no hardware CRC peripheral for that one.

## Learning Objectives

- Implement a UART driver with a DMA-backed circular receive buffer.
- Implement a CRC-32 algorithm from scratch, and understand why a hardware
  CRC accelerator outperforms it.
- Design and implement a simple point-to-point exchange protocol, gated by
  a button, between two independently-running devices.
- Detect corrupted data using a checksum computed on both ends of a link.

## Hardware

Board: [LP-MSPM0G3507 LaunchPad](https://www.ti.com/tool/LP-MSPM0G3507).
You will need two boards to exchange data with each other. Which UART
instance/pins you use is up to you - check the datasheet's
pin-to-peripheral function table for which pins carry UART signals, and
cross-connect the two boards' TX/RX lines (and grounds).

## Grading Rubric (60 pt)

1. **UART transmission - 10 pts.** Serial driver transmits data at the
   configured baud rate.
2. **Circular receive buffer - 10 pts.** Serial driver's receive side is a
   circular buffer that works with DMA.
3. **CRC driver - 10 pts.** `crc32_software` produces a CRC that matches a
   known-good software implementation.
4. **Data transmission - 10 pts.** IMU or ADC data is transmitted with a
   valid CRC appended.
5. **Data reception - 10 pts.** IMU or ADC data is received and its CRC is
   validated.
6. **Code submission - 10 pts.** Submit a PR to your git repo targeting your
   `main` branch.

## Reference material

- **[MSPM0G3507 datasheet](https://www.ti.com/lit/ds/symlink/mspm0g3507.pdf)** -
  pin-to-peripheral function table for UART pins.
- **[MSPM0G350x Technical Reference Manual](https://www.ti.com/lit/ug/slau846e/slau846e.pdf)** -
  UART and CRC accelerator chapters.
- **MSPM0 SDK DriverLib documentation** - UART, DMA, and CRC APIs.

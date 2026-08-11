#include "gpio.hpp"

// TODO(lab1): Implement this driver from scratch.
//
// Read the MSPM0G350x Technical Reference Manual chapters on:
//   - General Purpose Input/Output (GPIO)
//   - Input/Output Multiplexer (IOMUX)
// and the MSPM0G3507 datasheet's pin-to-peripheral function table to find:
//   1. The base address of the GPIOA and GPIOB peripherals.
//   2. The register offsets you need: power enable, output enable,
//      data out (or the atomic set/clear/toggle aliases), and data in.
//   3. The IOMUX "PINCM" register for each pin, and the bit fields that
//      select the GPIO peripheral function, enable input, and select the
//      pull resistor.
//
// Follow this repo's style.md §S.10 (Memory-Mapped I/O) for how to model
// the register map: a `volatile` struct reached via `reinterpret_cast`
// from a named `constexpr` base address, scoped to an anonymous namespace
// in this file. Do not put register offsets or peripheral addresses in
// gpio.hpp — callers of this driver should never need them.

namespace gpio {

// To be added

}  // namespace gpio

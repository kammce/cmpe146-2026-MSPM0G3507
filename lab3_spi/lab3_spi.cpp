#include <cstdio>

#include "ti_msp_dl_config.h"

#include "gpio.hpp"
#include "spi.hpp"

int main(void)
{
  SYSCFG_DL_init();

  std::printf("Hello, World\n");

  // TODO(lab3, step 2): Configure the SPI signal pins and the flash
  // chip's chip-select pin (gpio.hpp), then use spi.hpp to talk to the
  // NOR flash: read + print the manufacturer ID and capacity, read +
  // print the first 32-byte block, then take user input and write up to
  // 32 bytes to that block.

  while (true) {
    // TODO(lab3, step 2): application loop.
  }
}

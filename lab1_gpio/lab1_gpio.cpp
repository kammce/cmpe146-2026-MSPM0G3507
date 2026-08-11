#include <cstdio>

#include "ti_msp_dl_config.h"

#include "gpio.hpp"

int main(void)
{
  SYSCFG_DL_init();

  std::printf("Hello, World\n");

  // TODO(lab1, step 2): Configure the RGB LED pins as outputs and the two
  // push-buttons (S1, S2) as inputs using the gpio:: driver you write in
  // gpio.cpp. See README.md for pin assignments and reference material.

  while (true) {
    // TODO(lab1, step 2): implement the button -> LED color behavior
    // described in lab_instructions.md.
  }
}

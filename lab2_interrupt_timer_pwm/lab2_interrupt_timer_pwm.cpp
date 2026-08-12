#include <cstdio>

#include "ti_msp_dl_config.h"

#include "gpio.hpp"
#include "timer.hpp"

int main(void)
{
  SYSCFG_DL_init();

  std::printf("Hello, World\n");

  // TODO(lab2, step 2): Configure the RGB LED pins as outputs (gpio.hpp),
  // S1/S2 as edge-triggered interrupt inputs (interrupt.hpp), a PWM
  // output (pwm.hpp), and a time base (clock.hpp).

  // TODO(lab2, step 3): Wait for the first button press, then run the
  // rhythm game loop described in README.md: cycle the LED through the
  // color sequence at your chosen BPM, score each press by reaction
  // time, and end the game on a wrong press, a missed window, or a press
  // while the LED is off.

  while (true) {
    // TODO(lab2, step 3): game loop.
  }
}

#include <cinttypes>
#include <cstdint>
#include <cstdio>

#include "../hal/gpio.hpp"
#include "../hal/timer.hpp"

int main()
{
  std::printf("Hello, World\n");

  // TODO(lab2, step 1): Implement lab2::steady_clock

  // TODO(lab2, step 2): Use your steady clock to write a blocking delay and
  // test it with printf or blinking an LED - your choice. Use a printf to
  // check if the delay is happening between print statements

  // TODO(lab2, step3): Implement lab2::pwm using what you learned from
  // lab2::steady_clock

  // TODO(lab2, step4): Test against an LED and see if you can control the
  // brightness

  while (true) {
    // TODO(lab2, step 5): Use the steady clock together with your PWM driver to
    // animate the RGB LED as a continuous color wheel, as described in
    // README.md.
  }
}

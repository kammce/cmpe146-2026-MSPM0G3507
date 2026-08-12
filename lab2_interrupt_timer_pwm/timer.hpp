#pragma once

#include "gpio.hpp"
#include <cstdint>

namespace lab2 {

/**
 * @brief Digital edge triggered interrupt pin hardware abstraction
 *
 * Use this to automatically call a function when a pin's state has
 * transitioned.
 *
 * The transition states are:
 *
 *   - falling edge: the pin reads a transitions from HIGH to LOW
 *   - rising edge: the pin reads a transitions from LOW to HIGH
 *   - both: the pin reads any state change
 */
class edge_triggered_pin
{
public:
  /**
   * @brief The condition in which an interrupt is triggered.
   *
   */
  enum class trigger_edge : uint8_t
  {
    /**
     * @brief Trigger the interrupt when a pin transitions from HIGH voltage to
     * LOW voltage.
     *
     */
    falling = 0,
    /**
     * @brief Trigger the interrupt when a pin transitions from LOW voltage to
     * HIGH voltage.
     *
     */
    rising = 1,
    /**
     * @brief Trigger the interrupt when a pin transitions it state
     *
     */
    both = 2,
  };

  /**
   * @brief Generic settings for interrupt pins
   *
   */
  struct settings
  {
    /**
     * @brief Pull resistor for an interrupt pin.
     *
     * In general, it is highly advised to either set the pull resistor to
     * something other than "none" or to attach an external pull up resistor to
     * the interrupt pin in order to prevent random interrupt from firing.
     */
    pin_resistor resistor = pin_resistor::pull_up;

    /**
     * @brief The trigger condition that will signal the system to run the
     * callback.
     *
     */
    trigger_edge trigger = trigger_edge::rising;

    /**
     * @brief Enables default comparison
     *
     */
    bool operator<=>(settings const&) const = default;
  };

  /**
   * @brief Edge triggered pin handler
   *
   * @param p_state - true if state of the pin when the interrupt was triggered
   * was HIGH, otherwise LOW
   */
  using handler = void(bool p_state);

  /**
   * @brief Configure the edge triggered pin to match the settings supplied
   *
   * @param p_settings - settings to apply to edge triggered pin
   *
   * @return true - if the settings are valid
   * @return false - if the settings could not be achieved
   */
  bool configure(settings const& p_settings)
  {
    driver_configure(p_settings);
  }

  /**
   * @brief Set the callback for when the interrupt occurs
   *
   * Any state transitions before this function is called are lost.
   *
   * @param p_callback - function to execute when the trigger condition occurs.
   */
  void on_trigger(hal::callback<handler> p_callback)
  {
    driver_on_trigger(p_callback);
  }

  virtual ~edge_triggered_pin() = default;

private:
  virtual void driver_configure(settings const& p_settings) = 0;
  virtual void driver_on_trigger(hal::callback<handler> p_callback) = 0;
};

/**
 * @brief Hardware abstraction interface for a steady clock mechanism
 *
 * Implementations of this interface must follow the same requirements as a
 * std::chrono::steady_clock, in that the clock is monotonic & steady. An
 * additional requirement is added to ensure that the clock is reliable. Meaning
 * calls to the interface functions do not return errors because this clock
 * should be infallible. To ensure this, this clock should be driven by the
 * platform's peripheral drivers or some other mechanism that is unlikely to go
 * offline while the platform is in a normal operating state.
 *
 * This clock is steady meaning that subsequent calls to get the uptime of this
 * clock cannot decrease as physical time moves forward and the time between
 * ticks of this clock are constant and defined by the clock's frequency.
 *
 * This can be used to get the time since the boot up, or to be more accurate,
 * the time when the steady clock object is created. This clock is most suitable
 * for measuring time intervals.
 *
 * After creation of this clock, the operating frequency shall not change.
 */
class steady_clock
{
public:
  /**
   * @brief Get the operating frequency of the steady clock
   *
   * @return uint32_t - operating frequency of the steady clock. Guaranteed to
   * be a positive value by the implementing driver.
   */
  [[nodiscard]] uint32_t frequency()
  {
    return driver_frequency();
  }

  /**
   * @brief Get the current value of the steady clock
   *
   * @return uint64_t - Number of counts that the steady clock has counted
   * since it started.
   */
  [[nodiscard]] uint64_t uptime()
  {
    return driver_uptime();
  }

  virtual ~steady_clock() = default;

private:
  virtual uint32_t driver_frequency() = 0;
  virtual uint64_t driver_uptime() = 0;
};

/**
 * @brief Pulse Width Modulation (PWM) channel hardware abstraction.
 *
 * This driver controls the waveform generation of a square wave and its
 * properties such as frequency and duty cycle.
 *
 * Frequency, meaning how often the waveform cycles from from low to high.
 *
 * Duty cycle, what proportion of the wavelength of the pulse is the voltage
 * HIGH.
 *
 *  ```
 *     ____________________       _
 *    |                    |     |
 *   _|                    |_____|
 *    ^                    ^     ^
 *    |<------ high ------>|<low>|
 *
 *    HIGH Duration = 18 segments
 *    LOW Duration =  5 segments
 *    Duty Cycle = 20 / (20 + 5) = 80%
 *
 *    If each segment is 1us then the wavelength is 25us
 *    Thus frequency is (1 / 25us) = 40kHz
 * ```
 *
 * PWM is used for power control like motor control, lighting, transmitting
 * signals to servos, sending telemetry and much more.
 *
 */
class pwm
{
public:
  /**
   * @brief Returns the frequency of this pwm channel
   *
   * @returns uint32_t - frequency in hertz as an unsigned integer
   */
  uint32_t frequency()
  {
    return driver_frequency();
  }

  /**
   * @brief Set the pwm waveform duty cycle
   *
   * The input value `p_duty_cycle` is a 16-bit unsigned value from 0 to 65535.
   *
   * The 16-bit value is directly proportional to the duty cycle
   * percentage, such that a value of:
   *
   *     - 0% would be 0 (65535 * 0.00)
   *     - 100% would be 65535 (65535 * 1.00)
   *     - 25% would be 16384 (65535 * 0.25)
   *     - 44.5% would be 29163 (65535 * 0.445)
   *
   * @param p_duty_cycle - a value from 0 to 65535 representing the duty
   * cycle percentage.
   */
  void duty_cycle(uint16_t p_duty_cycle)
  {
    driver_duty_cycle(p_duty_cycle);
  }

  virtual ~pwm() = default;

private:
  virtual uint32_t driver_frequency() = 0;
  virtual void driver_duty_cycle(uint16_t p_duty_cycle) = 0;
};
}  // namespace lab2

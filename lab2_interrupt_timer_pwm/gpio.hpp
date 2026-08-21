#pragma once

namespace lab2 {

/**
 * @brief Set of possible pin mode resistor settings.
 *
 * See each enumeration to get more details about when and how these should be
 * used.
 *
 */
enum class pin_resistor : uint8_t
{
  /// No pull up. This will cause the pin to float. This may be desirable if the
  /// pin has an external resistor attached or if the signal is sensitive to
  /// external devices like resistors.
  none = 0,
  /// Pull the pin down to devices GND. This will ensure that the voltage read
  /// by the pin when there is no signal on the pin is LOW (or false).
  pull_down,
  /// See pull down explanation, but in this case the pin is pulled up to VCC,
  /// also called VDD on some systems.
  pull_up,
};

/**
 * @brief Digital input pin hardware abstraction interface.
 *
 * Use this to read a pin and determine if the voltage on it is HIGH or LOW.
 *
 */
class input_pin
{
public:
  /// Generic settings for input pins
  struct settings
  {
    /// Pull resistor for an input pin
    pin_resistor resistor = pin_resistor::pull_up;

    /**
     * @brief Enables default comparison
     *
     */
    bool operator<=>(settings const&) const = default;
  };

  /**
   * @brief Configure the input pin to match the settings supplied
   *
   * @param p_settings - settings to apply to input pin
   *
   * @return true - if the settings are valid
   * @return false - if the settings could not be achieved
   */
  bool configure(settings const& p_settings)
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Read the state of the input pin
   *
   * @return bool - true indicates HIGH voltage level and false
   * indicates LOW voltage level
   */
  [[nodiscard]] bool level()
  {
    return driver_level();
  }

  virtual ~input_pin() = default;

private:
  virtual bool driver_configure(settings const& p_settings) = 0;
  virtual bool driver_level() = 0;
};

/**
 * @brief Digital output pin hardware abstraction.
 *
 * Use this to drive a pin HIGH or LOW in order to send a control signal or turn
 * off or on an LED.
 *
 */
class output_pin
{
public:
  /// Generic settings for output pins
  struct settings
  {
    /// Pull resistor for the pin. This generally only helpful when open
    /// drain is enabled.
    pin_resistor resistor = pin_resistor::none;

    /// Starting level of the output pin. HIGH voltage defined as true and LOW
    /// voltage defined as false.
    bool open_drain = false;

    /**
     * @brief Enables default comparison
     *
     */
    bool operator<=>(settings const&) const = default;
  };

  /**
   * @brief Configure the output pin to match the settings supplied
   *
   * @param p_settings - settings to apply to output pin
   *
   * @return true - if the settings are valid
   * @return false - if the settings could not be achieved
   */
  bool configure(settings const& p_settings)
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Set the state of the pin
   *
   * @param p_high - if true then the pin state is set to HIGH voltage. If
   * false, the pin state is set to LOW voltage.
   */
  void level(bool p_high)
  {
    driver_level(p_high);
  }

  /**
   * @brief Read the current state of the output pin from hardware
   *
   * Implementations must read the pin state from hardware and will not simply
   * cache the results from the execution of `level(bool)`.
   *
   * This pin may not equal the state set by `level(bool)` when the pin is
   * configured as open-drain.
   *
   * @return true - if the level of the pin is HIGH
   * @return false - if the level of the pin is LOW
   */
  [[nodiscard]] bool level()
  {
    return driver_level();
  }

  virtual ~output_pin() = default;

private:
  virtual bool driver_configure(settings const& p_settings) = 0;
  virtual void driver_level(bool p_high) = 0;
  virtual bool driver_level() = 0;
};

struct edge_triggered_callback
{
  /**
   * @brief The callback invoked after an edge trigger interrupt on a pin has
   * occurred
   *
   * @param p_state - the current logical state of the pin with an edge
   * triggered interrupt
   */
  virtual void callback(bool p_state) = 0;
};

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
    return driver_configure(p_settings);
  }

  /**
   * @brief Set the callback for when the interrupt occurs
   *
   * Any state transitions before this function is called are lost.
   *
   * @param p_callback - function to execute when the trigger condition occurs,
   * or nullptr to disable interrupt
   */
  void on_trigger(edge_triggered_callback* p_callback)
  {
    driver_on_trigger(p_callback);
  }

  virtual ~edge_triggered_pin() = default;

private:
  virtual bool driver_configure(settings const& p_settings) = 0;
  virtual void driver_on_trigger(edge_triggered_callback* p_callback) = 0;
};
}  // namespace lab2

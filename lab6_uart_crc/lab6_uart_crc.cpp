#include <array>
#include <cstdint>
#include <cstdio>

#include "../hal/adc.hpp"
#include "../hal/crc.hpp"
#include "../hal/gpio.hpp"
#include "../hal/i2c.hpp"
#include "../hal/serial.hpp"
#include "../hal/timer.hpp"

// TODO(lab6, step 1): Make this class inherit from and implement the
// `lab6::serial` interface (`serial.hpp`), using DriverLib or register-level
// code. driver_receive_buffer()/driver_receive_cursor() must be backed by a
// circular buffer that a DMA transfer (or interrupt) fills continuously in
// the background - see `lab6::serial::receive_cursor()`'s docs for how a
// caller is expected to consume it.
class serial_driver
{};

// TODO(lab6, step 2): Make this class inherit from and implement the
// `lab6::crc32` interface (`crc.hpp`), written from scratch (no DriverLib,
// no hardware CRC peripheral). This is the software CRC-32 implementation
// that gets benchmarked against the hardware-accelerated one supplied for
// this lab.
class crc32_software
{};

// A transmitted frame: the full IMU reading (not just roll/pitch) plus all
// 64 raw ADC samples, with a trailing CRC-32 over everything before it.
struct packet
{
  float imu_x;
  float imu_y;
  float imu_z;
  std::array<std::uint16_t, 64> adc_samples;
  std::uint32_t crc;
};

int main()
{
  std::printf("Hello, World\n");

  // TODO(lab6, step 3): Construct your accelerometer driver (lab4) and
  // adc16_stream driver (lab5), a serial_driver, a crc32_software, and the
  // supplied hardware-accelerated lab6::crc32 implementation. Construct a
  // lab1::input_pin for the start/stop button and a lab2::steady_clock for
  // timing the CRC benchmark.

  while (true) {
    // TODO(lab6, step 4): Wait for the button to signal start. Once
    // started, the two devices take turns: fill a `packet` with the IMU
    // reading and 64 ADC samples, compute its CRC with both crc32_software
    // and the supplied hardware CRC implementation (timing each with the
    // steady_clock and printing both durations), transmit the packet with
    // the CRC appended, then wait to receive the other device's packet and
    // validate its CRC. Repeat until the button signals stop.
  }
}

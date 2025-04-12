//
// Created by Apehaenger on 03/24/2025
//

#include <drivers/charger/bq_2576/bq_2576.hpp>
#include <globals.hpp>

#include "robot.hpp"

namespace Robot {

static BQ2576 charger{};

namespace General {
void InitPlatform() {
  // Not used, we could start the GUI driver task here for example
}
bool IsHardwareSupported() {
  /* TODO: Check EEPROM
  // First batch of universal boards have a non-working EEPROM
  // so we assume that the firmware is compatible, if the xcore is the first batch and no carrier was found.
  if (carrier_board_info.board_info_version == 0 &&
      strncmp("N/A", carrier_board_info.board_id, sizeof(carrier_board_info.board_id)) == 0 &&
      strncmp("xcore", board_info.board_id, sizeof(board_info.board_id)) == 0 && board_info.version_major == 1 &&
      board_info.version_minor == 1 && board_info.version_patch == 7) {
    return true;
  }

  // Else, we accept universal boards
  return strncmp("hw-openmower-universal", carrier_board_info.board_id, sizeof(carrier_board_info.board_id)) == 0;
*/
  return true;
}
}  // namespace General

namespace GPS {
UARTDriver* GetUartPort() {
#ifndef STM32_UART_USE_USART6
#error STM32_SERIAL_USE_UART6 must be enabled for the Sabo build to work
#endif
  return &UARTD6;
}
}  // namespace GPS

namespace Power {

I2CDriver* GetPowerI2CD() {
  return &I2CD1;
}

Charger* GetCharger() {
  return &charger;
}

float GetMaxVoltage() {
  return 29.4f;  // As rated on battery pack, which is 7 * 4.2V
}

float GetChargeCurrent() {
  // Battery pack is 7S3P, so max. would be 1.3Ah * 3 = 3.9A
  // 3.9A would be also the max. charge current for the stock PSU!
  return 1.95f;  // Lets stay save and conservative for now
}

float GetMinVoltage() {
  // Stock Sabo battery pack has INR18650-13L (Samsung) which are specified as:
  // Empty = 3.0V, Critical discharge <=2.5V. For now, let's stay save and use 3.3V
  return 7.0f * 3.3;
}

}  // namespace Power

namespace Emergency {
std::pair<const Sensor*, size_t> getSensors() {
  static const Sensor sensors[] = {Sensor{"Front-left-wheel", PAL_LINE(GPIOG, 4), false, SensorType::WHEEL},
                                   Sensor{"Front-right-wheel", PAL_LINE(GPIOG, 5), false, SensorType::WHEEL},
                                   Sensor{"Top-stop-button", PAL_LINE(GPIOG, 8), false, SensorType::BUTTON},
                                   Sensor{"Rear-stop-handle", PAL_LINE(GPIOD, 10), false, SensorType::BUTTON}};
  return {sensors, sizeof(sensors) / sizeof(sensors[0])};
}

u_int getLiftPeriod() {
  return 100;
}

u_int getTiltPeriod() {
  return 2500;
}
}  // namespace Emergency

}  // namespace Robot

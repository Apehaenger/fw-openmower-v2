//
// Created by clemens on 27.01.25.
//

#include "robot.hpp"
#include <globals.hpp>

namespace Robot {

namespace General {
void InitPlatform() {
  // Not used, we could star the GUI driver task here for example
}

bool IsHardwareSupported() {
  // Accept YardForce 1.x.x boards
  return strncmp("hw-openmower-yardforce", carrier_board_info.board_id, sizeof(carrier_board_info.board_id)) == 0 &&
      carrier_board_info.version_major == 1;
}

}  // namespace General

namespace GPS {
UARTDriver* GetUartPort() {
#ifndef STM32_UART_USE_USART6
#error STM32_SERIAL_USE_UART6 must be enabled for the YardForce build to work
#endif
  return &UARTD6;
}
}

namespace Power {

I2CDriver* GetPowerI2CD() {
  return &I2CD1;
}

float GetMaxVoltage() {
  return 7.0f * 4.2f;
}

float GetChargeCurrent() {
  return 0.5;
}

float GetMinVoltage() {
  // 3.3V min, 7s pack
  return 7.0f * 3.3;
}

}  // namespace Power
}  // namespace Robot

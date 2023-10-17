/**
 * @file   Stop.cpp
 * @brief  モーターストップ
 * @author bizyutyu
 */

#include "Stop.h"

Stop::Stop() {}

void Stop::run()
{
  Controller::stopMotor();
}

void Stop::logRunning()
{
  logger.log("Run Stop");
}
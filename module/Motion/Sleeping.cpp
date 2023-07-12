/**
 * @file   Sleeping.cpp
 * @brief  自タスクスリープ
 * @author aridome222
 */

#include "Sleeping.h"
#include "Timer.h"

Sleeping::Sleeping(int _milliSec) : milliSec(_milliSec) {}

void Sleeping::run()
{
  timer.sleep(milliSec);
}

void Sleeping::logRunning()
{
  logger.log("Run Sleeping");
}
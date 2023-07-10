/**
 * @file Clock.cpp
 * @brief クロッククラスで用いる関数（ダミー）
 * @author miyashita64 YKhm20020
 */

#include "Clock.h"
using namespace ev3api;

Clock::Clock()
{
  microTime = 0;
}

void Clock::sleep(int duration)
{
  microTime += static_cast<uint64_t>(duration);
}

uint64_t Clock::now()
{
  microTime = microTime + static_cast<uint64_t>(1);
  return microTime;
}

/**
 * @file Clock.cpp
 * @brief クロッククラスで用いる関数（ダミー）
 * @author miyashita64 YKhm20020
 */

#include "Clock.h"
using namespace ev3api;

void Clock::sleep(int duration)
{
  microTime += duration;
}

uint64_t Clock::now()
{
  return microTime;
}

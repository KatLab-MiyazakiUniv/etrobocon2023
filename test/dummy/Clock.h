/**
 * @file Clock.h
 * @brief クロッククラス（ダミー）
 * @author miyashita64 YKhm20020
 */
#ifndef CLOCK_H
#define CLOCK_H

#include <cstdint>

namespace ev3api {
  class Clock {
   public:
    Clock();
    void sleep(int duration);
    uint64_t now();

   private:
    uint64_t microTime;
  };
}  // namespace ev3api

#endif
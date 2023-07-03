/**
 * @file TimerTest.cpp
 * @brief Timerクラスをテストする
 * @author YKhm20020 miyashita64
 */

#include "Timer.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(TimerTest, sleep)
  {
    Timer timer;
    timer.sleep();
  }

  TEST(TimerTest, now)
  {
    Timer timer;
    timer.now();
  }

  TEST(TimerTest, sleep50)
  {
    Timer timer;
    int sleepTime = 50;
    int initTime = timer.now();
    timer.sleep(sleepTime);
    int actualTime = timer.now();
    int expectedTime = initTime + sleepTime;
    EXPECT_EQ(actualTime, expectedTime);
  }
}  // namespace etrobocon2023_test
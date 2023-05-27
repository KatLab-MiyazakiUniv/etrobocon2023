/**
 * @file TimerTest.cpp
 * @brief Timerクラスをテストする
 * @author YKhm20020
 */

#include "Timer.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(TimerTest, sleep)
  {
    Timer timer;
    timer.sleep();
  }

  TEST(TimerTest, sleep50)
  {
    Timer timer;
    timer.sleep(50);
  }

  TEST(TimerTest, now)
  {
    Timer timer;
    int expected = 0;
    int actual = timer.now();

    EXPECT_EQ(expected, actual);
  }
}  // namespace etrobocon2023_test
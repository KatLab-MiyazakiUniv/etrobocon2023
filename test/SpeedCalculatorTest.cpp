/**
 * @file SpeedCalculatorTest.cpp
 * @brief SpeedCalculatorクラスをテストする
 * @author miyashita64 bizyutu
 */

#include "SpeedCalculator.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(SpeedCalculatorTest, calcPwmFromSpeed)
  {
    SpeedCalculator speedCalc(300.0);
    int actualPwm = speedCalc.calcPwmFromSpeed();
    EXPECT_LT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcPwmFromMinusSpeed)
  {
    SpeedCalculator speedCalc(-250.7);
    int actualPwm = speedCalc.calcPwmFromSpeed();
    EXPECT_GT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcPwmFromZeroSpeed)
  {
    SpeedCalculator speedCalc(0.0);
    int actualPwm = speedCalc.calcPwmFromSpeed();
    EXPECT_EQ(0, actualPwm);
  }
}  // namespace etrobocon2023_test
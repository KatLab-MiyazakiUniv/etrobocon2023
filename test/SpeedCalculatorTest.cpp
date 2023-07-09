/**
 * @file SpeedCalculatorTest.cpp
 * @brief SpeedCalculatorクラスをテストする
 * @author miyashita64 bizyutu
 */

#include "SpeedCalculator.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(SpeedCalculatorTest, calcRightPwmFromSpeed)
  {
    SpeedCalculator speedCalc(300.0);
    int actualPwm = speedCalc.calcRightPwmFromSpeed();
    EXPECT_LT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcRightPwmFromMinusSpeed)
  {
    SpeedCalculator speedCalc(-250.7);
    int actualPwm = speedCalc.calcRightPwmFromSpeed();
    EXPECT_GT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcRightPwmFromZeroSpeed)
  {
    SpeedCalculator speedCalc(0.0);
    int actualPwm = speedCalc.calcRightPwmFromSpeed();
    EXPECT_EQ(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcLeftPwmFromSpeed)
  {
    SpeedCalculator speedCalc(300.0);
    int actualPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_LT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcLeftPwmFromMinusSpeed)
  {
    SpeedCalculator speedCalc(-250.7);
    int actualPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_GT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcLeftPwmFromZeroSpeed)
  {
    SpeedCalculator speedCalc(0.0);
    int actualPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_EQ(0, actualPwm);
  }
}  // namespace etrobocon2023_test
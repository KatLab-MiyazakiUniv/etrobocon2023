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
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(300.0);
    int actualPwm = speedCalc.calcRightPwmFromSpeed();
    EXPECT_LT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcRightPwmFromMinusSpeed)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(-250.7);
    int actualPwm = speedCalc.calcRightPwmFromSpeed();
    EXPECT_GT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcRightPwmFromZeroSpeed)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(0.0);
    int actualPwm = speedCalc.calcRightPwmFromSpeed();
    EXPECT_EQ(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcLeftPwmFromSpeed)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(300.0);
    int actualPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_LT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcLeftPwmFromMinusSpeed)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(-250.7);
    int actualPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_GT(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcLeftPwmFromZeroSpeed)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(0.0);
    int actualPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_EQ(0, actualPwm);
  }

  TEST(SpeedCalculatorTest, calcPwmFromMinusLeftSpeedWithTwoArguments)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(300.0, -300.0);
    int actualRightPwm = speedCalc.calcRightPwmFromSpeed();
    int actualLeftPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_LT(0, actualRightPwm);
    EXPECT_GT(0, actualLeftPwm);
  }

  TEST(SpeedCalculatorTest, calcPwmFromMinusRightSpeedWithTwoArguments)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(-250.7, 250.7);
    int actualRightPwm = speedCalc.calcRightPwmFromSpeed();
    int actualLeftPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_GT(0, actualRightPwm);
    EXPECT_LT(0, actualLeftPwm);
  }

  TEST(SpeedCalculatorTest, calcPwmFromZeroRightSpeedWithTwoArguments)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(0.0, 250.7);
    int actualRightPwm = speedCalc.calcRightPwmFromSpeed();
    int actualLeftPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_EQ(0, actualRightPwm);
    EXPECT_LT(0, actualLeftPwm);
  }

  TEST(SpeedCalculatorTest, calcPwmFromZeroLeftSpeedWithTwoArguments)
  {
    // PWMの初期化
    ManagePwm::updateRightPwm(0.0);
    ManagePwm::updateLeftPwm(0.0);
    SpeedCalculator speedCalc(-300.0, 0.0);
    int actualRightPwm = speedCalc.calcRightPwmFromSpeed();
    int actualLeftPwm = speedCalc.calcLeftPwmFromSpeed();
    EXPECT_GT(0, actualRightPwm);
    EXPECT_EQ(0, actualLeftPwm);
  }
}  // namespace etrobocon2023_test
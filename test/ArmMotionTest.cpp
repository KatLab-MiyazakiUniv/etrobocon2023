/**
 *  @file   ArmMotionTest.cpp
 *  @brief  ArmMotionクラスのテスト
 *  @author bizyutyu
 */

#include "ArmMotion.h"
#include <gtest/gtest.h>
#include "Measurer.h"
#include "Controller.h"

using namespace std;

namespace etrobocon2023_test {
  TEST(ArmMotionTest, runPlusAngle)
  {
    // PWMの初期化
    Controller::stopArmMotor();
    // アームモータ角位置を初期化
    Measurer::resetArmMotorCount();
    int armTargetAngle = 60;
    int armPwm = 40;
    ArmMotion am(armTargetAngle, armPwm);

    // 初期値
    int ArmMotorCount = Measurer::getArmMotorCount();

    // 期待するアーム角位置
    int expected = -60;

    am.run();  // アーム動作を実行

    // 関数実行後のアーム角位置
    int actual = Measurer::getArmMotorCount() - ArmMotorCount;

    // アーム角位置のテスト
    EXPECT_GE(expected, actual);
  }

  TEST(ArmMotionTest, runMinusAngle)
  {
    // PWMの初期化
    Controller::stopArmMotor();
    // アームモータ角位置を初期化
    Measurer::resetArmMotorCount();
    int armTargetAngle = -60;
    int armPwm = 40;
    ArmMotion am(armTargetAngle, armPwm);

    // 初期値
    int ArmMotorCount = Measurer::getArmMotorCount();

    // 期待するアーム角位置
    int expected = 60;

    am.run();  // アーム動作を実行

    // 関数実行後のアーム角位置
    int actual = Measurer::getArmMotorCount() - ArmMotorCount;

    // アーム角位置のテスト
    EXPECT_LE(expected, actual);
  }

  TEST(ArmMotionTest, runZeroAngle)
  {
    // PWMの初期化
    Controller::stopArmMotor();
    // アームモータ角位置を初期化
    Measurer::resetArmMotorCount();
    int armTargetAngle = 0;
    int armPwm = 40;
    ArmMotion am(armTargetAngle, armPwm);

    // 初期値
    int ArmMotorCount = Measurer::getArmMotorCount();

    // 期待するアーム角位置
    int expected = 0;

    am.run();  // アーム動作を実行

    // 関数実行後のアーム角位置
    int actual = Measurer::getArmMotorCount() - ArmMotorCount;

    // アーム角位置のテスト
    EXPECT_EQ(expected, actual);
  }

  TEST(ArmMotionTest, runOver60Angle)
  {
    // PWMの初期化
    Controller::stopArmMotor();
    // アームモータ角位置を初期化
    Measurer::resetArmMotorCount();
    int armTargetAngle = 61;
    int armPwm = 40;
    ArmMotion am(armTargetAngle, armPwm);

    // 初期値
    int ArmMotorCount = Measurer::getArmMotorCount();

    // 期待するアーム角位置
    int expected = -60;

    am.run();  // アーム動作を実行

    // 関数実行後のアーム角位置
    int actual = Measurer::getArmMotorCount() - ArmMotorCount;

    // アーム角位置のテスト
    EXPECT_GE(expected, actual);
  }

  TEST(ArmMotionTest, runUnder60Angle)
  {
    // PWMの初期化
    Controller::stopArmMotor();
    // アームモータ角位置を初期化
    Measurer::resetArmMotorCount();
    int armTargetAngle = -61;
    int armPwm = 40;
    ArmMotion am(armTargetAngle, armPwm);

    // 初期値
    int ArmMotorCount = Measurer::getArmMotorCount();

    // 期待するアーム角位置
    int expected = 60;

    am.run();  // アーム動作を実行

    // 関数実行後のアーム角位置
    int actual = Measurer::getArmMotorCount() - ArmMotorCount;

    // アーム角位置のテスト
    EXPECT_LE(expected, actual);
  }

  TEST(ArmMotionTest, runMinusPWM)
  {
    // PWMの初期化
    Controller::stopArmMotor();
    // アームモータ角位置を初期化
    Measurer::resetArmMotorCount();
    int armTargetAngle = 60;
    int armPwm = -30;
    ArmMotion am(armTargetAngle, armPwm);

    // 初期値
    int ArmMotorCount = Measurer::getArmMotorCount();

    // 期待するアーム角位置
    int expected = 0;

    am.run();  // アーム動作を実行

    // 関数実行後のアーム角位置
    int actual = Measurer::getArmMotorCount() - ArmMotorCount;

    // アーム角位置のテスト
    EXPECT_EQ(expected, actual);
  }

  TEST(ArmMotionTest, runZeroPWM)
  {
    // PWMの初期化
    Controller::stopArmMotor();
    // アームモータ角位置を初期化
    Measurer::resetArmMotorCount();
    int armTargetAngle = 20;
    int armPwm = 0;
    ArmMotion am(armTargetAngle, armPwm);

    // 初期値
    int ArmMotorCount = Measurer::getArmMotorCount();

    // 期待するアーム角位置
    int expected = 0;

    am.run();  // アーム動作を実行

    // 関数実行後のアーム角位置
    int actual = Measurer::getArmMotorCount() - ArmMotorCount;

    // アーム角位置のテスト
    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2023_test
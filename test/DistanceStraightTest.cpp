/**
 *  @file   DistanceStraightTest.cpp
 *  @brief  DistanceStraightクラスのテスト
 *  @author Negimarge
 */

#include "DistanceStraight.h"
#include <gtest/gtest.h>
#include "Measurer.h"
#include "Controller.h"

using namespace std;

namespace etrobocon2023_test {
  TEST(DistanceStraightTest, run)
  {
    // PWMの初期化
    Controller::setRightMotorPwm(0.0);
    Controller::setLeftMotorPwm(0.0);
    double targetDistance = 350;
    double targetSpeed = 50;
    DistanceStraight ds(targetDistance, targetSpeed);

    // 初期値
    int leftCount = Measurer::getLeftCount();
    int rightCount = Measurer::getRightCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);
    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) + targetDistance;

    /**
     * 一回のsetPWM()でダミーのモータカウントに加算される値はpwm * 0.05
     * StraightRunnerのPWM値は100まで加速するので，許容誤差のpwmは100で計算する
     */
    double distanceError = Mileage::calculateMileage(100 * 0.05, 100 * 0.05);  // 許容誤差
    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 関数実行後の走行距離
    leftCount = Measurer::getLeftCount();
    rightCount = Measurer::getRightCount();
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // タイヤごとの走行距離
    double leftDifference = std::abs(leftActual - leftInitial);
    double rightDifference = std::abs(rightActual - rightInitial);
    // タイヤの走行距離の誤差
    double actualError = std::abs(leftDifference - rightDifference);

    // 走行距離のテスト
    EXPECT_LE(expectedDistance, actualDistance);
    EXPECT_GE(expectedDistance + distanceError, actualDistance);

    // 直進できているかのテスト
    EXPECT_GE(expectedError, actualError);
  }

  TEST(DistanceStraightTest, runFullPwm)
  {
    // PWMの初期化
    Controller::setRightMotorPwm(0.0);
    Controller::setLeftMotorPwm(0.0);
    double targetDistance = 350;
    double targetSpeed = 100;
    DistanceStraight ds(targetDistance, targetSpeed);

    // 初期値
    int leftCount = Measurer::getLeftCount();
    int rightCount = Measurer::getRightCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);
    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) + targetDistance;

    /**
     * 一回のsetPWM()でダミーのモータカウントに加算される値はpwm * 0.05
     * StraightRunnerのPWM値は100まで加速するので，許容誤差のpwmは100で計算する
     */
    double distanceError = Mileage::calculateMileage(100 * 0.05, 100 * 0.05);  // 許容誤差
    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 関数実行後の走行距離
    leftCount = Measurer::getLeftCount();
    rightCount = Measurer::getRightCount();
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // タイヤごとの走行距離
    double leftDifference = std::abs(leftActual - leftInitial);
    double rightDifference = std::abs(rightActual - rightInitial);
    // タイヤの走行距離の誤差
    double actualError = std::abs(leftDifference - rightDifference);

    // 走行距離のテスト
    EXPECT_LE(expectedDistance, actualDistance);
    EXPECT_GE(expectedDistance + distanceError, actualDistance);

    // 直進できているかのテスト
    EXPECT_GE(expectedError, actualError);
  }

  TEST(DistanceStraightTest, runMinusDistance)
  {
    // PWMの初期化
    Controller::setRightMotorPwm(0.0);
    Controller::setLeftMotorPwm(0.0);
    double targetDistance = -350;
    double targetSpeed = 100;
    DistanceStraight ds(targetDistance, targetSpeed);

    // 初期値
    int leftCount = Measurer::getLeftCount();
    int rightCount = Measurer::getRightCount();
    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetDistance value passed to DistanceStraight is -350.00";
    expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    ds.run();                            // 直進を実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 関数実行後の走行距離
    leftCount = Measurer::getLeftCount();
    rightCount = Measurer::getRightCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);      // 標準出力でWarningを出している
    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

  TEST(DistanceStraightTest, runBack)
  {
    // PWMの初期化
    Controller::setRightMotorPwm(0.0);
    Controller::setLeftMotorPwm(0.0);
    double targetDistance = 350;
    double targetSpeed = -50;
    DistanceStraight ds(targetDistance, targetSpeed);

    // 初期値
    int leftCount = Measurer::getLeftCount();
    int rightCount = Measurer::getRightCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);
    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) - targetDistance;

    /**
     * 一回のsetPWM()でダミーのモータカウントに加算される値はpwm * 0.05
     * StraightRunnerのPWM値は-100まで加速するので，許容誤差のpwmは-100で計算する
     */
    double distanceError = Mileage::calculateMileage(-100 * 0.05, -100 * 0.05);  // 許容誤差
    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 関数実行後の走行距離
    leftCount = Measurer::getLeftCount();
    rightCount = Measurer::getRightCount();
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // タイヤごとの走行距離
    double leftDifference = std::abs(leftActual - leftInitial);
    double rightDifference = std::abs(rightActual - rightInitial);
    // タイヤの走行距離の誤差
    double actualError = std::abs(leftDifference - rightDifference);

    // 走行距離のテスト
    EXPECT_GE(expectedDistance, actualDistance);
    EXPECT_LE(expectedDistance + distanceError, actualDistance);

    // 直進できているかのテスト
    EXPECT_GE(expectedError, actualError);
  }

  TEST(DistanceStraightTest, runBackFullPwm)
  {
    // PWMの初期化
    Controller::setRightMotorPwm(0.0);
    Controller::setLeftMotorPwm(0.0);
    double targetDistance = 350;
    double targetSpeed = -100;
    DistanceStraight ds(targetDistance, targetSpeed);

    // 初期値
    int leftCount = Measurer::getLeftCount();
    int rightCount = Measurer::getRightCount();
    double leftInitial = Mileage::calculateWheelMileage(leftCount);
    double rightInitial = Mileage::calculateWheelMileage(rightCount);
    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount) - targetDistance;

    /**
     * 一回のsetPWM()でダミーのモータカウントに加算される値はpwm * 0.05
     * StraightRunnerのPWM値は-100まで加速するので，許容誤差のpwmは-100で計算する
     */
    double distanceError = Mileage::calculateMileage(-100 * 0.05, -100 * 0.05);  // 許容誤差
    double expectedError = 1;  // タイヤの走行距離の誤差の期待値

    ds.run();  // 直進を実行

    // 関数実行後の走行距離
    leftCount = Measurer::getLeftCount();
    rightCount = Measurer::getRightCount();
    double leftActual = Mileage::calculateWheelMileage(leftCount);
    double rightActual = Mileage::calculateWheelMileage(rightCount);
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    // タイヤごとの走行距離
    double leftDifference = std::abs(leftActual - leftInitial);
    double rightDifference = std::abs(rightActual - rightInitial);
    // タイヤの走行距離の誤差
    double actualError = std::abs(leftDifference - rightDifference);

    // 走行距離のテスト
    EXPECT_GE(expectedDistance, actualDistance);
    EXPECT_LE(expectedDistance + distanceError, actualDistance);

    // 直進できているかのテスト
    EXPECT_GE(expectedError, actualError);
  }

  TEST(DistanceStraightTest, runZeroPwm)
  {
    // PWMの初期化
    Controller::setRightMotorPwm(0.0);
    Controller::setLeftMotorPwm(0.0);
    double targetDistance = 350;
    double targetSpeed = 0;
    DistanceStraight ds(targetDistance, targetSpeed);

    // 初期値
    int leftCount = Measurer::getLeftCount();
    int rightCount = Measurer::getRightCount();
    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetSpeed value passed to DistanceStraight is 0";
    expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    ds.run();                            // 直進を実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 関数実行後の走行距離
    leftCount = Measurer::getLeftCount();
    rightCount = Measurer::getRightCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);      // 標準出力でWarningを出している
    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

  TEST(DistanceStraightTest, runMinusDistanceZeroPwm)
  {
    // PWMの初期化
    Controller::setRightMotorPwm(0.0);
    Controller::setLeftMotorPwm(0.0);
    double targetDistance = -350;
    double targetSpeed = 0;
    DistanceStraight ds(targetDistance, targetSpeed);

    // 初期値
    int leftCount = Measurer::getLeftCount();
    int rightCount = Measurer::getRightCount();
    // 期待する走行距離
    double expectedDistance = Mileage::calculateMileage(rightCount, leftCount);

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetSpeed value passed to DistanceStraight is 0";
    expectedOutput += "\n\x1b[39m";  // 文字色をデフォルトに戻す

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    ds.run();                            // 直進を実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 関数実行後の走行距離
    leftCount = Measurer::getLeftCount();
    rightCount = Measurer::getRightCount();
    double actualDistance = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);      // 標準出力でWarningを出している
    EXPECT_EQ(expectedDistance, actualDistance);  // 直進前後で走行距離に変化はない
  }

}  // namespace etrobocon2023_test
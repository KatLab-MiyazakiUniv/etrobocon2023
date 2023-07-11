/**
 * @file   ColorLineTracingTest.cpp
 * @brief  ColorLineTracingクラスのテスト
 * @author YKhm20020 bizyutyu
 */

#include "LineTracing.h"
#include "ColorLineTracing.h"
#include "Mileage.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2023_test {
  // 最初3回の色取得で連続して指定色を取得するテストケース
  TEST(ColorLineTracingTest, runToGetFirst)
  {
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = 50000.0;
    double targetBrightness = 50.0;
    int basePwm = 100;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    int initialRightCount = Measurer::getRightCount();
    int initialLeftCount = Measurer::getLeftCount();
    double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);

    srand(9037);  // 3回連続して緑を取得する乱数シード
    cl.run();     // 緑までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = Measurer::getRightCount();
    int leftCount = Measurer::getLeftCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);  // 初期値より少しでも進んでいる
  }

  // 少し走ってから指定色を取得するテストケース
  TEST(ColorLineTracingTest, runLeftEdge)
  {
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    int basePwm = 100;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    int initialRightCount = Measurer::getRightCount();
    int initialLeftCount = Measurer::getLeftCount();
    double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);

    srand(0);  // 最初に識別する色が青ではない乱数シード
    cl.run();  // 青までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = Measurer::getRightCount();
    int leftCount = Measurer::getLeftCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);  // 実行後に少しでも進んでいる
  }

  // 少し走ってから指定色を取得するテストケース
  TEST(ColorLineTracingTest, runRightEdge)
  {
    COLOR targetColor = COLOR::RED;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    int basePwm = 100;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = false;
    ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    int initialRightCount = Measurer::getRightCount();
    int initialLeftCount = Measurer::getLeftCount();
    double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);

    srand(0);  // 最初に識別する色が赤ではない乱数シード
    cl.run();  // 赤までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = Measurer::getRightCount();
    int leftCount = Measurer::getLeftCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_LT(expected, actual);  // 実行後に少しでも進んでいる
  }

  // 少し走ってから指定色を取得するテストケース
  TEST(ColorLineTracingTest, runBackLeftEdge)
  {
    COLOR targetColor = COLOR::YELLOW;
    double targetSpeed = -100.0;
    double targetBrightness = 45.0;
    int basePwm = -100;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    int initialRightCount = Measurer::getRightCount();
    int initialLeftCount = Measurer::getLeftCount();
    double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);

    srand(0);  // 最初に識別する色が黄ではない乱数シード
    cl.run();  // 黄までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = Measurer::getRightCount();
    int leftCount = Measurer::getLeftCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected, actual);  // 実行後に少しでも進んでいる
  }

  // 少し走ってから指定色を取得するテストケース
  TEST(ColorLineTracingTest, runBackRightEdge)
  {
    COLOR targetColor = COLOR::GREEN;
    double targetSpeed = -100.0;
    double targetBrightness = 45.0;
    int basePwm = -100;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = false;
    ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    int initialRightCount = Measurer::getRightCount();
    int initialLeftCount = Measurer::getLeftCount();
    double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);

    srand(0);  // 最初に識別する色が緑ではない乱数シード
    cl.run();  // 緑までライントレースを実行

    // ライントレース後の走行距離
    int rightCount = Measurer::getRightCount();
    int leftCount = Measurer::getLeftCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_GT(expected, actual);  // 実行後に少しでも進んでいる
  }

  TEST(ColorLineTracingTest, runZeroSpeed)
  {
    COLOR targetColor = COLOR::BLUE;
    double targetSpeed = 0.0;
    double targetBrightness = 45.0;
    int basePwm = 100;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    int initialRightCount = Measurer::getRightCount();
    int initialLeftCount = Measurer::getLeftCount();
    double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetSpeed value passed to ColorLineTracing is 0";
    expectedOutput += "\n\x1b[39m";      // 文字色をデフォルトに戻す

    srand(0);  // 最初に識別する色が青ではない乱数シード
    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    cl.run();                            // 青までライントレースを実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // ライントレース後の走行距離
    int rightCount = Measurer::getRightCount();
    int leftCount = Measurer::getLeftCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }

  TEST(ColorLineTracingTest, runNoneColor)
  {
    COLOR targetColor = COLOR::NONE;
    double targetSpeed = 100.0;
    double targetBrightness = 45.0;
    int basePwm = 100;
    PidGain gain = { 0.1, 0.05, 0.05 };
    bool isLeftEdge = true;
    ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

    // 初期値から期待する走行距離を求める
    int initialRightCount = Measurer::getRightCount();
    int initialLeftCount = Measurer::getLeftCount();
    double expected = Mileage::calculateMileage(initialRightCount, initialLeftCount);

    // Warning文
    string expectedOutput = "\x1b[36m";  // 文字色をシアンに
    expectedOutput += "Warning: The targetColor passed to ColorLineTracing is NONE";
    expectedOutput += "\n\x1b[39m";      // 文字色をデフォルトに戻す

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    cl.run();                            // ライントレースを実行
    string actualOutput = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // ライントレース後の走行距離
    int rightCount = Measurer::getRightCount();
    int leftCount = Measurer::getLeftCount();
    double actual = Mileage::calculateMileage(rightCount, leftCount);

    EXPECT_EQ(expectedOutput, actualOutput);  // 標準出力でWarningを出している
    EXPECT_EQ(expected, actual);  // ライントレース前後で走行距離に変化はない
  }
}  // namespace etrobocon2023_test
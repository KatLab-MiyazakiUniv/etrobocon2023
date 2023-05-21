/**
 * @file PidTest.cpp
 * @brief PidGainクラス,Pidクラスをテストする
 * @author Negimarge
 */

#include "Pid.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(PidGainTest, gain)
  {
    double expectedKp = 0.1;
    double expectedKi = 0.03;
    double expectedKd = 0.612;
    PidGain actualPidGain(expectedKp, expectedKi, expectedKd);
    EXPECT_DOUBLE_EQ(expectedKp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expectedKi, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expectedKd, actualPidGain.kd);
  }

  TEST(PidGainTest, gainZero)
  {
    double expectedKp = 0.0;
    double expectedKi = 0.0;
    double expectedKd = 0.0;
    PidGain actualPidGain(expectedKp, expectedKi, expectedKd);
    EXPECT_DOUBLE_EQ(expectedKp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expectedKi, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expectedKd, actualPidGain.kd);
  }

  TEST(PidGainTest, gainMinus)
  {
    double expectedKp = -0.5;
    double expectedKi = -0.2;
    double expectedKd = -0.3;
    PidGain actualPidGain(expectedKp, expectedKi, expectedKd);
    EXPECT_DOUBLE_EQ(expectedKp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expectedKi, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expectedKd, actualPidGain.kd);
  }

  TEST(PidTest, calculatePid)
  {
    constexpr double DELTA = 0.01;
    double kp = 0.6;
    double ki = 0.02;
    double kd = 0.03;
    double targetValue = 70;
    Pid actualPid(kp, ki, kd, targetValue);
    double currentValue = 20;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double expectedP = currentDeviation * kp;
    double expectedI = ((preDeviation + currentDeviation) * DELTA / 2.0) * ki;
    double expectedD = (currentDeviation - preDeviation) * kd / DELTA;
    double expected = expectedP + expectedI + expectedD;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  TEST(PidTest, calculatePidZero)
  {
    constexpr double DELTA = 0.01;
    double kp = 0.0;
    double ki = 0.0;
    double kd = 0.0;
    double targetValue = 0;
    Pid actualPid(kp, ki, kd, targetValue);
    double currentValue = 40;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * kp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2.0) * ki;
    double d = (currentDeviation - preDeviation) * kd / DELTA;
    double expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  TEST(PidTest, calculatePidMinus)
  {
    constexpr double DELTA = 0.01;
    double kp = -0.3;
    double ki = -0.02;
    double kd = -0.175;
    double targetValue = 100;
    Pid actualPid(kp, ki, kd, targetValue);
    double currentValue = 0;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * kp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * ki;
    double d = (currentDeviation - preDeviation) * kd / DELTA;
    double expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  TEST(PidTest, calculatePidChangeDelta)
  {
    constexpr double DELTA = 0.03;
    double kp = 0.6;
    double ki = 0.02;
    double kd = 0.03;
    double targetValue = 70;
    Pid actualPid(kp, ki, kd, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * kp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * ki;
    double d = (currentDeviation - preDeviation) * kd / DELTA;
    double expected = p + i + d;
    // 第2引数に周期を渡し、周期に応じた計算結果を返すことができるかを確認(デフォルトでは0.01が渡される)
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, DELTA));
  }

  TEST(PidTest, calculatePidChangeDeltaMinus)
  {
    constexpr double DELTA = -0.03;
    double kp = 0.6;
    double ki = 0.02;
    double kd = 0.03;
    double targetValue = 70;
    Pid actualPid(kp, ki, kd, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * kp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * ki;
    double d = (currentDeviation - preDeviation) * kd / DELTA;
    double expected = p + i + d;
    // 第2引数に周期を渡し、周期に応じた計算結果を返すことができるかを確認(デフォルトでは0.01が渡される)
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, DELTA));
  }

  // 周期に0を渡したときに、デフォルト周期0.01として計算されるかをテストする
  TEST(PidTest, calculatePidChangeDeltaZero)
  {
    constexpr double DELTA = 0;      // 実際に渡す周期
    constexpr double kdELTA = 0.01;  // 期待される周期
    double kp = 0.6;
    double ki = 0.02;
    double kd = 0.03;
    double targetValue = 70;
    Pid actualPid(kp, ki, kd, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * kp;
    double i = ((preDeviation + currentDeviation) * kdELTA / 2) * ki;
    double d = (currentDeviation - preDeviation) * kd / kdELTA;
    double expected = p + i + d;
    // 第2引数に周期を渡し、周期に応じた計算結果を返すことができるかを確認(デフォルトでは0.01が渡される)
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, DELTA));
  }

  // setしてcalculatePidを呼び出す(Setterのテスト)
  TEST(PidTest, caluclatePidSetter)
  {
    constexpr double DELTA = 0.01;
    double kp = 0.6;
    double ki = 0.05;
    double kd = 0.01;
    double targetValue = 70;
    Pid actualPid(kp, ki, kd, targetValue);
    double currentValue = 60;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);           // 現在の偏差
    double p = currentDeviation * kp;                                 // P制御
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * ki;  // I制御(誤差の累積は0)
    double d = (currentDeviation - preDeviation) * kd / DELTA;  // D制御(前回の誤差は0)
    double expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));

    double integral = (preDeviation + currentDeviation) * DELTA / 2;  // 誤差の累積
    preDeviation = currentDeviation;                                  // 前回の誤差の更新
    kp = 0.1;
    ki = 0.2;
    kd = 0.3;
    actualPid.setPidGain(kp, ki, kd);  // PIDゲインの更新
    currentValue = 100;
    currentDeviation = (targetValue - currentValue);
    integral += (preDeviation + currentDeviation) * DELTA / 2;
    p = currentDeviation * kp;
    i = integral * ki;
    d = (currentDeviation - preDeviation) / DELTA * kd;
    expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

}  // namespace etrobocon2023_test
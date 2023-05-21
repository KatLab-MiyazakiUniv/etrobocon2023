/**
 * @file PidTest.cpp
 * @brief PidGainクラス,Pidクラスをテストする
<<<<<<< HEAD
 * @author Negihara
=======
 * @author Negimarge
>>>>>>> work-2
 */

#include "Pid.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(PidGainTest, gain)
  {
<<<<<<< HEAD
    double expected_p = 0.1;
    double expected_i = 0.03;
    double expected_d = 0.612;
    PidGain actualPidGain(expected_p, expected_i, expected_d);
    EXPECT_DOUBLE_EQ(expected_p, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expected_i, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expected_d, actualPidGain.kd);
=======
    double expectedKp = 0.1;
    double expectedKi = 0.03;
    double expectedKd = 0.612;
    PidGain actualPidGain(expectedKp, expectedKi, expectedKd);
    EXPECT_DOUBLE_EQ(expectedKp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expectedKi, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expectedKd, actualPidGain.kd);
>>>>>>> work-2
  }

  TEST(PidGainTest, gainZero)
  {
<<<<<<< HEAD
    double expected_p = 0.0;
    double expected_i = 0.0;
    double expected_d = 0.0;
    PidGain actualPidGain(expected_p, expected_i, expected_d);
    EXPECT_DOUBLE_EQ(expected_p, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expected_i, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expected_d, actualPidGain.kd);
=======
    double expectedKp = 0.0;
    double expectedKi = 0.0;
    double expectedKd = 0.0;
    PidGain actualPidGain(expectedKp, expectedKi, expectedKd);
    EXPECT_DOUBLE_EQ(expectedKp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expectedKi, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expectedKd, actualPidGain.kd);
>>>>>>> work-2
  }

  TEST(PidGainTest, gainMinus)
  {
<<<<<<< HEAD
    double expected_p = -0.5;
    double expected_i = -0.2;
    double expected_d = -0.3;
    PidGain actualPidGain(expected_p, expected_i, expected_d);
    EXPECT_DOUBLE_EQ(expected_p, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expected_i, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expected_d, actualPidGain.kd);
=======
    double expectedKp = -0.5;
    double expectedKi = -0.2;
    double expectedKd = -0.3;
    PidGain actualPidGain(expectedKp, expectedKi, expectedKd);
    EXPECT_DOUBLE_EQ(expectedKp, actualPidGain.kp);
    EXPECT_DOUBLE_EQ(expectedKi, actualPidGain.ki);
    EXPECT_DOUBLE_EQ(expectedKd, actualPidGain.kd);
>>>>>>> work-2
  }

  TEST(PidTest, calculatePid)
  {
    constexpr double DELTA = 0.01;
<<<<<<< HEAD
    double expected_p = 0.6;
    double expected_i = 0.02;
    double expected_d = 0.03;
    double targetValue = 70;
    Pid actualPid(expected_p, expected_i, expected_d, targetValue);
    double currentValue = 20;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expected_p;
    double i = ((preDeviation + currentDeviation) * DELTA / 2.0) * expected_i;
    double d = (currentDeviation - preDeviation) * expected_d / DELTA;
    double expected = p + i + d;
=======
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
>>>>>>> work-2
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  TEST(PidTest, calculatePidZero)
  {
    constexpr double DELTA = 0.01;
<<<<<<< HEAD
    double expected_p = 0.0;
    double expected_i = 0.0;
    double expected_d = 0.0;
    double targetValue = 0;
    Pid actualPid(expected_p, expected_i, expected_d, targetValue);
    double currentValue = 40;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expected_p;
    double i = ((preDeviation + currentDeviation) * DELTA / 2.0) * expected_i;
    double d = (currentDeviation - preDeviation) * expected_d / DELTA;
=======
    double expectedKp = 0.0;
    double expectedKi = 0.0;
    double expectedKd = 0.0;
    double targetValue = 0;
    Pid actualPid(expectedKp, expectedKi, expectedKd, targetValue);
    double currentValue = 40;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expectedKp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2.0) * expectedKi;
    double d = (currentDeviation - preDeviation) * expectedKd / DELTA;
>>>>>>> work-2
    double expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  TEST(PidTest, calculatePidMinus)
  {
    constexpr double DELTA = 0.01;
<<<<<<< HEAD
    double expected_p = -0.3;
    double expected_i = -0.02;
    double expected_d = -0.175;
    double targetValue = 100;
    Pid actualPid(expected_p, expected_i, expected_d, targetValue);
    double currentValue = 0;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expected_p;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * expected_i;
    double d = (currentDeviation - preDeviation) * expected_d / DELTA;
=======
    double expectedKp = -0.3;
    double expectedKi = -0.02;
    double expectedKd = -0.175;
    double targetValue = 100;
    Pid actualPid(expectedKp, expectedKi, expectedKd, targetValue);
    double currentValue = 0;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expectedKp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * expectedKi;
    double d = (currentDeviation - preDeviation) * expectedKd / DELTA;
>>>>>>> work-2
    double expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

  TEST(PidTest, calculatePidChangeDelta)
  {
    constexpr double DELTA = 0.03;
<<<<<<< HEAD
    double expected_p = 0.6;
    double expected_i = 0.02;
    double expected_d = 0.03;
    double targetValue = 70;
    Pid actualPid(expected_p, expected_i, expected_d, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expected_p;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * expected_i;
    double d = (currentDeviation - preDeviation) * expected_d / DELTA;
=======
    double expectedKp = 0.6;
    double expectedKi = 0.02;
    double expectedKd = 0.03;
    double targetValue = 70;
    Pid actualPid(expectedKp, expectedKi, expectedKd, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expectedKp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * expectedKi;
    double d = (currentDeviation - preDeviation) * expectedKd / DELTA;
>>>>>>> work-2
    double expected = p + i + d;
    // 第2引数に周期を渡し、周期に応じた計算結果を返すことができるかを確認(デフォルトでは0.01が渡される)
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, DELTA));
  }

  TEST(PidTest, calculatePidChangeDeltaMinus)
  {
    constexpr double DELTA = -0.03;
<<<<<<< HEAD
    double expected_p = 0.6;
    double expected_i = 0.02;
    double expected_d = 0.03;
    double targetValue = 70;
    Pid actualPid(expected_p, expected_i, expected_d, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expected_p;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * expected_i;
    double d = (currentDeviation - preDeviation) * expected_d / DELTA;
=======
    double expectedKp = 0.6;
    double expectedKi = 0.02;
    double expectedKd = 0.03;
    double targetValue = 70;
    Pid actualPid(expectedKp, expectedKi, expectedKd, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expectedKp;
    double i = ((preDeviation + currentDeviation) * DELTA / 2) * expectedKi;
    double d = (currentDeviation - preDeviation) * expectedKd / DELTA;
>>>>>>> work-2
    double expected = p + i + d;
    // 第2引数に周期を渡し、周期に応じた計算結果を返すことができるかを確認(デフォルトでは0.01が渡される)
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, DELTA));
  }

  // 周期に0を渡したときに、デフォルト周期0.01として計算されるかをテストする
  TEST(PidTest, calculatePidChangeDeltaZero)
  {
    constexpr double DELTA = 0;              // 実際に渡す周期
<<<<<<< HEAD
    constexpr double EXPECTED_DELTA = 0.01;  // 期待される周期
    double expected_p = 0.6;
    double expected_i = 0.02;
    double expected_d = 0.03;
    double targetValue = 70;
    Pid actualPid(expected_p, expected_i, expected_d, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expected_p;
    double i = ((preDeviation + currentDeviation) * EXPECTED_DELTA / 2) * expected_i;
    double d = (currentDeviation - preDeviation) * expected_d / EXPECTED_DELTA;
=======
    constexpr double expectedKdELTA = 0.01;  // 期待される周期
    double expectedKp = 0.6;
    double expectedKi = 0.02;
    double expectedKd = 0.03;
    double targetValue = 70;
    Pid actualPid(expectedKp, expectedKi, expectedKd, targetValue);
    double currentValue = 55;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);
    double p = currentDeviation * expectedKp;
    double i = ((preDeviation + currentDeviation) * expectedKdELTA / 2) * expectedKi;
    double d = (currentDeviation - preDeviation) * expectedKd / expectedKdELTA;
>>>>>>> work-2
    double expected = p + i + d;
    // 第2引数に周期を渡し、周期に応じた計算結果を返すことができるかを確認(デフォルトでは0.01が渡される)
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue, DELTA));
  }

  // setしてcalculatePidを呼び出す(Setterのテスト)
  TEST(PidTest, caluclatePidSetter)
  {
    constexpr double DELTA = 0.01;
<<<<<<< HEAD
    double expected_p = 0.6;
    double expected_i = 0.05;
    double expected_d = 0.01;
    double targetValue = 70;
    Pid actualPid(expected_p, expected_i, expected_d, targetValue);
    double currentValue = 60;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);              // 現在の偏差
    double p = currentDeviation * expected_p;                            // P制御
    double i
        = ((preDeviation + currentDeviation) * DELTA / 2) * expected_i;  // I制御(誤差の累積は0)
    double d = (currentDeviation - preDeviation) * expected_d / DELTA;  // D制御(前回の誤差は0)
=======
    double expectedKp = 0.6;
    double expectedKi = 0.05;
    double expectedKd = 0.01;
    double targetValue = 70;
    Pid actualPid(expectedKp, expectedKi, expectedKd, targetValue);
    double currentValue = 60;
    double preDeviation = 0;
    double currentDeviation = (targetValue - currentValue);              // 現在の偏差
    double p = currentDeviation * expectedKp;                            // P制御
    double i
        = ((preDeviation + currentDeviation) * DELTA / 2) * expectedKi;  // I制御(誤差の累積は0)
    double d = (currentDeviation - preDeviation) * expectedKd / DELTA;  // D制御(前回の誤差は0)
>>>>>>> work-2
    double expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));

    double integral = (preDeviation + currentDeviation) * DELTA / 2;  // 誤差の累積
<<<<<<< HEAD
    preDeviation = currentDeviation;            // 前回の誤差の更新
    expected_p = 0.1;
    expected_i = 0.2;
    expected_d = 0.3;
    actualPid.setPidGain(expected_p, expected_i, expected_d);  // PIDゲインの更新
    currentValue = 100;
    currentDeviation = (targetValue - currentValue);
    integral += (preDeviation + currentDeviation) * DELTA / 2;
    p = currentDeviation * expected_p;
    i = integral * expected_i;
    d = (currentDeviation - preDeviation) / DELTA * expected_d;
=======
    preDeviation = currentDeviation;                                  // 前回の誤差の更新
    expectedKp = 0.1;
    expectedKi = 0.2;
    expectedKd = 0.3;
    actualPid.setPidGain(expectedKp, expectedKi, expectedKd);  // PIDゲインの更新
    currentValue = 100;
    currentDeviation = (targetValue - currentValue);
    integral += (preDeviation + currentDeviation) * DELTA / 2;
    p = currentDeviation * expectedKp;
    i = integral * expectedKi;
    d = (currentDeviation - preDeviation) / DELTA * expectedKd;
>>>>>>> work-2
    expected = p + i + d;
    EXPECT_DOUBLE_EQ(expected, actualPid.calculatePid(currentValue));
  }

}  // namespace etrobocon2023_test
/**
 * @file MeasurerTest.cpp
 * @brief Measurerクラスをテストする
 * @author YKhm20020
 */

#include "Measurer.h"
#include <gtest/gtest.h>

// rgb_raw_tの比較用関数
bool eqRgb(rgb_raw_t rgb1, rgb_raw_t rgb2)
{
  return rgb1.r == rgb2.r && rgb1.g == rgb2.g && rgb1.b == rgb2.b;
}

namespace etrobocon2023_test {
  TEST(MeasurerTest, getRawColor)
  {
    rgb_raw_t expected1 = { 8, 9, 10 };       // black
    rgb_raw_t expected2 = { 104, 101, 146 };  // white
    rgb_raw_t expected3 = { 111, 19, 19 };    // red
    rgb_raw_t expected4 = { 120, 108, 71 };   // yellow
    rgb_raw_t expected5 = { 4, 75, 35 };      // green
    rgb_raw_t expected6 = { 81, 92, 144 };    // blue
    rgb_raw_t actual = Measurer::getRawColor();

    ASSERT_TRUE(eqRgb(actual, expected1) || eqRgb(actual, expected2) || eqRgb(actual, expected3)
                || eqRgb(actual, expected4) || eqRgb(actual, expected5)
                || eqRgb(actual, expected6));
  }

  TEST(MeasurerTest, getBrightness)
  {
    /**
    getRawColorの基準となるRGB値

    rgb_raw_t expectedRgb1 = { 8, 9, 10 };       // black
    rgb_raw_t expectedRgb2 = { 104, 101, 146 };  // white
    rgb_raw_t expectedRgb3 = { 111, 19, 19 };    // red
    rgb_raw_t expectedRgb4 = { 120, 108, 71 };   // yellow
    rgb_raw_t expectedRgb5 = { 4, 75, 35 };      // green
    rgb_raw_t expectedRgb6 = { 81, 92, 144 };    // blue
    **/

    // 各色に対応する明度
    int expected1 = 3;
    int expected2 = 57;
    int expected3 = 43;
    int expected4 = 47;
    int expected5 = 29;
    int expected6 = 56;
    int actual = Measurer::getBrightness();
    ASSERT_TRUE(actual == expected1 || actual == expected2 || actual == expected3
                || actual == expected4 || actual == expected5 || actual == expected6);
  }

  TEST(MeasurerTest, getLeftCount)
  {
    Measurer::leftMotor->reset();
    int expected = 0;
    int actual = Measurer::getLeftCount();

    EXPECT_EQ(expected, actual);
  }

  TEST(MeasurerTest, getRightCount)
  {
    Measurer::rightMotor->reset();
    int expected = 0;
    int actual = Measurer::getRightCount();

    EXPECT_EQ(expected, actual);
  }

  TEST(MeasurerTest, getArmMotorCount)
  {
    Measurer::armMotor->reset();
    int expected = 0;
    int actual = Measurer::getArmMotorCount();

    EXPECT_EQ(expected, actual);
  }
}  // namespace etrobocon2023_test
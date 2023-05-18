/**
 * @file MileageTest.cpp
 * @brief 走行距離の測定用クラスのテスト
 * @author TanakaShinnosuke
 */

#include "Mileage.h"
#include <math.h>
#include <gtest/gtest.h>

namespace etrobocon2022_test {
  TEST(MileageTest, calculateMileage)
  {
    double radius = 50;
    double rightAngle = 30;
    double leftAngle = 40;
    double rightWheel = 2 * rightAngle * radius * M_PI / 360;
    double leftWheel = 2 * leftAngle * radius * M_PI / 360;
    double expected = (rightWheel + leftWheel) / 2;
    double actual = Mileage::calculateMileage(rightAngle, leftAngle);
    EXPECT_DOUBLE_EQ(expected, actual);
  }

  TEST(MileageTest, calculateMileage_minus)
  {
    double radius = 50;
    double rightAngle = -30;
    double leftAngle = -40;
    double rightWheel = 2 * rightAngle * radius * M_PI / 360;
    double leftWheel = 2 * leftAngle * radius * M_PI / 360;
    double expected = (rightWheel + leftWheel) / 2;
    double actual = Mileage::calculateMileage(rightAngle, leftAngle);
    EXPECT_DOUBLE_EQ(expected, actual);
  }

  TEST(MileageTest, calculateMileage_zero)
  {
    double radius = 50;
    double rightAngle = 0;
    double leftAngle = 0;
    double rightWheel = 2 * rightAngle * radius * M_PI / 360;
    double leftWheel = 2 * leftAngle * radius * M_PI / 360;
    double expected = (rightWheel + leftWheel) / 2;
    double actual = Mileage::calculateMileage(rightAngle, leftAngle);
    EXPECT_DOUBLE_EQ(expected, actual);
  }
}  // namespace etrobocon2022_test
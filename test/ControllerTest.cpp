/**
 * @file ControllerTest.cpp
 * @brief Controllerクラスをテストする
 * @author miyashita64 bizyutu
 */

#include "ev3api.h"
#include "Controller.h"
#include "Measurer.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(ControllerTest, setRightMotorPwm)
  {
    const int pwm = 50;
    int initCount = Measurer::getRightCount();
    Controller::setRightMotorPwm(pwm);
    int currentCount = Measurer::getRightCount();
    EXPECT_LT(initCount, currentCount);
  }

  TEST(ControllerTest, setRightMotorMinusPwm)
  {
    const int pwm = -30;
    int initCount = Measurer::getRightCount();
    Controller::setRightMotorPwm(pwm);
    int currentCount = Measurer::getRightCount();
    EXPECT_GT(initCount, currentCount);
  }

  TEST(ControllerTest, setLeftMotorPwm)
  {
    const int pwm = 50;
    int initCount = Measurer::getLeftCount();
    Controller::setLeftMotorPwm(pwm);
    int currentCount = Measurer::getLeftCount();
    EXPECT_LT(initCount, currentCount);
  }

  TEST(ControllerTest, setLeftMotorMinusPwm)
  {
    const int pwm = -150;
    int initCount = Measurer::getLeftCount();
    Controller::setLeftMotorPwm(pwm);
    int currentCount = Measurer::getLeftCount();
    EXPECT_GT(initCount, currentCount);
  }

  TEST(ControllerTest, stopMotor)
  {
    Controller::stopMotor();
    SUCCEED();
  }

  TEST(ControllerTest, setArmMotorPwm)
  {
    const int pwm = 50;
    int initCount = Measurer::getArmMotorCount();
    Controller::setArmMotorPwm(pwm);
    int currentCount = Measurer::getArmMotorCount();
    EXPECT_LT(initCount, currentCount);
  }

  TEST(ControllerTest, stopArmMotor)
  {
    Controller::stopArmMotor();
    SUCCEED();
  }

}  // namespace etrobocon2023_test
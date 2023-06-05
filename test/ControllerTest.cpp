/**
 * @file ControllerTest.cpp
 * @brief Controllerクラスをテストする
 * @author miyashita64 bizyutu
 */

#include "ev3api.h"
#include "Controller.h"
#include <gtest/gtest.h>

namespace etrobocon2023_test {
  TEST(ControllerTest, setRightMotorPwm)
  {
    const int pwm = 50;
    ev3api::Motor _rightWheel(Controller::rightWheelPort);
    ev3api::Motor _leftWheel(Controller::leftWheelPort);
    ev3api::Motor _armMotor(Controller::armMotorPort);
    Controller::rightMotor = &_rightWheel;
    Controller::leftMotor = &_leftWheel;
    Controller::armMotor = &_armMotor;
    Controller::setRightMotorPwm(pwm);
    Controller::rightMotor->reset();
    SUCCEED();
  }

  TEST(ControllerTest, setLeftMotorPwm)
  {
    const int pwm = 50;
    Controller::setLeftMotorPwm(pwm);
    Controller::leftMotor->reset();
    SUCCEED();
  }

  TEST(ControllerTest, stopMotor)
  {
    Controller::stopMotor();
    SUCCEED();
  }

  TEST(ControllerTest, setArmMotorPwm)
  {
    const int pwm = 50;
    Controller::setArmMotorPwm(pwm);
    Controller::armMotor->reset();
    SUCCEED();
  }

  TEST(ControllerTest, stopArmMotor)
  {
    Controller::stopArmMotor();
    SUCCEED();
  }

}  // namespace etrobocon2023_test
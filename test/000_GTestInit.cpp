/**
 * @file 000_GTestInit.cpp
 * @brief GoogleTest時に必要な初期化処理を行う
 * @author miyashita64
 */

#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Motor.h"
#include "Controller.h"
#include "Measurer.h"
#include <gtest/gtest.h>

using namespace std;

namespace etrobocon2023_test {

  TEST(GTestInit, init)
  {
    const ePortS colorSensorPort = PORT_2;
    const ePortS sonarSensorPort = PORT_3;
    const ePortM armMotorPort = PORT_A;
    const ePortM rightMotorPort = PORT_B;
    const ePortM leftMotorPort = PORT_C;

    ev3api::ColorSensor _colorSensor(PORT_2);
    ev3api::SonarSensor _sonarSensor(PORT_3);
    ev3api::Motor _rightMotor(rightMotorPort);
    ev3api::Motor _leftMotor(leftMotorPort);
    ev3api::Motor _armMotor(armMotorPort);

    Controller::rightMotor = &_rightMotor;
    Controller::leftMotor = &_leftMotor;
    Controller::armMotor = &_armMotor;
    Measurer::colorSensor = &_colorSensor;
    Measurer::sonarSensor = &_sonarSensor;
    Measurer::rightMotor = &_rightMotor;
    Measurer::leftMotor = &_leftMotor;
    Measurer::armMotor = &_armMotor;

    SUCCEED();
  }
}  // namespace etrobocon2023_test
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

    ev3api::ColorSensor* _colorSensorPtr = new ev3api::ColorSensor(colorSensorPort);
    ev3api::SonarSensor* _sonarSensorPtr = new ev3api::SonarSensor(sonarSensorPort);
    ev3api::Motor* _rightMotorPtr = new ev3api::Motor(rightMotorPort);
    ev3api::Motor* _leftMotorPtr = new ev3api::Motor(leftMotorPort);
    ev3api::Motor* _armMotorPtr = new ev3api::Motor(armMotorPort);

    Controller::rightMotor = _rightMotorPtr;
    Controller::leftMotor = _leftMotorPtr;
    Controller::armMotor = _armMotorPtr;
    Measurer::colorSensor = _colorSensorPtr;
    Measurer::sonarSensor = _sonarSensorPtr;
    Measurer::rightMotor = _rightMotorPtr;
    Measurer::leftMotor = _leftMotorPtr;
    Measurer::armMotor = _armMotorPtr;

    SUCCEED();
  }
}  // namespace etrobocon2023_test
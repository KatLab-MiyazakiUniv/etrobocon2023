/**
 * @file   EtRobocon2023.cpp
 * @brief  全体を制御するクラス
 * @author KakinokiKanta miyashita64 bizyutyu
 */

#include "EtRobocon2023.h"
// ev3api.hをインクルードしているものは.cppに書く
#include "Measurer.h"
#include "Controller.h"
#include "ev3api.h"
#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "Timer.h"

void EtRobocon2023::start()
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
  ev3api::Clock* _clockPtr = new ev3api::Clock();

  Controller::rightMotor = _rightMotorPtr;
  Controller::leftMotor = _leftMotorPtr;
  Controller::armMotor = _armMotorPtr;
  Measurer::colorSensor = _colorSensorPtr;
  Measurer::sonarSensor = _sonarSensorPtr;
  Measurer::rightMotor = _rightMotorPtr;
  Measurer::leftMotor = _leftMotorPtr;
  Measurer::armMotor = _armMotorPtr;
  Timer::clock = _clockPtr;
}
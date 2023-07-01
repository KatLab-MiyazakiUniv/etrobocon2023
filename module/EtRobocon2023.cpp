/**
 * @file   EtRobocon2023.cpp
 * @brief  全体を制御するクラス
 * @author KakinokiKanta miyashita64 bizyutyu
 */

#include "EtRobocon2023.h"
// ev3api.hをインクルードしているものは.cppに書く
#include "ev3api.h"
#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Motor.h"
#include "Controller.h"
#include "Measurer.h"

void EtRobocon2023::start()
{
  const ePortS colorSensorPort = PORT_2;
  const ePortS sonarSensorPort = PORT_3;
  const ePortM armMotorPort = PORT_A;
  const ePortM rightMotorPort = PORT_B;
  const ePortM leftMotorPort = PORT_C;

  ev3api::ColorSensor _colorSensor(colorSensorPort);
  ev3api::SonarSensor _sonarSensor(sonarSensorPort);
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
}
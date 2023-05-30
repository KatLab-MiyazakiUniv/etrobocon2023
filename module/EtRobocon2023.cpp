/**
 * @file   EtRobocon2023.cpp
 * @brief  全体を制御するクラス
 * @author KakinokiKanta miyashita64 bizyutyu
 */

#include "EtRobocon2023.h"
// ev3api.hをインクルードしているものは.cppに書く
#include "ev3api.h"
#include "Motor.h"
#include "Controller.h"

void EtRobocon2023::start()
{
  ev3api::Motor _rightWheel(Controller::rightWheelPort);
  ev3api::Motor _leftWheel(Controller::leftWheelPort);
  ev3api::Motor _armMotor(Controller::armMotorPort);
  Controller::rightMotor = &_rightWheel;
  Controller::leftMotor = &_leftWheel;
  Controller::armMotor = &_armMotor;
}
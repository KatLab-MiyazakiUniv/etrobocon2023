/**
 * @file   EtRobocon2023.cpp
 * @brief  全体を制御するクラス
 * @author KakinokiKanta
 */

#include "EtRobocon2023.h"
#include "SpeedCalculator.h"
#include "Timer.h"
#include "Motor.h"
#include "ev3api.h"
#include "Controller.h"

void EtRobocon2023::start()
{
  Timer timer;
  timer.sleep(3000);
  ev3api::Motor _rightWheel(Controller::rightWheelPort);
  ev3api::Motor _leftWheel(Controller::leftWheelPort);
  ev3api::Motor _armMotor(Controller::armMotorPort);
  Controller::rightMotor = &_rightWheel;
  Controller::leftMotor = &_leftWheel;
  Controller::armMotor = &_armMotor;
  int time = timer.now();
  SpeedCalculator sc(300);
  while(time + 2000 > timer.now()){
    int pwm = sc.calcPwmFromSpeed();
    printf("pwm=%d\n", pwm);
    Controller::setRightMotorPwm(pwm);
    Controller::setLeftMotorPwm(pwm);
    timer.sleep();
  }
  Controller::stopMotor();
}
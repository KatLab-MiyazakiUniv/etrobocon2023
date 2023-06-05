/**
 * @file Controller.cpp
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author aridome222 miyahita64 bizyutyu
 */
#include "Controller.h"

ev3api::Motor* Controller::rightMotor = nullptr;
ev3api::Motor* Controller::leftMotor = nullptr;
ev3api::Motor* Controller::armMotor = nullptr;

int Controller::limitPwmValue(const int value)
{
  if(value > MOTOR_PWM_MAX) {
    return MOTOR_PWM_MAX;
  } else if(value < MOTOR_PWM_MIN) {
    return MOTOR_PWM_MIN;
  }
  return value;
}

// 右モータにPWM値をセット
void Controller::setRightMotorPwm(const int pwm)
{
  rightMotor->setPWM(limitPwmValue(pwm));
}

// 左モータにPWM値をセット
void Controller::setLeftMotorPwm(const int pwm)
{
  leftMotor->setPWM(limitPwmValue(pwm));
}

// タイヤのモータを停止する
void Controller::stopMotor()
{
  rightMotor->stop();
  leftMotor->stop();
}

// アームのモータにPWM値をセット
void Controller::setArmMotorPwm(const int pwm)
{
  armMotor->setPWM(limitPwmValue(pwm));
}

// アームのモータを停止する
void Controller::stopArmMotor()
{
  armMotor->stop();
}
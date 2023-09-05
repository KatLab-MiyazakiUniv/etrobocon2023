/**
 * @file Controller.cpp
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author aridome222 miyahita64 bizyutyu
 */
#include "Controller.h"

ev3api::Motor* Controller::rightMotor = nullptr;
ev3api::Motor* Controller::leftMotor = nullptr;
ev3api::Motor* Controller::armMotor = nullptr;
// PWMの初期化
double Controller::manageRightPwm = 0.0;
double Controller::manageLeftPwm = 0.0;
double Controller::manageArmPwm = 0.0;

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
void Controller::setRightMotorPwm(const double pwm)
{
  manageRightPwm = pwm;
  rightMotor->setPWM(limitPwmValue(int(pwm)));
}

// 左モータにPWM値をセット
void Controller::setLeftMotorPwm(const double pwm)
{
  manageLeftPwm = pwm;
  leftMotor->setPWM(limitPwmValue(int(pwm)));
}

// タイヤのモータを停止する
void Controller::stopMotor()
{
  manageRightPwm = 0.0;
  manageLeftPwm = 0.0;
  rightMotor->stop();
  leftMotor->stop();
}

// アームのモータにPWM値をセット
void Controller::setArmMotorPwm(const double pwm)
{
  manageArmPwm = pwm;
  armMotor->setPWM(limitPwmValue(int(pwm)));
}

// アームのモータを停止する
void Controller::stopArmMotor()
{
  manageArmPwm = 0.0;
  armMotor->stop();
}

// 右タイヤのPWMを取得する
double Controller::getRightPwm()
{
  return manageRightPwm;
}

// 左タイヤのPWMを取得する
double Controller::getLeftPwm()
{
  return manageLeftPwm;
}
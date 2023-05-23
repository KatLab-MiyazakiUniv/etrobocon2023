/**
 * @file Controller.cpp
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author aridome222
 */
#include "Controller.h"

// Controller::rightWheel = PORT_B;
// Controller::leftWheel = PORT_C
// Controller::armMotor = PORT_A:
// Controller::Controller() : rightWheel(PORT_B), leftWheel(PORT_C), armMotor(PORT_A) {}

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
  ev3api::Motor(rightWheelPort).setPWM(limitPwmValue(pwm));
}

// 左モータにPWM値をセット
void Controller::setLeftMotorPwm(const int pwm)
{
  ev3api::Motor(leftWheelPort).setPWM(limitPwmValue(pwm));
}

// タイヤのモータを停止する
void Controller::stopMotor()
{
  ev3api::Motor(leftWheelPort).stop();
  ev3api::Motor(rightWheelPort).stop();
}

// アームのモータにPWM値をセット
void Controller::setArmMotorPwm(const int pwm)
{
  ev3api::Motor(armMotorPort).setPWM(limitPwmValue(pwm));
}

// アームのモータを停止する
void Controller::stopArmMotor()
{
  ev3api::Motor(armMotorPort).stop();
}

// 自タスクスリープ（デフォルトは10ミリ秒）
void Controller::sleep(int milliSec)
{
  // clock.sleep()はマイクロ秒指定なので，単位を合わせて呼び出す
  Controller::clock.sleep(milliSec * 1000);
}
/**
 * @file Controller.h
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author aridome222
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ev3api.h"
#include "Motor.h"

class Controller {
 public:
  Controller();  // 明示的にインスタンス化を禁止

  /**
   * タイヤのモータにPWM値をセット
   * @param pwm PWM値
   */
  void setRightMotorPwm(const int pwm);
  void setLeftMotorPwm(const int pwm);

  /**
   * タイヤのモータを停止する
   */
  void stopMotor();

  /**
   * アームのモータにPWM値をセット
   * @param pwm PWM値
   */
  void setArmMotorPwm(const int pwm);

  /**
   * アームのモータを停止する
   */
  void stopArmMotor();

 private:
  
  ev3api::Motor rightMotor;
  int MOTOR_PWM_MAX = 100;
  int MOTOR_PWM_MIN = -100;

  ePortM rightWheelPort = PORT_B;
  ePortM leftWheelPort = PORT_A;
  ePortM armMotorPort = PORT_C;

  /**
   * モータに設定するPWM値の制限
   * @param value 入力されたPWM値
   * @return 制限されたPWM値
   */
  int limitPwmValue(const int value);
};

#endif
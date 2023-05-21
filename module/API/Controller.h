/**
 * @file Controller.h
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author aridome222
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ev3api.h"
#include "Motor.h"
#include "Clock.h"

class Controller {
 public:
  Controller();
  Controller(const Controller&) = delete;  // 明示的にインスタンス化を禁止

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

  /**
   * 自タスクスリープ（デフォルトは10ミリ秒）
   * @param milliSec スリープ時間(ミリ秒)
   */
  void sleep(int milliSec = 10);

 private:
  static const int MOTOR_PWM_MAX = 100;
  static const int MOTOR_PWM_MIN = -100;

  ev3api::Motor rightWheel;
  ev3api::Motor leftWheel;
  ev3api::Motor armMotor;
  ev3api::Clock clock;

  /**
   * モータに設定するPWM値の制限
   * @param value 入力されたPWM値
   * @return 制限されたPWM値
   */
  int limitPwmValue(const int value);
};

#endif
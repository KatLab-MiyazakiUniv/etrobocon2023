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
  Controller() = delete;  // 明示的にインスタンス化を禁止

  /**
   * タイヤのモータにPWM値をセット
   * @param pwm PWM値
   */
  static void setRightMotorPwm(const int pwm);
  static void setLeftMotorPwm(const int pwm);

  /**
   * タイヤのモータを停止する
   */
  static void stopMotor();

  /**
   * アームのモータにPWM値をセット
   * @param pwm PWM値
   */
  static void setArmMotorPwm(const int pwm);

  /**
   * アームのモータを停止する
   */
  static void stopArmMotor();

  /**
   * 自タスクスリープ（デフォルトは10ミリ秒）
   * @param milliSec スリープ時間(ミリ秒)
   */
  static void sleep(int milliSec = 10);

 private:
  static const int MOTOR_PWM_MAX = 100;
  static const int MOTOR_PWM_MIN = -100;

  static const ePortM rightWheelPort = PORT_B;
  static const ePortM leftWheelPort = PORT_A;
  static const ePortM armMotorPort = PORT_C;
  static ev3api::Clock clock;

  /**
   * モータに設定するPWM値の制限
   * @param value 入力されたPWM値
   * @return 制限されたPWM値
   */
  static int limitPwmValue(const int value);
};

#endif
/**
 * @file Controller.h
 * @brief モーター制御に用いる関数をまとめたラッパークラス
 * @author aridome222 miyashita64 bizyutyu
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ev3api.h"
#include "Motor.h"

class Controller {
 public:
  static ev3api::Motor* rightMotor;
  static ev3api::Motor* leftMotor;
  static ev3api::Motor* armMotor;

  Controller() = delete;  // 明示的にインスタンス化を禁止

  /**
   * タイヤのモータにPWM値をセット
   * @param pwm PWM値
   */
  static void setRightMotorPwm(const double pwm);
  static void setLeftMotorPwm(const double pwm);

  /**
   * タイヤのモータを停止する
   */
  static void stopMotor();

  /**
   * アームのモータにPWM値をセット
   * @param pwm PWM値
   */
  static void setArmMotorPwm(const double pwm);

  /**
   * アームのモータを停止する
   */
  static void stopArmMotor();

  /**
   * @brief 右タイヤのPWMを取得する
   * @return 右タイヤのPWM
   */
  static double getRightPwm();

  /**
   * @brief 左タイヤのPWMを取得する
   * @return 左タイヤのPWM
   */
  static double getLeftPwm();

 private:
  static const int MOTOR_PWM_MAX = 100;
  static const int MOTOR_PWM_MIN = -100;
  static double manageRightPwm;  // 右タイヤPWM
  static double manageLeftPwm;   // 左タイヤPWM
  static double manageArmPwm;    // アームPWM

  /**
   * モータに設定するPWM値の制限
   * @param value 入力されたPWM値
   * @return 制限されたPWM値
   */
  static int limitPwmValue(const int value);
};

#endif
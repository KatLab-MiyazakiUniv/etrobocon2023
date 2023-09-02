/**
 * @file   Rotation.h
 * @brief  Pwm指定回頭動作
 * @author Negimarge
 */

#ifndef ROTATION_H
#define ROTATION_H

#include "Motion.h"
#include "Mileage.h"
#include "Timer.h"
#include "SystemInfo.h"

class PwmRotation : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _angle 回転角度(deg) 0~360
   * @param _pwm PWM値 0~100
   * @param _isClockwise 回頭方向 ture:時計回り, false:反時計回り
   */
  PwmRotation(int _angle, int _pwm, bool _isClockwise);

  /**
   * @brief 回頭する
   */
  void run();

  /**
   * @brief 実行のログを取る
   */
  void logRunning();

 private:
  int angle;         // 回転角度(deg) 0~360
  int pwm;           // PWM値 0~100
  bool isClockwise;  // 回頭方向 ture:時計回り, false:反時計回り
  Timer timer;
};

#endif
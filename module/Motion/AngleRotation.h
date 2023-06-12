/**
 * @file   AngleRotation.h
 * @brief  角度指定による回頭動作
 * @author desty505
 */

#ifndef ROTATION_H
#define ROTATION_H

#include "Motion.h"
#include "Mileage.h"
#include "SystemInfo.h"

class AngleRotation : public Rotation {
 public:
  /**
   * コンストラクタ
   * @param _angle 回転角度(deg) 0~360
   * @param _targetSpeed 目標速度
   * @param _isClockwise 回頭方向 ture:時計回り, false:反時計回り
   */
  AngleRotation(int _angle, double _targetSpeed, bool _isClockwise);

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
  int targetSpeed;   // 目標速度
  bool isClockwise;  // 回頭方向 ture:時計回り, false:反時計回り
};
#endif
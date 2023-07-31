/**
 * @file   CorrectingRotation.h
 * @brief  角度補正回頭
 * @author aridome222
 */

#ifndef CORRECTING_ROTATION_H
#define CORRECTING_ROTATION_H

#include "Motion.h"
#include "Mileage.h"
#include "Timer.h"
#include "SpeedCalculator.h"
#include "StringOperator.h"
#include "SystemInfo.h"

class CorrectingRotation : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _targetAngle 目標角度(deg) 0~89
   * @param _targetSpeed 目標速度[mm/s]
   */
  CorrectingRotation(int _targetAngle, double _targetSpeed);

  /**
   * @brief 角度補正回頭する
   */
  void run();

  /**
   * @brief 実行のログを取る
   */
  void logRunning();

 private:
  static constexpr int NO_CORRECTION_ANGLE = 2;  // 補正免除角度(deg)
  int targetAngle;                               // 目標角度(deg) 0~89
  int targetSpeed;                               // 目標速度[mm/s]
  Timer timer;
};

#endif
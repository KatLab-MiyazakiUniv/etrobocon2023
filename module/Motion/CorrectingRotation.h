/**
 * @file   CorrectingRotation.h
 * @brief  角度補正回頭
 * @author aridome222
 */

#ifndef CORRECTING_ROTATION_H
#define CORRECTING_ROTATION_H

#include "CompositeMotion.h"
#include "StringOperator.h"
#include "AngleRotation.h"

class CorrectingRotation : public CompositeMotion {
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
  void run() override;

  /**
   * @brief 角度補正回頭する際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  static constexpr int NO_CORRECTION_ANGLE = 2;  // 補正免除角度(deg)
  int targetAngle;                               // 目標角度(deg) 0~89
  double targetSpeed;                            // 目標速度[mm/s]
  Timer timer;
};

#endif
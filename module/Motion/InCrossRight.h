/**
 * @file InCrossRight.h
 * @brief 交点内移動（右折）のクラス
 * @author YKhm20020
 */

#ifndef IN_CROSS_RIGHT_H
#define IN_CROSS_RIGHT_H

#include "PwmRotation.h"
#include "DistanceStraight.h"
#include "EdgeChanging.h"
#include "BlockAreaMotion.h"

class InCrossRight : public BlockAreaMotion {
 public:
  /**
   * コンストラクタ
   */
  InCrossRight(bool& _isLeftEdge);

  /**
   * @brief 交点サークル上を右に進む
   */
  void run() override;

  /**
   * @brief 右に進む際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  double targetDistance = 65.0;  // 目標距離
  double dsTargetSpeed = 200;    // 距離指定直進の目標速度 [mm/s]
  int targetAngle = 74;          // 目標回頭角度
  int prPwm = 70;               // 角度指定回頭の目標PWM
  bool isClockwise = true;       // 回頭の方向は右回り
  bool& isLeftEdge;
};

#endif
/**
 * @file InCrossRight.h
 * @brief 交点内移動（右折）のクラス
 * @author YKhm20020
 */

#ifndef IN_CROSS_RIGHT_H
#define IN_CROSS_RIGHT_H

#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "EdgeChanging.h"
#include "BlockAreaMotion.h"

class InCrossRight : public BlockAreaMotion {
 public:
  /**
   * コンストラクタ
   */
  InCrossRight(double _targetDistance, double _targetSpeed, int _targetAngle);

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
  double targetDistance = 25.0;  // 目標距離
  double targetSpeed;            // 目標速度 [mm/s]
  int targetAngle = 74;          // 目標回頭角度
  bool isClockwise = true;       // 回頭の方向は右回り
  bool isLeftEdge = false;       // 回頭前のエッジは右
  bool nextEdge = true;          // 回頭後のエッジを左に変更
};

#endif
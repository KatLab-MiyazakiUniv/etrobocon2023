/**
 * @file InCrossLeft.h
 * @brief 交点内移動（左折）のクラス
 * @author YKhm20020
 */

#ifndef IN_CROSS_LEFT_H
#define IN_CROSS_LEFT_H

#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "EdgeChanging.h"
#include "BlockMotion.h"

class InCrossLeft : public BlockMotion {
 public:
  InCrossLeft(double _targetDistance, double _targetSpeed, int _targetAngle);

  /**
   * @brief 交点サークル上を左に進む
   */
  void run() override;

  /**
   * @brief 左に進む際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  double targetDistance = 25.0;  // 目標距離
  double targetSpeed;            // 目標速度 [mm/s]
  int targetAngle = 74;          // 目標ピボットターン角度
  bool isClockwise = false;      // ピボットターンの方向は左回り
  bool isLeftEdge = true;        // ピボットターン前のエッジは左
  bool nextEdge = false;         // ピボットターン後のエッジを右に変更
};

#endif
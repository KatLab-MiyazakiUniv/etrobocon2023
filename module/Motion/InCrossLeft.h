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
#include "BlockAreaMotion.h"

class InCrossLeft : public BlockAreaMotion {
 public:
  /**
   * コンストラクタ
   */
  InCrossLeft(double _targetDistance, double _dsTargetSpeed, double _arTargetSpeed,
              int _targetAngle);

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
  double targetDistance;     // 目標距離 (一昨年は25.0)
  double dsTargetSpeed;      // 距離指定直進の目標速度 [mm/s]
  double arTargetSpeed;      // 角度指定回頭の目標速度 [mm/s]
  int targetAngle;           // 目標回頭角度 (一昨年は74)
  bool isClockwise = false;  // 回頭の方向は左回り
  bool isLeftEdge = true;    // 回頭前のエッジは左
  bool nextEdge = false;     // 回頭後のエッジを右に変更
};

#endif
/**
 * @file InCrossStraight.h
 * @brief 交点内移動（直進）のクラス
 * @author YKhm20020
 */

#ifndef IN_CROSS_STRAIGHT_H
#define IN_CROSS_STRAIGHT_H

#include "DistanceStraight.h"
#include "BlockMotion.h"

class InCrossStraight : public BlockMotion {
 public:
  // コンストラクタ
  InCrossStraight(double _targetSpeed);

  /**
   * @brief 交点サークル上を前に進む
   */
  void run() override;

  /**
   * @brief 直進する際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  double targetDistance = 100.0;  // 交点サークルの外径
  double targetSpeed;             // 目標速度[mm/s]
};

#endif
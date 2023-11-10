/**
 * @file CrossToCross.h
 * @brief 交点から交点までライントレースするクラス
 * @author YKhm20020
 */

#ifndef CROSS_TO_CROSS_H
#define CROSS_TO_CROSS_H

#include "ColorLineTracing.h"
#include "ColorJudge.h"
#include "Measurer.h"
#include "BlockAreaMotion.h"

class CrossToCross : public BlockAreaMotion {
 public:
  /**
   * コンストラクタ
   */
  CrossToCross(COLOR _targetColor, int _targetBrightness, bool& _isLeftEdge);

  /**
   * @brief 交点間を移動する
   */
  void run() override;

  /**
   * @brief 交点間を移動する際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  COLOR targetColor;                        // 目標色
  double targetSpeed = 200;                 // 目標速度 [mm/s]
  int targetBrightness;                     // 目標輝度
  PidGain gain = PidGain(0.1, 0.08, 0.05);  // PIDゲイン
  bool& isLeftEdge;  // エッジの左右判定(true:左エッジ, false:右エッジ)};
};

#endif
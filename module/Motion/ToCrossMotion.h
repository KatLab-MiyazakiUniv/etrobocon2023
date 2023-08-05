/**
 * @file ToCrossMotion.h
 * @brief 交点間移動のクラス
 * @author YKhm20020
 */

#ifndef TO_CROSS_MOTION_H
#define TO_CROSS_MOTION_H

#include "ColorLineTracing.h"
#include "ColorJudge.h"
#include "Measurer.h"
#include "BlockMotion.h"

class ToCrossMotion : public BlockMotion {
 public:
  ToCrossMotion(COLOR _targetColor, double _targetSpeed, int _targetBrightness,
                const PidGain& _gain, bool& _isLeftEdge);

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
  COLOR targetColor;     // 目標色
  double targetSpeed;    // 目標速度 [mm/s]
  int targetBrightness;  // 目標輝度
  PidGain gain;          // PIDゲイン
  bool& isLeftEdge;      // エッジの左右判定(true:左エッジ, false:右エッジ)};
};

#endif
/**
 * @file CrossToMid.h
 * @brief 交点サークルから直線の中点へ移動するクラス
 * @author YKhm20020
 */

#ifndef CROSS_TO_MID_H
#define CROSS_TO_MID_H

#include "InCrossStraight.h"
#include "InCrossLeft.h"
#include "InCrossRight.h"
#include "DistanceLineTracing.h"
#include "BlockAreaMotion.h"

class CrossToMid : public BlockAreaMotion {
 public:
  /**
   * コンストラクタ
   */
  CrossToMid(double _targetLineDistance, double _targetCircleDistance, double _dlTargetSpeed,
             double _dsTargetSpeed, int targetAngle, int _prPwm, int _targetBrightness,
             const PidGain& _gain, bool _isClockwise, bool& _isLeftEdge, bool _nextEdge);

  /**
   * @brief 交点サークルから中点へ移動
   */
  void run() override;

  /**
   * @brief 交点から中点へ移動する際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  double targetLineDistance;  // サークル間直線の端から中点までの距離 (一昨年は125.0)
  double targetCircleDistance;  // 移動するサークル内距離（直進は100.0, 左右折は25.0）
  double dlTargetSpeed;         // 距離指定ライントレースの目標速度 [mm/s]
  double dsTargetSpeed;         // 距離指定直進の目標速度 [mm/s]
  int targetAngle;              // 目標回頭角度
  int prPwm;                    // 角度指定回頭の目標PWM
  int targetBrightness;         // 目標輝度
  PidGain gain;                 // PIDゲイン
  bool isClockwise;             // 回頭方向 (true:時計回り, false:反時計回り)
  bool& isLeftEdge;             // エッジの左右判定(true:左エッジ, false:右エッジ)
  bool nextEdge;                // 方向転換後のエッジ
};

#endif

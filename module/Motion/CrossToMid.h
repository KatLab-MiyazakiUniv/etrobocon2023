/**
 * @file CrossToMid.h
 * @brief サークルの交点から交点へ移動するクラス
 * @author YKhm20020
 */

#ifndef CROSS_TO_MID_H
#define CROSS_TO_MID_H

#include "DirectionChanger.h"
#include "DistanceLineTracing.h"

class CrossToMid : public BlockMotion {
 public:
  CrossToMid(COLOR _targetColor, double _targetSpeed, int _targetBrightness, int targetAngle,
             const PidGain& _gain, bool _isClockwise, bool& _isLeftEdge, bool _nextEdge);

  /**
   * @brief サークルの交点から中点へ移動
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
  COLOR targetColor;              // 目標色
  double targetDistance = 125.0;  // サークル間直線の端から中点までの距離
  double targetSpeed;             // 目標速度 [mm/s]
  int targetBrightness;           // 目標輝度
  int targetAngle;                // 目標回頭角度
  PidGain gain;                   // PIDゲイン
  bool isClockwise;               // 回頭方向 (true:時計回り, false:反時計回り)
  bool& isLeftEdge;               // エッジの左右判定(true:左エッジ, false:右エッジ)
  bool nextEdge;                  // 方向転換後のエッジ
};

#endif

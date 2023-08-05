/**
 * @file MidToMid.h
 * @brief サークル間直線の中点から別の直線の中点へ移動するクラス
 * @author YKhm20020
 */

#ifndef MID_TO_MID_H
#define MID_TO_MID_H

#include "DirectionChanger.h"
#include "DistanceLineTracing.h"
#include "ColorLineTracing.h"

class MidToMid : public BlockMotion {
 public:
  MidToMid(COLOR _targetColor, double _targetSpeed, int _targetBrightness, const PidGain& _gain,
           bool& _isLeftEdge, int _rotateAngle, int _changeAngle);

  /**
   * @brief サークル間直線の中点から別の直線の中点へ移動
   */
  void run();

  /**
   * @brief 中点から中点へ移動する際の事前条件判定をする
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
  bool& isLeftEdge;      // エッジの左右判定(true:左エッジ, false:右エッジ)
  int rotateAngle;       // 回転角度（deg） -180 < angle <= 180 45度刻み
  int changeAngle;       // 設置動作の直前方向から見て回頭する角度
};

#endif

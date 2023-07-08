/**
 * @file   DistanceLineTracing.h
 * @brief  指定距離ライントレース動作
 * @author YKhm20020
 */

#ifndef DISTANCE_LINE_TRACING_H
#define DISTANCE_LINE_TRACING_H

#include "LineTracing.h"
#include "Timer.h"

class DistanceLineTracing : public LineTracing {
 public:
  /**
   * コンストラクタ
   * @param _targetDistance 目標距離 0~
   * @param _targetSpeed 目標速度 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _gain PIDゲイン
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   */
  DistanceLineTracing(double _targetDistance, double _targetSpeed, int _targetBrightness,
                      const PidGain& _gain, bool& _isLeftEdge);

  /**
   * @brief 指定距離だけライントレースする
   */
  using LineTracing::run;

  /**
   * @brief 指定距離ライントレースする際の事前条件判定をする
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */
  bool isMetPrecondition(double targetSpeed) override;

  /**
   * @brief 指定距離ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド必須
   */
  bool isMetPostcondition() override;

  /**
   * @brief 実行のログを取る
   * @note オーバーライド必須
   */
  void logRunning() override;

 private:
  double targetDistance;  // 目標距離 0~
};

#endif
/**
 * @file   LineTracing.h
 * @brief  ライントレース動作
 * @author YKhm20020
 */

#ifndef LINE_TRACING_H
#define LINE_TRACING_H

#include "Motion.h"
#include "Mileage.h"
#include "Pid.h"
#include "SpeedCalculator.h"

class LineTracing : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _targetSpeed 目標速度 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _gain PIDゲイン
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   */
  LineTracing(double _targetSpeed, int _targetBrightness, const PidGain& _gain, bool _isLeftEdge);

  /**
   * @brief ライントレースする
   */
  void run();

  /**
   * @brief ライントレースする際の事前条件判定をする
   * @param basePwm 初期PWM値
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */
  bool isMetPrecondition(int basePwm, double targetSpeed);

  /**
   * @brief ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド必須
   */
  virtual bool isMetPostcondition() = 0;

  /**
   * @brief 実行のログを取る
   */
  virtual void logRunning();

 private:
  double targetSpeed;       // 目標速度 0~
  int targetBrightness;     // 目標輝度 0~
  int basePwm;              // 初期PWM値 -100~100
  PidGain gain;             // PIDゲイン
  bool isLeftEdge;          // エッジの左右判定(true:左エッジ, false:右エッジ)
  double initLeftMileage;   // クラス呼び出し時の左車輪の走行距離
  double initRightMileage;  // クラス呼び出し時の右車輪の走行距離
  class Timer timer;
};

#endif
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

class LineTracing : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _targetDistance 目標距離 0~
   * @param _targetBrightness 目標輝度 0~
   * @param _pwm PWM値 -100~100
   * @param _gain PIDゲイン
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   */
  LineTracing(double _targetDistance, int _targetBrightness, int _pwm, const PidGain& _gain,
              bool& _isLeftEdge);

  /**
   * @brief ライントレースする
   */
  void run();

  /**
   * @brief ライントレースする際の事前条件判定をする
   * @param pwm PWM値
   * @param targetDistance 目標距離
   * @note オーバーライド必須
   */
  virtual bool isMetPrecondition(int pwm, double targetDistance) = 0;

  /**
   * @brief ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @param initLeftMileage   ライントレースを始めた時点での左車輪の走行距離
   * @param initRightMileage  ライントレースを始めた時点での右車輪の走行距離
   * @note オーバーライド必須
   */
  virtual bool isMetPostcondition(double initLeftMileage, double initRightMileage) = 0;

  /**
   * @brief 実行のログを取る
   */
  void logRunning();

 private:
  double targetDistance;    // 目標距離 0~
  int targetBrightness;     // 目標輝度 0~
  int pwm;                  // PWM値 -100~100
  PidGain gain;             // PIDゲイン
  bool& isLeftEdge;         // エッジの左右判定(true:左エッジ, false:右エッジ)
  double initLeftMileage;   // クラス呼び出し時の左車輪の走行距離
  double initRightMileage;  // クラス呼び出し時の右車輪の走行距離
};

#endif
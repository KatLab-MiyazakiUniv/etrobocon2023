/**
 * @file   ColorLineTracing.h
 * @brief  指定色ライントレース動作
 * @author YKhm20020
 */

#ifndef COLOR_LINE_TRACING_H
#define COLOR_LINE_TRACING_H

#include "LineTracing.h"
#include "ColorJudge.h"

class ColorLineTracing : public LineTracing {
 public:
  /**
   * コンストラクタ
   * @param _targetColor 指定色
   * @param _targetSpeed 目標速度
   * @param _targetBrightness 目標輝度 0~
   * @param _gain PIDゲイン
   * @param _isLeftEdge エッジの左右判定(true:左エッジ, false:右エッジ)
   */
  ColorLineTracing(COLOR _targetColor, double _targetSpeed, int _targetBrightness,
                   const PidGain& _gain, bool& _isLeftEdge);

  /**
   * @brief 指定色までライントレースする
   */
  using LineTracing::run;

  /**
   * @brief 指定色ライントレースする際の事前条件判定をする
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */
  bool isMetPrecondition(double targetSpeed) override;

  /**
   * @brief 指定色ライントレースする際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @note オーバーライド必須
   */
  bool isMetPostcondition() override;

  /**
   * @brief 実行のログを取る
   * @note オーバーライド必須
   */
  void logRunning() override;

 private:
  static constexpr int JUDGE_COUNT = 3;
  int colorCount = 0;
  COLOR targetColor;  // 指定色
};

#endif
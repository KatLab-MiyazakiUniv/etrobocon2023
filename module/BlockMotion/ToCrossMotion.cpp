/**
 * @file ToCrossMotion.cpp
 * @brief 交点サークル間移動のクラス
 * @author YKhm20020
 */

#include "ToCrossMotion.h"

ToCrossMotion::ToCrossMotion(LineTracer& _lineTracer)
  : BlockMotion(1.02, 1.01), lineTracer(_lineTracer)
{
}

void ToCrossMotion::runToCross(void)
{
  int targetBrightness = 12;
  int pwm = 40;
  double targetDistance = 40;  // 要調整
  int targetSpeed = 40;        // 要調整
  PidGain gain(1.0, 0.0, 0.23);

  DistanceStraight ds(targetDistance, targetSpeed);

  // 白黒以外までライントレース
  lineTracer.runToColor(targetBrightness, pwm, gain);
}
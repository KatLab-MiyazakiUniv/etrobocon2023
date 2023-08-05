/**
 * @file ToCrossMotion.cpp
 * @brief 交点サークル間移動のクラス
 * @author YKhm20020
 */

#include "ToCrossMotion.h"

ToCrossMotion::ToCrossMotion(COLOR _targetColor, bool& _isLeftEdge)
  : BlockMotion(1.02, 1.01), targetColor(_targetColor), isLeftEdge(_isLeftEdge)
{
}

void ToCrossMotion::runToCross()
{
  int targetSpeed = 40;  // 要調整
  int targetBrightness = 12;
  PidGain gain(1.0, 0.0, 0.23);

  ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

  // 指定したサークルの色(白黒以外)までライントレース
  cl.run();
}
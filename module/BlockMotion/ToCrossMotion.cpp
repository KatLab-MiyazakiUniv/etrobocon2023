/**
 * @file ToCrossMotion.cpp
 * @brief 交点サークル間移動のクラス
 * @author YKhm20020
 */

#include "ToCrossMotion.h"

ToCrossMotion::ToCrossMotion(bool& _isLeftEdge) : BlockMotion(1.02, 1.01), isLeftEdge(_isLeftEdge)
{
}

void ToCrossMotion::runToCross()
{
  COLOR targetColor = COLOR::NONE;
  COLOR currentColor = COLOR::NONE;
  int targetSpeed = 40;  // 要調整
  int targetBrightness = 12;
  PidGain gain(1.0, 0.0, 0.23);

  // 白黒以外までライントレース
  currentColor = ColorJudge::getColor(Measurer::getRawColor());
  while(!(currentColor == COLOR::WHITE || currentColor == COLOR::BLACK)) {
    if(currentColor == COLOR::WHITE) {
      targetColor = COLOR::WHITE;
      ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);
      cl.run();
    } else if(currentColor == COLOR::BLACK) {
      targetColor = COLOR::BLACK;
      ColorLineTracing cl2(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);
      cl2.run();
    }
  }
}
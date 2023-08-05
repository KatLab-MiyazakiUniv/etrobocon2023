/**
 * @file CrossToMid.cpp
 * @brief サークルの交点から交点へ移動するクラス
 * @author YKhm20020
 */

#include "CrossToMid.h"

CrossToMid::CrossToMid(int _direction, bool& _isLeftEdge)
  : BlockMotion(1.02, 1.01), direction(_direction), isLeftEdge(_isLeftEdge)
{
}

void CrossToMid::run()
{
  double targetSpeed = 40.0;  // 要調整
  double targetDistance = 126.8; // サークル間直線端から中点までの距離
  int targetBrightness = 12;
  PidGain gain(1.0, 0.0, 0.23);
  
  DistanceLineTracing dl(targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

  if(direction == 0){
    InCrossStraight is(targetSpeed);
    is.runForward();

  }else if(direction == 3){
    InCrossRight ir();
    ir.runRight();

  }else if(direction == 9){
    InCrossLeft il();
    il.runLeft();
  }

  // 交点から中点までライントレース
  dl.run();
}
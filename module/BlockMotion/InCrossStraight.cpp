/**
 * @file InCrossMotion.cpp
 * @brief 交点内移動（直進）のクラス
 * @author YKhm20020
 */

#include "InCrossStraight.h"

InCrossStraight::InCrossStraight() : BlockMotion(1.0, 1.12) {}

void InCrossStraight::runForward(void)
{
  double targetDistance = 100.0;  // 交点サークルの外径
  int targetSpeed = 30;           // 要調整

  DistanceStraight ds(targetDistance, targetSpeed);

  // 交点サークルの外径分直進する
  ds.run();
}
/**
 * @file InCrossRight.cpp
 * @brief 交点内移動（右折）のクラス
 * @author YKhm20020
 */

#include "InCrossRight.h"

InCrossRight::InCrossRight() : BlockMotion(1.23, 1.09) {}

void InCrossRight::runRight(void)
{
  double targetDistance = 25.0;
  int targetSpeed = 30;  // 要調整
  int angle = 74;
  bool isClockwise = true;  // ピボットターンの方向を右回りに
  bool isLeftEdge = false;  // ピボットターン前のエッジを右に
  bool nextEdge = true;     // ピボットターン後のエッジを左に

  DistanceStraight ds(targetDistance, targetSpeed);
  AngleRotation rotation(angle, targetSpeed, isClockwise);
  EdgeChanging ec(isLeftEdge, nextEdge);

  // ピボットターン後の位置を調整するため、直進する
  ds.run();
  // 右に90度ピボットターンする
  rotation.run();
  // エッジを左にする
  ec.run();
}
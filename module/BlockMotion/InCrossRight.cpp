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
  double targetSpeed = 30;  // 要調整
  int angle = 74;
  bool isClockwise = true;  // ピボットターンの方向は右回り
  bool isLeftEdge = false;  // ピボットターン前のエッジは右
  bool nextEdge = true;     // ピボットターン後のエッジを左に変更

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
/**
 * @file InCrossLeft.cpp
 * @brief 交点内移動（左折）のクラス
 * @author YKhm20020
 */

#include "InCrossLeft.h"

InCrossLeft::InCrossLeft() : BlockMotion(1.23, 1.09) {}

void InCrossLeft::runLeft(void)
{
  double targetDistance = 25.0;
  int targetSpeed = 30;  // 要調整
  int angle = 74;
  bool isClockwise = false;  // ピボットターンの方向は左回り
  bool isLeftEdge = true;    // ピボットターン前のエッジは左
  bool nextEdge = false;     // ピボットターン後のエッジを右に変更

  DistanceStraight ds(targetDistance, targetSpeed);
  AngleRotation rotation(angle, targetSpeed, isClockwise);
  EdgeChanging ec(isLeftEdge, nextEdge);

  // ピボットターン後の位置を調整するため、直進する
  ds.run();
  // 左に90度ピボットターンする
  rotation.run();
  // エッジを右にする
  ec.run();
}
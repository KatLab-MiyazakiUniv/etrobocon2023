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
  int runPwm = 30;
  int angle = 74;
  int turnPwm = 90;
  bool isClockwise = false;  // ピボットターンの方向を左回りに
  bool isLeftEdge = true;    // ピボットターン前のエッジを左に
  bool nextEdge = false;     // ピボットターン後のエッジを右に

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
/**
 * @file DirectionChanger.cpp
 * @brief 方向転換クラス
 * @author YKhm20020
 */

#include "DirectionChanger.h"

DirectionChanger::DirectionChanger() : BingoMotion(0.88, 0.92) {}

void DirectionChanger::changeDirection(int rotateAngle, int changeAngle)
{
  double targetDistance = 10.0;
  int targetSpeed = 20;     // 要調整
  bool isClockwise = true;  // 転換する方向を右回りに
  bool isLeftEdge = false;  // 方向転換前のエッジ
  bool nextEdge = true;     // 方向転換後のエッジ

  DistanceStraight ds(targetDistance, targetSpeed);
  AngleRotation rotation(angle, targetSpeed, isClockwise);
  EdgeChanging ec(isLeftEdge, nextEdge);

  //回転方向を判定
  if(rotateAngle >= 0) {
    isClockwise = true;
    rotation.run(rotateAngle, targetSpeed, isClockwise);
  } else {
    isClockwise = false;
    rotation.run(rotateAngle, targetSpeed, isClockwise);
  }

  //エッジ切り替え
  switch(changeAngle) {
    case 90:  // 右方向に方向転換する場合
      nextEdge = true;
      ec.run(isLeftEdge, nextEdge);
      break;
    case -90:  // 左方向に方向転換する場合
      nextEdge = false;
      ec.run(isLeftEdge, nextEdge);
      break;
    case 180:  // 後ろ方向に方向転換する場合、要修正
    case -180:
      ec.run(isLeftEdge, nextEdge);
      break;
  }

  //色サークルに乗るまで直進
  ds.run(targetDistance, targetSpeed);

  //白黒を判定するまで進む
  // 実装方法を探す
  ds.runStraightToBlackWhite(runPwm);
}
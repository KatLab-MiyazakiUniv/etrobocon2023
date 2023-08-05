/**
 * @file DirectionChanger.cpp
 * @brief 方向転換クラス
 * @author YKhm20020
 */

#include "DirectionChanger.h"

DirectionChanger::DirectionChanger(COLOR _targetColor)
  : BlockMotion(0.88, 0.92), targetColor(_targetColor)
{
}

void DirectionChanger::changeDirection(int rotateAngle, int changeAngle)
{
  double targetDistance = 10.0;
  int targetSpeed = 20;     // 要調整
  bool isLeftEdge = false;  // 方向転換前のエッジ
  bool nextEdge = true;     // 方向転換後のエッジ

  DistanceStraight ds(targetDistance, targetSpeed);
  ColorStraight cs(targetColor, targetSpeed);

  // 回転方向を判定
  if(rotateAngle >= 0) {
    bool isClockwise = true;
    AngleRotation rotation(changeAngle, targetSpeed, isClockwise);
    rotation.run();
  } else {
    bool isClockwise = false;
    AngleRotation rotation2(changeAngle, targetSpeed, isClockwise);
    rotation2.run();
  }

  //エッジ切り替え
  switch(changeAngle) {
    case 90:  // 右方向に方向転換する場合
    {
      nextEdge = true;
      EdgeChanging ec(isLeftEdge, nextEdge);
      ec.run();
      break;
    }
    case -90:  // 左方向に方向転換する場合
    {
      nextEdge = false;
      EdgeChanging ec2(isLeftEdge, nextEdge);
      ec2.run();
      break;
    }
    case 180:  // 後ろ方向に方向転換する場合, 要修正
    case -180: {
      EdgeChanging ec3(isLeftEdge, nextEdge);
      ec3.run();
      break;
    }
  }

  // 色サークルに乗るまで直進
  ds.run();

  // 指定色（白または黒）を判定するまで直進
  cs.run();
}
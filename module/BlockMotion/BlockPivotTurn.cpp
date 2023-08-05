/**
 * @file BlockPivotTurn.cpp
 * @brief ピボットターン設置クラス
 * @author YKhm20020
 */

#include "BlockPivotTurn.h"

BlockPivotTurn::BlockPivotTurn(bool _isClockwise) : BlockMotion(2.0, 1.41){}

void BlockPivotTurn::setBlockPivotTurn()
{
  double runDistance = 40.0;      // 最初に直進する距離
  int runFirstSpeed = 10;         // 最初に直進する際の速度, 要調整
  int rotateFirstAngle = 10;      // 最初に回頭する角度
  int rotateFirstSpeed = 10;      // 最初に回頭する際の速度, 要調整
  int pivotAngle = 50;            // ピボットターンの角度
  int pivotSpeed = 40;            // ピボットターンの速度, 要調整
  int rotateSecondAngle = 88;     // 二回目の回頭の角度
  int rotateSecondSpeed = 10;     // 二回目の回頭の速度, 要調整
  double forwardDistance = 90.0;  // 前進する距離
  double backDistance = 115.0;    // 後退する距離
  int runSpeed = 30;              // 前進、後退する際の速度, 要調整

  InCrossLeft inCrossLeft();
  InCrossRight inCrossRight();

  DistanceStraight ds(runDistance, runFirstSpeed);
  DistanceStraight ds2(forwardDistance, runSpeed);
  DistanceStraight ds3(backDistance, -(runSpeed - 10));

  DistanceStraight ds4(runDistance, runSpeed);
  DistanceStraight ds5(forwardDistance, runSpeed);
  DistanceStraight ds6(backDistance, -(runSpeed - 10));

  isClockwise = true;
  AngleRotation rotation(rotateFirstAngle, rotateFirstSpeed, isClockwise);
  AngleRotation rotation2(pivotAngle - 2, pivotSpeed, isClockwise);
  AngleRotation rotation3(rotateSecondAngle - 20, rotateSecondSpeed - 2, isClockwise);

  isClockwise = false;
  AngleRotation rotation4(rotateFirstAngle, rotateFirstSpeed, isClockwise);
  AngleRotation rotation5(pivotAngle, pivotSpeed, isClockwise);
  AngleRotation rotation6(rotateSecondAngle - 10, rotateSecondSpeed, isClockwise);

  // ピボットターンする
  if(isClockwise) {
    ds.run();
    rotation.run();
    rotation2.run();
    timer.sleep();
    rotation3.run();
    ds2.run();
    ds3.run();
  } else {
    ds4.run();
    rotation4.run();
    rotation5.run();
    rotation6.run();
    ds5.run();
    ds6.run();
  }
  timer.sleep(500);
}
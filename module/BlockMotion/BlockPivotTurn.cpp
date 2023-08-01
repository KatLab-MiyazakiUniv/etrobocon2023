/**
 * @file BlockPivotTurn.cpp
 * @brief ピボットターン設置クラス
 * @author YKhm20020
 */

#include "BlockPivotTurn.h"
#include "Clock.h"

BlockPivotTurn::BlockPivotTurn() : BlockMotion(2.0, 1.41) {}

void BlockPivotTurn::setBlockPivotTurn(bool isClockwise)
{
  double runDistance = 40.0;      // 最初に直進する距離
  int runFirstSpeed = 10;         // 最初に直進する際の速度
  int rotateFirstAngle = 10;      // 最初に回頭する角度
  int rotateFirstPwm = 10;        // 最初に回頭するPwm
  int rotateFirstPwm = 10;        // 最初に回頭する際の速度
  int pivotAngle = 50;            // ピボットターンの角度
  int pivotPwm = 40;              // ピボットターンのPwm値
  int rotateSecondAngle = 88;     // 二回目の回頭の角度
  int rotateSecondPwm = 10;       // 二回目の回頭のPwm値
  double forwardDistance = 90.0;  // 前進する距離
  double backDistance = 115.0;    // 後退する距離
  int runPwm = 30;                // 前進、後退する際のPwm値

  InCrossLeft inCrossLeft();
  InCrossRight inCrossRight();

  DistanceStraight ds(targetDistance, targetSpeed);
  AngleRotation rotation(angle, targetSpeed, isClockwise);

  // ピボットターンする
  if(isClockwise) {
    ds.run(runDistance, runFirstSpeed);
    rotation.run(rotateFirstAngle, rotateFirstSpeed, isClockwise);
    rotation.turnForwardRightPivot(pivotAngle - 2, pivotPwm);
    timer.sleep();
    rotation.rotateRight(rotateSecondAngle - 20, rotateSecondPwm - 2);
    ds.run(forwardDistance, runPwm);
    ds.run(backDistance, -(runPwm - 10));
  } else {
    ds.run(runDistance, runPwm);
    rotation.rotateRight(rotateFirstAngle, rotateFirstPwm);
    rotation.turnForwardLeftPivot(pivotAngle, pivotAngle);
    rotation.rotateLeft(rotateSecondAngle - 10, rotateSecondPwm);
    ds.run(forwardDistance, runPwm);
    ds.run(backDistance, -(runPwm - 10));
  }
  timer.sleep(500);
}
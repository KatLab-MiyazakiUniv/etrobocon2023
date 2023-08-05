/**
 * @file InCrossStraight.cpp
 * @brief 交点内移動（直進）のクラス
 * @author YKhm20020
 */

#include "InCrossStraight.h"

using namespace std;

InCrossStraight::InCrossStraight(double _targetSpeed)
  : BlockMotion(1.0, 1.12), targetSpeed(_targetSpeed)
{
}

void InCrossStraight::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  DistanceStraight ds(targetDistance, targetSpeed);

  // 交点サークルの外径分直進する
  ds.run();
}

bool InCrossStraight::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to InCrossStraight is 0");
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void InCrossStraight::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run InCrossStraight (targetDistance: %.2f, targetSpeed: %.2f)",
           targetDistance, targetSpeed);
  logger.log(buf);
}
/**
 * @file   DistanceStraight.cpp
 * @brief  目標距離まで直進するクラス
 * @author Negimarge
 */

#include "DistanceStraight.h"
using namespace std;

DistanceStraight::DistanceStraight(double _targetDistance, double _targetSpeed)
  : Straight(_targetSpeed), targetDistance(_targetDistance)
{
}

bool DistanceStraight::isRunPostconditionJudgement()
{
  // 現在の距離を取得する
  currentRightMotorCount = Measurer::getRightCount();
  currentLeftMotorCount = Measurer::getLeftCount();
  currentDistance = Mileage::calculateMileage(currentLeftMotorCount, currentRightMotorCount);

  // 現在の距離が目標距離に到達したらループを終了する
  if(abs(currentDistance - initialDistance) >= targetDistance) {
    return true;
  }

  return false;
}

bool DistanceStraight::isRunPreconditionJudgement()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];
  // 目標速度値が0の場合は終了する
  if(targetSpeed == 0) {
    logger.logWarning("The targetSpeed value passed to DistanceStraight is 0");
    return false;
  }
  // 目標距離の値が0以下の場合はwarningを出して終了する
  if(targetDistance <= 0) {
    snprintf(buf, BUF_SIZE, "The targetDistance value passed to DistanceStraight is %.2f",
             targetDistance);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void DistanceStraight::logRunning()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run DistanceStraight (Distance: %f, targetSpeed: %f)", targetDistance,
           targetSpeed);
  logger.log(buf);
}
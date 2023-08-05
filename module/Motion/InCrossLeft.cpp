/**
 * @file InCrossLeft.cpp
 * @brief 交点内移動（左折）のクラス
 * @author YKhm20020
 */

#include "InCrossLeft.h"

using namespace std;

InCrossLeft::InCrossLeft(double _targetDistance, double _targetSpeed, int _targetAngle) : BlockMotion(1.23, 1.09), targetDistance(_targetDistance), targetSpeed(_targetSpeed), targetAngle(_targetAngle) {}

void InCrossLeft::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  DistanceStraight ds(targetDistance, targetSpeed);
  AngleRotation rotation(targetAngle, targetSpeed, isClockwise);
  EdgeChanging ec(isLeftEdge, nextEdge);

  // ピボットターン後の位置を調整するため、直進する
  ds.run();
  // 左に90度ピボットターンする
  rotation.run();
  // エッジを右にする
  ec.run();
}

bool InCrossLeft::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to InCrossLeft is 0");
    logger.logWarning(buf);
    return false;
  }

  // targetAngleが0以下の場合はwarningを出して終了する
  if(targetAngle <= 0 || targetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The targetAngle value passed to InCrossLeft is %d", targetAngle);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void InCrossLeft::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* isLeftEdgeStr = isLeftEdge ? "true" : "false";
  const char* nextEdgeStr = nextEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run InCrossLeft (targetDistance: %.2f, targetSpeed: %.2f, targetAngle: %d, isLeftEdge: %s, nextEdge: %s)",
           targetDistance, targetSpeed, targetAngle, isLeftEdgeStr, nextEdgeStr);
  logger.log(buf);
}
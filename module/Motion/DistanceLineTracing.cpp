/**
 * @file   DistanceLineTracing.cpp
 * @brief  指定距離ライントレース動作
 * @author YKhm20020 bizyutyu
 */

#include "DistanceLineTracing.h"
using namespace std;

DistanceLineTracing::DistanceLineTracing(double _targetDistance, double _targetSpeed,
                                         int _targetBrightness, const PidGain& _gain,
                                         bool& _isLeftEdge)
  : LineTracing(_targetSpeed, _targetBrightness, _gain, _isLeftEdge),
    targetDistance(_targetDistance){};

bool DistanceLineTracing::isMetPrecondition(double targetSpeed)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to DistanceLineTracing is 0");
    logger.logWarning(buf);
    return false;
  }

  // targetDistance値が0以下の場合はwarningを出して終了する
  if(targetDistance <= 0.0) {
    snprintf(buf, BUF_SIZE, "The targetDistance value passed to DistanceLineTracing is %.2f",
             targetDistance);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

bool DistanceLineTracing::isMetPostcondition()
{
  // 初期値を代入
  currentDistance = Mileage::calculateMileage(Measurer::getRightCount(), Measurer::getLeftCount());

  // 走行距離が目標距離に到達
  if(abs(currentDistance - initialDistance) >= targetDistance) return false;

  return true;
}

void DistanceLineTracing::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run DistanceLineTracing (targetDistance: %.2f, targetSpeed: %.2f, targetBrightness: "
           "%d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s)",
           targetDistance, targetSpeed, targetBrightness, gain.kp, gain.ki, gain.kd, str);
  logger.log(buf);
}
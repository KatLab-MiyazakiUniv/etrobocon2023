/**
 * @file CrossToMid.cpp
 * @brief サークルの交点から交点へ移動するクラス
 * @author YKhm20020
 */

#include "CrossToMid.h"

using namespace std;

CrossToMid::CrossToMid(COLOR _targetColor, double _targetSpeed, int _targetBrightness,
                       int _targetAngle, const PidGain& _gain, bool _isClockwise, bool& _isLeftEdge,
                       bool _nextEdge)
  : BlockMotion(1.02, 1.01),  // 動作時間, 失敗リスク TODO: 測定し直す
    targetColor(_targetColor),
    targetSpeed(_targetSpeed),
    targetBrightness(_targetBrightness),
    targetAngle(_targetAngle),
    gain(_gain),
    isClockwise(_isClockwise),
    isLeftEdge(_isLeftEdge),
    nextEdge(_nextEdge){};

void CrossToMid::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  DirectionChanger dc(targetColor, targetDistance, targetSpeed, targetAngle, isClockwise,
                      isLeftEdge, nextEdge);
  DistanceLineTracing dl(targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

  // サークル内移動
  dc.run();

  // 交点から中点までライントレース
  dl.run();
}

bool CrossToMid::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // 目標の色がNoneのときwarningを出して終了する
  if(targetColor == COLOR::NONE) {
    logger.logWarning("The targetColor passed to CrossToMid is NONE");
    return false;
  }

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to CrossToMid is 0");
    logger.logWarning(buf);
    return false;
  }

  // targetAngleが0以下の場合はwarningを出して終了する
  if(targetAngle <= 0 || targetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The targetAngle value passed to CrossToMid is %d", targetAngle);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void CrossToMid::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* isClockwiseStr = isClockwise ? "true" : "false";
  const char* isLeftEdgeStr = isLeftEdge ? "true" : "false";
  const char* nextEdgeStr = nextEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run CrossToMid (targetColor: %s, targetDistance: %.2f, targetSpeed: %.2f, "
           "targetBrightness: %d, targetAngle: %d, gain: "
           "(%.2f,%.2f,%.2f), isClockwise: %s, isLeftEdge: %s, nextEdge: %s)",
           ColorJudge::colorToString(targetColor), targetDistance, targetSpeed, targetBrightness,
           targetAngle, gain.kp, gain.ki, gain.kd, isClockwiseStr, isLeftEdgeStr, nextEdgeStr);
  logger.log(buf);
}
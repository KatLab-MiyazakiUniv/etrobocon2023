/**
 * @file CrossToCross.cpp
 * @brief 交点から交点までライントレースするクラス
 * @author YKhm20020
 */

#include "CrossToCross.h"

using namespace std;

CrossToCross::CrossToCross(COLOR _targetColor, bool& _isLeftEdge)
  : targetColor(_targetColor), isLeftEdge(_isLeftEdge){};

void CrossToCross::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

  // 指定したサークルの色(白黒以外)までライントレース
  cl.run();
}

bool CrossToCross::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // 目標の色がNoneのときwarningを出して終了する
  if(targetColor == COLOR::NONE) {
    logger.logWarning("The targetColor passed to CrossToCross is NONE");
    return false;
  }

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to CrossToCross is 0");
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void CrossToCross::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run CrossToCross (targetColor: %s, targetSpeed: %.2f, targetBrightness: %d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s)",
           ColorJudge::colorToString(targetColor), targetSpeed, targetBrightness, gain.kp, gain.ki,
           gain.kd, str);
  logger.log(buf);
}
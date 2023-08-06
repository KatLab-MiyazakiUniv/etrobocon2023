/**
 * @file ToCross.cpp
 * @brief 交点サークル間移動のクラス
 * @author YKhm20020
 */

#include "ToCross.h"

using namespace std;

ToCross::ToCross(COLOR _targetColor, double _targetSpeed, int _targetBrightness,
                 const PidGain& _gain, bool& _isLeftEdge)
  : BlockMotion(1.02, 1.01),  // 動作時間, 失敗リスク TODO: 測定し直す
    targetColor(_targetColor),
    targetSpeed(_targetSpeed),
    targetBrightness(_targetBrightness),
    gain(_gain),
    isLeftEdge(_isLeftEdge)
{
}

void ToCross::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);

  // 指定したサークルの色(白黒以外)までライントレース
  cl.run();
}

bool ToCross::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // 目標の色がNoneのときwarningを出して終了する
  if(targetColor == COLOR::NONE) {
    logger.logWarning("The targetColor passed to ToCross is NONE");
    return false;
  }

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to ToCross is 0");
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void ToCross::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run ToCross (targetColor: %s, targetSpeed: %.2f, targetBrightness: %d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s)",
           ColorJudge::colorToString(targetColor), targetSpeed, targetBrightness, gain.kp, gain.ki,
           gain.kd, str);
  logger.log(buf);
}
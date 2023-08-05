/**
 * @file MidToMid.cpp
 * @brief サークル間直線の中点から別の直線の中点へ移動するクラス
 * @author YKhm20020
 */

#include "MidToMid.h"

using namespace std;

MidToMid::MidToMid(COLOR _targetColor, double _targetSpeed, int _targetBrightness,
                   const PidGain& _gain, bool& _isLeftEdge, int _rotateAngle, int _changeAngle)
  : BlockMotion(1.02, 1.01),
    targetColor(_targetColor),
    targetSpeed(_targetSpeed),
    targetBrightness(_targetBrightness),
    gain(_gain),
    isLeftEdge(_isLeftEdge),
    rotateAngle(_rotateAngle),
    changeAngle(_changeAngle)
{
}

void MidToMid::run()
{
  double targetDistance = 125.0;  // サークル間直線の端から中点までの距離

  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  ColorLineTracing cl(targetColor, targetSpeed, targetBrightness, gain, isLeftEdge);
  DirectionChanger dc(targetColor, targetDistance, targetSpeed, isLeftEdge, rotateAngle,
                      changeAngle);
  DistanceLineTracing dl(targetDistance, targetSpeed, targetBrightness, gain, isLeftEdge);

  // 中点からサークルの交点までライントレース
  cl.run();

  // サークル内移動
  dc.run();

  // 交点から中点までライントレース
  dl.run();
}

bool MidToMid::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to MidToMid is 0");
    logger.logWarning(buf);
    return false;
  }

  // 目標の色がNoneのときwarningを出して終了する
  if(targetColor == COLOR::NONE) {
    logger.logWarning("The targetColor passed to MidToMid is NONE");
    return false;
  }

  return true;
}

void MidToMid::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run MidToMid (targetColor: %s, targetSpeed: %.2f, targetBrightness: %d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s, rotateAngle: %d, changeAngle: %d)",
           ColorJudge::colorToString(targetColor), targetSpeed, targetBrightness, gain.kp, gain.ki,
           gain.kd, str, rotateAngle, changeAngle);
  logger.log(buf);
}
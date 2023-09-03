/**
 * @file CrossToMid.cpp
 * @brief 交点サークルから直線の中点へ移動するクラス
 * @author YKhm20020
 */

#include "CrossToMid.h"

using namespace std;

CrossToMid::CrossToMid(double _targetLineDistance, double _targetCircleDistance,
                       double _dlTargetSpeed, double _dsTargetSpeed, int _targetAngle,
                       double _arTargetSpeed, int _targetBrightness, const PidGain& _gain,
                       bool _isClockwise, bool& _isLeftEdge, bool _nextEdge)
  : BlockAreaMotion(1.02, 1.01),  // 動作時間, 失敗リスク TODO: 測定し直す
    targetLineDistance(_targetLineDistance),
    targetCircleDistance(_targetCircleDistance),
    dlTargetSpeed(_dlTargetSpeed),
    dsTargetSpeed(_dsTargetSpeed),
    targetAngle(_targetAngle),
    arTargetSpeed(_arTargetSpeed),
    targetBrightness(_targetBrightness),
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

  DistanceLineTracing dl(targetLineDistance, dlTargetSpeed, targetBrightness, gain, isLeftEdge);

  // サークル内移動
  if(targetAngle >= 0 && targetAngle <= 5) {  // 回頭角度が0度から5度の間であれば直進
    InCrossStraight is(targetCircleDistance, dsTargetSpeed);
    is.run();

  } else if(isClockwise == false) {  // 左に回頭するなら左折
    InCrossLeft il(targetCircleDistance, dsTargetSpeed, arTargetSpeed, targetAngle);
    il.run();

  } else {  // 右に回頭するなら右折
    InCrossRight ir(targetCircleDistance, dsTargetSpeed, arTargetSpeed, targetAngle);
    ir.run();
  }

  // 交点から中点までライントレース
  dl.run();
}

bool CrossToMid::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetLineDistance値が0以下の場合はwarningを出して終了する
  if(targetLineDistance <= 0.0) {
    snprintf(buf, BUF_SIZE, "The targetLineDistance value passed to CrossToMid is %.2f",
             targetLineDistance);
    logger.logWarning(buf);
    return false;
  }

  // targetCircleDistance値が0以下の場合はwarningを出して終了する
  if(targetCircleDistance <= 0.0) {
    snprintf(buf, BUF_SIZE, "The targetCircleDistance value passed to CrossToMid is %.2f",
             targetCircleDistance);
    logger.logWarning(buf);
    return false;
  }

  // dlTargetSpeed値が0の場合はwarningを出して終了する
  if(dlTargetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The dlTargetSpeed value passed to CrossToMid is 0");
    logger.logWarning(buf);
    return false;
  }

  // dsTargetSpeed値が0の場合はwarningを出して終了する
  if(dsTargetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The dsTargetSpeed value passed to CrossToMid is 0");
    logger.logWarning(buf);
    return false;
  }

  // arTargetSpeed値が0の場合はwarningを出して終了する
  if(arTargetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The arTargetSpeed value passed to CrossToMid is 0");
    logger.logWarning(buf);
    return false;
  }

  // targetAngleが0未満の場合はwarningを出して終了する
  if(targetAngle < 0 || targetAngle > 360) {
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

  snprintf(
      buf, BUF_SIZE,
      "Run CrossToMid (targetLineDistance: %.2f, targetCircleDistance: %.2f, dlTargetSpeed: %.2f, "
      "dsTargetSpeed: %.2f, targetAngle: %d, arTargetSpeed: %.2f"
      "targetBrightness: %d, gain: "
      "(%.2f,%.2f,%.2f), isClockwise: %s, isLeftEdge: %s, nextEdge: %s)",
      targetLineDistance, targetCircleDistance, dlTargetSpeed, dsTargetSpeed, targetAngle,
      arTargetSpeed, targetBrightness, gain.kp, gain.ki, gain.kd, isClockwiseStr, isLeftEdgeStr,
      nextEdgeStr);
  logger.log(buf);
}
/**
 * @file InCrossLeft.cpp
 * @brief 交点内移動（左折）のクラス
 * @author YKhm20020
 */

#include "InCrossLeft.h"

using namespace std;

InCrossLeft::InCrossLeft(double _targetDistance, double _dsTargetSpeed, int _targetAngle,
                         int _prPwm)
  : BlockAreaMotion(1.23, 1.09),  // 動作時間, 失敗リスク TODO: 測定し直す
    targetDistance(_targetDistance),
    dsTargetSpeed(_dsTargetSpeed),
    targetAngle(_targetAngle),
    prPwm(_prPwm){};

void InCrossLeft::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  DistanceStraight ds(targetDistance, dsTargetSpeed);

  PwmRotation rotation(targetAngle, prPwm, isClockwise);

  // 回頭後の位置を調整するため、直進する
  ds.run();
  // 左に90度回頭する
  rotation.run();
  // 円外へ出る
  ds.run();
}

bool InCrossLeft::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetDistance値が0以下の場合はwarningを出して終了する
  if(targetDistance <= 0.0) {
    snprintf(buf, BUF_SIZE, "The targetDistance value passed to InCrossLeft is %.2f",
             targetDistance);
    logger.logWarning(buf);
    return false;
  }

  // dsTargetSpeed値が0の場合はwarningを出して終了する
  if(dsTargetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The dsTargetSpeed value passed to InCrossLeft is 0");
    logger.logWarning(buf);
    return false;
  }

  // prPwm値が0の場合はwarningを出して終了する
  if(prPwm == 0) {
    snprintf(buf, BUF_SIZE, "The prPwm value passed to InCrossLeft is 0");
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

  snprintf(buf, BUF_SIZE,
           "Run InCrossLeft (targetDistance: %.2f, dsTargetSpeed: %.2f, targetAngle: %d, "
           "prPwm: %d)",
           targetDistance, dsTargetSpeed, targetAngle, prPwm);
  logger.log(buf);
}
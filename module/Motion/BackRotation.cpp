/**
 * @file BackRotation.cpp
 * @brief 交点振り向きのクラス
 * @author miyashita64
 */

#include "BackRotation.h"

using namespace std;

BackRotation::BackRotation(){};

void BackRotation::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  PwmRotation rotation(targetAngle, prPwm, isClockwise);

  // 右に180度回頭する
  rotation.run();
}

bool BackRotation::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // prPwm値が0の場合はwarningを出して終了する
  if(prPwm == 0) {
    snprintf(buf, BUF_SIZE, "The prPwm value passed to BackRotation is 0");
    logger.logWarning(buf);
    return false;
  }

  // targetAngleが0以下の場合はwarningを出して終了する
  if(targetAngle <= 0 || targetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The targetAngle value passed to BackRotation is %d", targetAngle);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void BackRotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE,
           "Run BackRotation (targetAngle: %d, prPwm: %d)",
           targetAngle, prPwm);
  logger.log(buf);
}
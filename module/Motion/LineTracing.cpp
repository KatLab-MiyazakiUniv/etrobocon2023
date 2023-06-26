/**
 * @file   LineTracing.cpp
 * @brief  ライントレース動作の中間クラス
 * @author YKhm20020
 */

#include "LineTracing.h"
using namespace std;

LineTracing::LineTracing(double _targetDistance, int _targetBrightness, int _pwm,
                         const PidGain& _gain, bool& _isLeftEdge)
  : targetDistance(_targetDistance),
    targetBrightness(_targetBrightness),
    pwm(_pwm),
    gain(_gain),
    isLeftEdge(_isLeftEdge)
{
}

void LineTracing::run()
{
  int currentPid = 0;
  int sign = 0;
  Pid pid(gain.kp, gain.ki, gain.kd, targetBrightness);

  // 事前条件を判定する
  if(!isMetPrecondition(pwm, targetDistance)) {
    return;
  }

  // 左右で符号を変える
  sign = isLeftEdge ? -1 : 1;

  // 呼び出し時の走行距離
  initLeftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  initRightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  // 継続条件を満たしている間ループ
  while(isMetPostcondition(initLeftMileage, initRightMileage)) {
    // PID計算
    currentPid = pid.calculatePid(Measurer::getBrightness()) * sign;

    // モータのPWM値をセット（0を超えないようにセット）
    int rightPwm = pwm > 0 ? max(pwm - (int)currentPid, 0) : min(pwm + (int)currentPid, 0);
    int leftPwm = pwm > 0 ? max(pwm + (int)currentPid, 0) : min(pwm - (int)currentPid, 0);
    Controller::setRightMotorPwm(rightPwm);
    Controller::setLeftMotorPwm(leftPwm);
  }

  // モータの停止
  Controller::stopMotor();
}

bool LineTracing::isMetPrecondition(int pwm, double targetDistance)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // pwm値が0の場合はwarningを出して終了する
  if(pwm == 0) {
    logger.logWarning("The pwm value passed to LineTracing is 0");
    return false;
  }

  // 目標距離が0以下の場合はwarningを出して終了する
  if(targetDistance <= 0) {
    snprintf(buf, BUF_SIZE, "The targetDistance value passed to LineTracing is %.2f",
             targetDistance);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

bool LineTracing::isMetPostcondition(double initLeftMileage, double initRightMileage)
{
  return false;
}

void LineTracing::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run LineTracing (targetDistance: %.2f, targetBrightness: %d, pwm: %d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s)",
           targetDistance, targetBrightness, pwm, gain.kp, gain.ki, gain.kd, str);
  logger.log(buf);
}
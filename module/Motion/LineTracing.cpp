/**
 * @file   LineTracing.cpp
 * @brief  ライントレース動作の中間クラス
 * @author YKhm20020
 */

#include "LineTracing.h"
using namespace std;

LineTracing::LineTracing(double _targetSpeed, int _targetBrightness, int _pwm,
                         const PidGain& _gain, bool& _isLeftEdge)
  : targetSpeed(_targetSpeed),
    targetBrightness(_targetBrightness),
    pwm(_pwm),
    gain(_gain),
    isLeftEdge(_isLeftEdge)
{
}

void LineTracing::run()
{
  int basePwm = 0;
  int turnPwm = 0;
  int edgeSign = 0;
  Pid pid(gain.kp, gain.ki, gain.kd, targetBrightness);

  // 事前条件を判定する
  if(!isMetPrecondition(pwm, targetSpeed)) {
    return;
  }

  // 左右で符号を変える
  edgeSign = isLeftEdge ? -1 : 1;

  // 呼び出し時の走行距離
  initLeftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  initRightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  // 継続条件を満たしている間ループ
  while(isMetPostcondition()) {
    // 目標速度に対するpwm値を計算
    SpeedCalculator SpeedCalculator(targetSpeed);
    basePwm = SpeedCalculator.calcPwmFromSpeed();

    // PIDで旋回値を計算
    // 目標速度に対するpwm値を計算
    turnPwm = pid.calculatePid(Measurer::getBrightness()) * edgeSign;

    // モータのPWM値をセット（0を超えないようにセット）
    int rightPwm = basePwm > 0 ? max(basePwm - (int)turnPwm, 0) : min(basePwm + (int)turnPwm, 0);
    int leftPwm = basePwm > 0 ? max(basePwm + (int)turnPwm, 0) : min(basePwm - (int)turnPwm, 0);
    Controller::setRightMotorPwm(rightPwm);
    Controller::setLeftMotorPwm(leftPwm);
  }

  // モータの停止
  Controller::stopMotor();
}

void LineTracing::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run LineTracing (targetSpeed: %.2f, targetBrightness: %d, pwm: %d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s)",
           targetSpeed, targetBrightness, pwm, gain.kp, gain.ki, gain.kd, str);
  logger.log(buf);
}
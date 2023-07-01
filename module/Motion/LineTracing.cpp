/**
 * @file   LineTracing.cpp
 * @brief  ライントレース動作の中間クラス
 * @author YKhm20020
 */

#include "LineTracing.h"
using namespace std;

LineTracing::LineTracing(double _targetSpeed, int _targetBrightness, const PidGain& _gain,
                         bool& _isLeftEdge)
  : targetSpeed(_targetSpeed),
    targetBrightness(_targetBrightness),
    gain(_gain),
    isLeftEdge(_isLeftEdge)
{
}

void LineTracing::run()
{
  int basePwm = 0;  // 初期PWM値 -100~100
  int turnPwm = 0;  // 目標速度に対するpwm値を計算
  int edgeSign = 0;
  Pid pid(gain.kp, gain.ki, gain.kd, targetBrightness);

  // 事前条件を判定する
  if(!isMetPrecondition(basePwm, targetSpeed)) {
    return;
  }

  // 左右で符号を変える
  edgeSign = isLeftEdge ? -1 : 1;

  // 呼び出し時の走行距離
  initLeftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  initRightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  // 継続条件を満たしている間ループ
  while(isMetPostcondition()) {
    // 初期pwm値を計算
    SpeedCalculator SpeedCalculator(targetSpeed);
    basePwm = SpeedCalculator.calcPwmFromSpeed();

    // 目標速度に対するpwm値を計算
    turnPwm = pid.calculatePid(Measurer::getBrightness()) * edgeSign;

    // モータのPWM値をセット（0を超えないようにセット）
    int rightPwm = basePwm > 0 ? max(basePwm - (int)turnPwm, 0) : min(basePwm + (int)turnPwm, 0);
    int leftPwm = basePwm > 0 ? max(basePwm + (int)turnPwm, 0) : min(basePwm - (int)turnPwm, 0);
    Controller::setRightMotorPwm(rightPwm);
    Controller::setLeftMotorPwm(leftPwm);

    // 10ミリ秒待機
    timer.sleep(10);
  }

  // モータの停止
  Controller::stopMotor();
}

bool LineTracing::isMetPrecondition(int basePwm, double targetSpeed)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // 初期pwm値が0の場合はwarningを出して終了する
  if(basePwm == 0) {
    logger.logWarning("The basePwm value passed to LineTracing is 0");
    return false;
  }

  // targetSpeed値が0以下の場合はwarningを出して終了する
  if(targetSpeed <= 0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to LineTracing is %lf", targetSpeed);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void LineTracing::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  // targetValueと%~のオーバーライド必須
  snprintf(buf, BUF_SIZE,
           "Run \"targetValue\" LineTracing (\"targetValue\": , targetSpeed: %.2f, "
           "targetBrightness: %d, basePwm: %d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s)",
           targetSpeed, targetBrightness, basePwm, gain.kp, gain.ki, gain.kd, str);
  logger.log(buf);
}
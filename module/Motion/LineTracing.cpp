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
  int turnPwm = 0;        // 旋回値を計算
  initialDistance = 0.0;  // 実行前の走行距離
  currentDistance = 0.0;  // 現在の走行距離
  int edgeSign = 0;
  Pid pid(gain.kp, gain.ki, gain.kd, targetBrightness);

  // 初期値を代入
  initialDistance = Mileage::calculateMileage(Measurer::getRightCount(), Measurer::getLeftCount());

  // 事前条件を判定する
  if(!isMetPrecondition(targetSpeed)) {
    return;
  }

  // 左右で符号を変える
  edgeSign = isLeftEdge ? -1 : 1;

  // 呼び出し時の走行距離
  initLeftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  initRightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  SpeedCalculator speedCalculator(targetSpeed);

  // 継続条件を満たしている間ループ
  while(isMetPostcondition()) {
    // 初期pwm値を計算
    int baseRightPwm = speedCalculator.calcRightPwmFromSpeed();
    int baseLeftPwm = speedCalculator.calcLeftPwmFromSpeed();

    // PIDで旋回値を計算
    turnPwm = pid.calculatePid(Measurer::getBrightness()) * edgeSign;

    // モータのPWM値をセット（0を超えないようにセット）
    int rightPwm = baseRightPwm > 0 ? max(baseRightPwm - (int)turnPwm, 0)
                                    : min(baseRightPwm + (int)turnPwm, 0);
    int leftPwm
        = baseLeftPwm > 0 ? max(baseLeftPwm + (int)turnPwm, 0) : min(baseLeftPwm - (int)turnPwm, 0);
    Controller::setRightMotorPwm(rightPwm);
    Controller::setLeftMotorPwm(leftPwm);

    // 10ミリ秒待機
    timer.sleep(10);
  }

  // モータの停止
  Controller::stopMotor();
}

void LineTracing::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  // targetValueと%~のオーバーライド必須
  snprintf(buf, BUF_SIZE,
           "Run \"targetValue\" LineTracing (\"targetValue\": , targetSpeed: %.2f, "
           "targetBrightness: %d, gain: "
           "(%.2f,%.2f,%.2f), isLeftEdge: %s)",
           targetSpeed, targetBrightness, gain.kp, gain.ki, gain.kd, str);
  logger.log(buf);
}
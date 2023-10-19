/**
 * @file   LineTracing.cpp
 * @brief  ライントレース動作の中間クラス
 * @author YKhm20020 bizyutyu
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
  double turnPwm = 0.0;   // 旋回値を計算
  initialDistance = 0.0;  // 実行前の走行距離
  currentDistance = 0.0;  // 現在の走行距離
  int edgeSign = 0;
  double timeConstant = 0.01;  // 旋回値用PIDに渡す時定数
  double initDeviation = double(targetBrightness) - double(Measurer::getBrightness());
  Pid pid(gain.kp, gain.ki, gain.kd, targetBrightness, initDeviation, timeConstant);

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
    double baseRightPwm = speedCalculator.calcRightPwmFromSpeed();
    double baseLeftPwm = speedCalculator.calcLeftPwmFromSpeed();

    // PIDで旋回値を計算
    turnPwm = pid.calculatePid(Measurer::getBrightness()) * edgeSign;

    // モータのPWM値をセット（0を超えないようにセット）
    double rightPwm
        = baseRightPwm > 0.0 ? max(baseRightPwm - turnPwm, 0.0) : min(baseRightPwm + turnPwm, 0.0);
    double leftPwm
        = baseLeftPwm > 0.0 ? max(baseLeftPwm + turnPwm, 0.0) : min(baseLeftPwm - turnPwm, 0.0);
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
           "targetBrightness: %d, gain: (%.2f,%.2f,%.2f), isLeftEdge: %s)",
           targetSpeed, targetBrightness, gain.kp, gain.ki, gain.kd, str);
  logger.log(buf);
}
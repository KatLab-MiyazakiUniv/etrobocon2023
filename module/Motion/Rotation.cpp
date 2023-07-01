/**
 * @file   Rotation.cpp
 * @brief  回頭動作の中間クラス
 * @author desty505
 */

#include "Rotation.h"

using namespace std;

Rotation::Rotation(int _targetValue, double _targetSpeed, bool _isClockwise)
  : targetValue(_targetValue), targetSpeed(_targetSpeed), isClockwise(_isClockwise)
{
}

void Rotation::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  // isClockwiseがtrueなら時計回り，falseなら反時計回り
  // isClockwiseは回転方向の係数
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;

  // 呼び出し時の走行距離
  initLeftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  initRightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  // 継続条件を満たしている間ループ
  while(isMetPostcondition()) {
    // PWM値を設定する
    SpeedCalculator SpeedCalculator(targetSpeed);
    int pwm = SpeedCalculator.calcPwmFromSpeed();

    // モータにPWM値をセット
    Controller::setLeftMotorPwm(pwm * leftSign);
    Controller::setRightMotorPwm(pwm * rightSign);

    // 10ミリ秒待機
    Timer.sleep(10);
  }

  // モータの停止
  Controller::stopMotor();
}

bool Rotation::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0以下の場合はwarningを出して終了する
  if(targetSpeed <= 0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to Rotation is %lf", targetSpeed);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

bool Rotation::isMetPostcondition()
{
  return false;
}

void Rotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isClockwise ? "true" : "false";

  snprintf(buf, BUF_SIZE, "Run Rotation (targetValue: %d, targetSpeed: %lf, isClockwise: %s)",
           targetValue, targetSpeed, str);
  logger.log(buf);
}
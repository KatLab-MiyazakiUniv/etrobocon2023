/**
 * @file   Rotation.cpp
 * @brief  回頭動作の中間クラス
 * @author desty505
 */

#include "Rotation.h"

using namespace std;

Rotation::Rotation(double _targetSpeed, bool _isClockwise)
  : targetSpeed(_targetSpeed), isClockwise(_isClockwise)
{
}

void Rotation::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition(targetSpeed)) {
    return;
  }

  // isClockwiseがtrueなら時計回り，falseなら反時計回り
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;

  // 呼び出し時の走行距離
  initLeftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  initRightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  // 両輪が目標距離に到達するまでループ
  while(true) {
    // 事後条件を判定する
    if(!isMetPostcondition(initLeftMileage, initRightMileage)) break;

    // PWM値を設定する
    SpeedCalculator SpeedCalculator(targetSpeed);
    int pwm = SpeedCalculator.calcPwmFromSpeed();

    // モータにPWM値をセット
    Controller::setLeftMotorPwm(pwm * leftSign);
    Controller::setRightMotorPwm(pwm * rightSign);
  }

  // モータの停止
  Controller::stopMotor();
}

bool Rotation::isMetPrecondition(double targetSpeed)
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

bool Rotation::isMetPostcondition(double initLeftMileage, double initRightMileage)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // 1周するのに必要な距離を求める
  double rotateDistance = M_PI * TREAD;

  // 回頭を初めてからの走行距離を求める
  double currentLeftDistance
      = Mileage::calculateWheelMileage(Measurer::getLeftCount()) - initLeftMileage;
  double currentRightDistance
      = Mileage::calculateWheelMileage(Measurer::getRightCount()) - initRightMileage;

  // 1周分回頭していたらtrueを返す
  if(currentLeftDistance >= rotateDistance || currentRightDistance >= rotateDistance) {
    return false;
  }
  return true;
}

void Rotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isClockwise ? "true" : "false";

  snprintf(buf, BUF_SIZE, "Run Rotation (targetValue: %d, targetSpeed: %d, isClockwise: %s)",
           targetValue, targetSpeed, str);
  logger.log(buf);
}
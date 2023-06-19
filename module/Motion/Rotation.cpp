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
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // 事前条件を判定する
  if(runPreconditionJudgement(targetSpeed)) {
    return;
  }

  // isClockwiseがtrueなら時計回り，falseなら反時計回り
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;
  double targetDistance = M_PI * TREAD / 360;  // 指定した角度に対する目標の走行距離(弧の長さ)

  // 現在の走行距離
  double leftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  double rightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  // 目標距離（呼び出し時の走行距離 ± 指定された回転量に必要な距離）
  double targetLeftDistance = leftMileage + targetDistance * leftSign;
  double targetRightDistance = rightMileage + targetDistance * rightSign;

  // 両輪が目標距離に到達するまでループ
  while(true) {
    // 残りの移動距離
    double diffLeftDistance
        = (targetLeftDistance - Mileage::calculateWheelMileage(Measurer::getLeftCount()))
          * leftSign;
    double diffRightDistance
        = (targetRightDistance - Mileage::calculateWheelMileage(Measurer::getRightCount()))
          * rightSign;

    // 事後条件を判定する
    if(runPostconditionJudgement()) break;

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

bool Rotation::runPreconditionJudgement(double targetSpeed)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0以下の場合はwarningを出して終了する
  if(targetSpeed <= 0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to Rotation is %d", targetSpeed);
    logger.logWarning(buf);
    return true;
  }

  return false;
}

bool Rotation::runPostconditionJudgement(double leftMileage, double rightMileage)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // 1周するのに必要な距離を求める
  double rotateDistance = M_PI * TREAD;

  // 回頭を初めてからの走行距離を求める
  double currentLeftDistance
      = Mileage::calculateWheelMileage(Measurer::getLeftCount()) - leftMileage;
  double currentRightDistance
      = Mileage::calculateWheelMileage(Measurer::getRightCount()) - rightMileage;

  // 1周分回頭していたらtrueを返す
  if(currentLeftDistance > rotateDistance || currentRightDistance > rotateDistance) {
    return true;
  }
  return false;
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
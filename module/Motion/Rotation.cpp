/**
 * @file   Rotation.cpp
 * @brief  回頭動作の中間クラス
 * @author desty505
 */

#include "Rotation.h"

using namespace std;

Rotation::rotation(int _targetValue, double _targetSpeed, bool _isClockwise)
  : targetValue(_targetValue), targetSpeed(_targetSpeed), isClockwise(_isClockwise)
{
}

void Rotation::run()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // targetSpeed値が0以下の場合はwarningを出して終了する
  if(targetSpeed <= 0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to Rotation is %d", targetSpeed);
    logger.logWarning(buf);
    return;
  }

  // 事前条件を判定する
  if(run_precondition_judgement(targetValue) != true) {
    return;
  }

  // isClockwiseがtrueなら時計回り，falseなら反時計回り
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;
  double targetDistance
      = M_PI * TREAD * angle / 360;  // 指定した角度に対する目標の走行距離(弧の長さ)

  // 現在の走行距離
  double leftMileage = Mileage::calculateWheelMileage(measurer.getLeftCount());
  double rightMileage = Mileage::calculateWheelMileage(measurer.getRightCount());

  // 目標距離（呼び出し時の走行距離 ± 指定された回転量に必要な距離）
  double targetLeftDistance = leftMileage + targetDistance * leftSign;
  double targetRightDistance = rightMileage + targetDistance * rightSign;

  // 両輪が目標距離に到達するまでループ
  while(true) {
    // 残りの移動距離
    double diffLeftDistance
        = (targetLeftDistance - Mileage::calculateWheelMileage(measurer.getLeftCount())) * leftSign;
    double diffRightDistance
        = (targetRightDistance - Mileage::calculateWheelMileage(measurer.getRightCount()))
          * rightSign;

    // 事後条件を判定する
    if(run_postcondition_judgement(leftMileage, rightMileage, targetLeftDistance,
                                   targetRightDistance)
       != true) {
      break;
    }

    // PWM値を設定する
    SpeedCalculator SpeedCalculator(targetSpeed);
    int pwm = SpeedCalculator.calcPwmFromSpeed();

    // モータにPWM値をセット
    controller.setLeftMotorPwm(pwm * leftSign);
    controller.setRightMotorPwm(pwm * rightSign);

    // 10ミリ秒待機
    controller.sleep();
  }

  // モータの停止
  controller.stopMotor();
}

bool Straight::run_precondition_judgement(int targetValue)
{
  return true;
}

bool Straight::run_postcondition_judgement(double leftMileage, double rightMileage)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // 1周するのに必要な距離を求める
  double rotateDistance = M_PI * TREAD;

  // 回頭を初めてからの走行距離を求める
  double currentLeftDistance
      = Mileage::calculateWheelMileage(measurer.getLeftCount()) - leftMileage;
  double currentRightDistance
      = Mileage::calculateWheelMileage(measurer.getRightCount()) - rightMileage;

  // 1周分回頭していたらfalseを返す
  if(currentLeftDistance > rotateDistance || currentRightDistance > rotateDistance) {
    return false;
  }
  return true;
}

void Rotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isClockwise ? "true" : "false";

  snprintf(buf, BUF_SIZE, "Run Rotation (angle: %d, pwm: %d, isClockwise: %s)", angle, pwm, str);
  logger.log(buf);
}
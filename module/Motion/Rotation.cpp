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
  if(!isMetPrecondition()) {
    return;
  }

  // isClockwiseがtrueなら時計回り，falseなら反時計回り
  // isClockwiseは回転方向の係数
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;

  // 呼び出し時の走行距離
  double initLeftMileage = Mileage::calculateWheelMileage(Measurer::getLeftCount());
  double initRightMileage = Mileage::calculateWheelMileage(Measurer::getRightCount());

  SpeedCalculator speedCalculator(targetSpeed);

  // 継続条件を満たしている間ループ
  while(isMetPostcondition(initLeftMileage, initRightMileage, leftSign, rightSign)) {
    // PWM値を設定する
    int leftPwm = speedCalculator.calcLeftPwmFromSpeed();
    int rightPwm = speedCalculator.calcRightPwmFromSpeed();

    // モータにPWM値をセット
    Controller::setLeftMotorPwm(leftPwm * leftSign);
    Controller::setRightMotorPwm(leftPwm * rightSign);

    // 10ミリ秒待機
    timer.sleep(10);
  }

  // モータの停止
  Controller::stopMotor();
}
/**
 * @file   PwmRotation.cpp
 * @brief  Pwm指定回頭動作
 * @author Negimarge
 */

#include "PwmRotation.h"

using namespace std;

PwmRotation::PwmRotation(int _angle, int _pwm, bool _isClockwise)
  : angle(_angle), pwm(_pwm), isClockwise(_isClockwise)
{
}

void PwmRotation::run()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // pwm値が0以下の場合はwarningを出して終了する
  if(pwm <= 0) {
    snprintf(buf, BUF_SIZE, "The pwm value passed to PwmRotation is %d", pwm);
    logger.logWarning(buf);
    return;
  }
  // angleが0以下の場合はwarningを出して終了する
  if(angle <= 0 || angle >= 360) {
    snprintf(buf, BUF_SIZE, "The angle value passed to PwmRotation is %d", angle);
    logger.logWarning(buf);
    return;
  }

  // isClockwiseがtrueなら時計回り，falseなら反時計回り
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;
  double targetDistance
      = M_PI * TREAD * angle / 360;  // 指定した角度に対する目標の走行距離(弧の長さ)
  // 目標距離（呼び出し時の走行距離 ± 指定された回転量に必要な距離）
  double targetLeftDistance
      = Mileage::calculateWheelMileage(Measurer::getLeftCount()) + targetDistance * leftSign;
  double targetRightDistance
      = Mileage::calculateWheelMileage(Measurer::getRightCount()) + targetDistance * rightSign;

  // 両輪が目標距離に到達するまでループ
  while(leftSign != 0 || rightSign != 0) {
    // 残りの移動距離
    double diffLeftDistance
        = (targetLeftDistance - Mileage::calculateWheelMileage(Measurer::getLeftCount()))
          * leftSign;
    double diffRightDistance
        = (targetRightDistance - Mileage::calculateWheelMileage(Measurer::getRightCount()))
          * rightSign;

    // 目標距離に到達した場合
    if(diffLeftDistance <= 0) {
      leftSign = 0;
    }
    if(diffRightDistance <= 0) {
      rightSign = 0;
    }

    // モータにPWM値をセット
    Controller::setLeftMotorPwm(pwm * leftSign);
    Controller::setRightMotorPwm(pwm * rightSign);

    // 10ミリ秒待機
    timer.sleep();
  }

  // モータの停止
  Controller::stopMotor();
}

void PwmRotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isClockwise ? "true" : "false";

  snprintf(buf, BUF_SIZE, "Run PwmRotation (angle: %d, pwm: %d, isClockwise: %s)", angle, pwm, str);
  logger.log(buf);
}
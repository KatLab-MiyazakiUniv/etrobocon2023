/**
 * @file   AngleRotation.cpp
 * @brief  角度指定による回頭動作
 * @author desty505
 */

#include "AngleRotation.h"

using namespace std;

AngleRotation::AngleRotation(int _targetAngle, double _targetSpeed, bool _isClockwise)
  : Rotation(_targetSpeed, _isClockwise), targetAngle(_targetAngle){};

bool AngleRotation::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0以下の場合はwarningを出して終了する
  if(targetSpeed <= 0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to Rotation is %lf", targetSpeed);
    logger.logWarning(buf);
    return false;
  }

  // targetAngleが0以下の場合はwarningを出して終了する
  if(targetAngle <= 0 || targetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The targetAngle value passed to Rotation is %d", targetAngle);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

bool AngleRotation::isMetPostcondition(double initLeftMileage, double initRightMileage,
                                       int leftSign, int rightSign)
{
  double targetDistance
      = M_PI * TREAD * targetAngle / 360;  // 指定した角度に対する目標の走行距離(弧の長さ)

  // 目標距離（呼び出し時の走行距離 ± 指定された回転量に必要な距離）
  double targetLeftDistance = initLeftMileage + targetDistance * leftSign;
  double targetRightDistance = initRightMileage + targetDistance * rightSign;

  // 残りの移動距離を算出
  double diffLeftDistance
      = (targetLeftDistance - Mileage::calculateWheelMileage(Measurer::getLeftCount())) * leftSign;
  double diffRightDistance
      = (targetRightDistance - Mileage::calculateWheelMileage(Measurer::getRightCount()))
        * rightSign;

  // 目標距離に到達した場合
  if(diffLeftDistance <= 0 && diffRightDistance <= 0) {
    return false;
  }
  return true;
}

void AngleRotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isClockwise ? "true" : "false";

  snprintf(buf, BUF_SIZE, "Run Rotation (targetAngle: %d, targetSpeed: %lf, isClockwise: %s)",
           targetAngle, targetSpeed, str);
  logger.log(buf);
}
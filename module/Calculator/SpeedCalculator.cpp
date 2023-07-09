/**
 *  @file SpeedCalculator.cpp
 *  @brief 走行速度を指定するクラス
 *  @author miyashita64 bizyutyu
 */
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(double _targetSpeed)
  : targetSpeed(_targetSpeed), pid(0.001, 0.000000001, 0.0001, _targetSpeed)
{
  rightPwm = 0.0;
  leftPwm = 0.0;
  int rightAngle = Measurer::getRightCount();
  int leftAngle = Measurer::getLeftCount();
  prevRightMileage = Mileage::calculateWheelMileage(rightAngle);
  prevLeftMileage = Mileage::calculateWheelMileage(leftAngle);
  prevRightTime = timer.now();
  prevLeftTime = timer.now();
}

int SpeedCalculator::calcRightPwmFromSpeed()
{
  // 右タイヤの回転角度を取得
  int rightAngle = Measurer::getRightCount();
  // 右タイヤの走行距離を算出
  double currentRightMileage = Mileage::calculateWheelMileage(rightAngle);
  double diffRightMileage = currentRightMileage - prevRightMileage;
  // 走行時間を算出
  int currentRightTime = timer.now();
  double diffRightTime = (double)(currentRightTime - prevRightTime);
  // 右タイヤの走行速度を算出
  double currentRightSpeed
      = (diffRightMileage == 0.0) ? -targetSpeed : calcSpeed(diffRightMileage, diffRightTime);
  // 走行速度に相当する右タイヤのPWM値を算出
  rightPwm += pid.calculatePid(currentRightSpeed, diffRightTime);
  // メンバを更新
  prevRightMileage = currentRightMileage;
  prevRightTime = currentRightTime;

  return (int)rightPwm;
}

int SpeedCalculator::calcLeftPwmFromSpeed()
{
  // 左タイヤの回転角度を取得
  int leftAngle = Measurer::getLeftCount();
  // 左タイヤの走行距離を算出
  double currentLeftMileage = Mileage::calculateWheelMileage(leftAngle);
  double diffLeftMileage = currentLeftMileage - prevLeftMileage;
  // 走行時間を算出
  int currentLeftTime = timer.now();
  double diffLeftTime = (double)(currentLeftTime - prevLeftTime);
  // 左タイヤの走行速度を算出
  double currentLeftSpeed
      = (diffLeftMileage == 0.0) ? -targetSpeed : calcSpeed(diffLeftMileage, diffLeftTime);
  // 走行速度に相当する左タイヤのPWM値を算出
  leftPwm += pid.calculatePid(currentLeftSpeed, diffLeftTime);
  // メンバを更新
  prevLeftMileage = currentLeftMileage;
  prevLeftTime = currentLeftTime;

  return (int)leftPwm;
}

double SpeedCalculator::calcSpeed(double diffMileage, double diffTime)
{
  // 走行時間が0のとき、0を返す
  if(diffTime == 0.0) return 0.0;
  // 走行速度[mm/s]を算出(= 1000 * mm / ms)
  double speed = 1000 * diffMileage / diffTime;
  return speed;
}

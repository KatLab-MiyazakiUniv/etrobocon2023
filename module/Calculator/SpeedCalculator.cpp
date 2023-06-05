/**
 *  @file SpeedCalculator.cpp
 *  @brief 走行速度を指定するクラス
 *  @author miyashita64 bizyutyu
 */
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(double _targetSpeed)
  : targetSpeed(_targetSpeed), pid(0.001, 0.000000001, 0.0001, _targetSpeed)
{
  pwm = 0.0;
  int rightAngle = Measurer::getRightCount();
  int leftAngle = Measurer::getLeftCount();
  prevMileage = Mileage::calculateMileage(rightAngle, leftAngle);
  prevTime = timer.now();
}

int SpeedCalculator::calcPwmFromSpeed()
{
  // 左右タイヤの回転角度を取得
  int rightAngle = Measurer::getRightCount();
  int leftAngle = Measurer::getLeftCount();
  // 走行距離を算出
  double currentMileage = Mileage::calculateMileage(rightAngle, leftAngle);
  double diffMileage = currentMileage - prevMileage;
  // 走行時間を算出
  int currentTime = timer.now();
  double diffTime = (double)(currentTime - prevTime);
  // 走行速度を算出
  double currentSpeed = (diffMileage == 0.0) ? -targetSpeed : calcSpeed(diffMileage, diffTime);
  // 走行速度に相当するPWM値を算出
  pwm += pid.calculatePid(currentSpeed, diffTime);
  // メンバを更新
  prevMileage = currentMileage;
  prevTime = currentTime;

  return (int)pwm;
}

double SpeedCalculator::calcSpeed(double diffMileage, double diffTime)
{
  // 走行時間が0のとき、0を返す
  if(diffTime == 0.0) return 0.0;
  // 走行速度[mm/s]を算出(= 1000 * mm / ms)
  double speed = 1000 * diffMileage / diffTime;
  return speed;
}

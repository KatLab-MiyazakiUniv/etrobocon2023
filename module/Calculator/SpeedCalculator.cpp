/**
 *  @file SpeedCalculator.cpp
 *  @brief 走行速度を指定するクラス
 *  @author miyashita64 bizyutyu
 */
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(double _targetSpeed) : pid(0.8, 0.1, 0.01, _targetSpeed)
{
  int rightAngle = Measurer::getRightCount();
  int leftAngle = Measurer::getLeftCount();
  prevMileage = Mileage::calculateMileage(rightAngle, leftAngle);
  prevTime = clock.now();
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
  uint64_t currentTime = clock.now();
  int diffTime = currentTime - prevTime;
  // 走行速度を算出
  double currentSpeed = calcSpeed(diffMileage, diffTime);
  // 走行速度に相当するPWM値を算出
  double pwm = pid.calculatePid(currentSpeed, diffTime);
  // メンバを更新
  prevMileage = currentMileage;
  prevTime = currentTime;
  return pwm;
}

double SpeedCalculator::calcSpeed(double diffMileage, double diffTime)
{
  // 走行時間が0のとき、0を返す
  if(diffTime == 0) return 0;
  // 走行速度を算出
  double speed = diffMileage / diffTime;
  return speed;
}

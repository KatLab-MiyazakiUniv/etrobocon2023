/**
 *  @file SpeedCalculator.cpp
 *  @brief 走行速度を指定するクラス
 *  @author miyashita64 bizyutyu
 */
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(double _targetSpeed) : pid(300.0, 0.1, 0.01, _targetSpeed)
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
  double currentSpeed = calcSpeed(diffMileage, diffTime);
  // 走行速度に相当するPWM値を算出
  pwm += pid.calculatePid(currentSpeed, diffTime);
  // メンバを更新
  prevMileage = currentMileage;
  prevTime = currentTime;
  printf("diffTime=%d\n", diffTime);
  printf("diffMil=%f\n", diffMileage);
  printf("speed=%f\n", currentSpeed);
  printf("MILEAGE=%f\n", currentMileage);
  printf("PWM=%f\n", pwm);

  return (int)pwm;
}

double SpeedCalculator::calcSpeed(double diffMileage, double diffTime)
{
  // 走行時間が0のとき、0を返す
  if(diffTime == 0) return 0;
  // 走行速度を算出
  double speed = diffMileage / diffTime;
  return speed;
}

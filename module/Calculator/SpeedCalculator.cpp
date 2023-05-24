/**
 **  @file SpeedCalculator.cpp
 **  @brief 走行距離を計算するクラス
 **  @author miyashita64 bizyutyu
 **/
#include <chrono>
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(double _targetValue) : pid(0.8, 0.1, 0.01, _targetValue){
  int rightAngle = Measurer::getRightCount();
  int leftAngle = Measurer::getLeftCount();
  prevMileage = Mileage::calculateMileage(rightAngle, leftAngle);
  prevTime = std::chrono::high_resolution_clock::now();
}

int SpeedCalculator::calcPwmFromSpeed(double _targetSpeed)
{
  // 左右タイヤの回転角度を取得
  int rightAngle = Measurer::getRightCount();
  int leftAngle = Measurer::getLeftCount();
  // 走行距離を算出
  double currentMileage = Mileage::calculateMileage(rightAngle, leftAngle);
  auto currentTime = std::chrono::high_resolution_clock::now();
  double diffMileage = currentMileage - prevMileage;
  // 走行時間を算出
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevTime);
  double diffTime = duration.count();
  // メンバを更新
  prevMileage = currentMileage;
  prevTime = currentTime;
  // 走行速度を算出
  double currentSpeed = calcSpeed(diffMileage, diffTime);
  // 走行速度に相当するPWM値を算出
  double pwm = pid.calculatePid(currentSpeed, diffTime);
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

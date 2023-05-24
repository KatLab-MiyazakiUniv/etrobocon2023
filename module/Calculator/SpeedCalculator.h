/**
 * @file SpeedCalculator.h
 * @brief 走行速度を指定するクラス
 * @author miyashita64 bizyutyu
 */

#ifndef SPEED_CALCULATOR_H
#define SPEED_CALCULATOR_H

#include "Controller.h"
#include "Measurer.h"
#include "Mileage.h"

class SpeedCalculator {
 public:
  /**
   * @brief コンストラクタ
   * @param _targetSpeed 目標とする走行速度[Todo]
   */
  SpeedCalculator(double _targetSpeed);

  /**
   * @brief 目標とする走行速度に相当するPWM値を算出する
   * @return 走行体全体の累計走行距離[mm]
   */
  int calcPwmFromSpeed();

 private:
  /**
   * @brief 走行速度を算出する
   * @return 走行速度[Todo]
   */
  double calcSpeed(double diffMileage, double diffTimed);
  
  Pid pid;
  double prevMileage;
  std::chrono::high_resolution_clock::time_point prevTime;
};
#endif
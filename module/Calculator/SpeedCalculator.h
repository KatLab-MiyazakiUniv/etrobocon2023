/**
 * @file SpeedCalculator.h
 * @brief 走行速度を指定するクラス
 * @author miyashita64 bizyutyu
 */

#ifndef SPEED_CALCULATOR_H
#define SPEED_CALCULATOR_H

#include "Measurer.h"
#include "Controller.h"
#include "Mileage.h"
#include "Pid.h"
#include "Timer.h"

class SpeedCalculator {
 public:
  /**
   * @brief コンストラクタ
   * @param _targetSpeed 目標とする走行速度[mm/s]
   */
  SpeedCalculator(double _targetSpeed);

  /**
   * @brief 目標とする走行速度に相当するPWM値を算出する
   * @return 走行速度に相当するPWM値
   */
  int calcPwmFromSpeed();

 private:
  Pid pid;
  Timer timer;
  const double targetSpeed;
  double pwm;
  double prevMileage;
  int prevTime;

  /**
   * @brief 走行速度を算出する
   * @param diffMileage 移動距離[mm]
   * @param diffTime 移動時間[ms]
   * @return 走行速度[mm/s]
   */
  double calcSpeed(double diffMileage, double diffTime);
};
#endif
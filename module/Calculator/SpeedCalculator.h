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
   * @brief 目標とする走行速度に相当する右車輪のPWM値を算出する
   * @return 走行速度に相当する右タイヤのPWM値
   */
  int calcRightPwmFromSpeed();

  /**
   * @brief 目標とする走行速度に相当する左車輪のPWM値を算出する
   * @return 走行速度に相当する左タイヤのPWM値
   */
  int calcLeftPwmFromSpeed();

 private:
  const double targetSpeed;
  Pid rightPid;
  Pid leftPid;
  Timer timer;
  double rightPwm;
  double leftPwm;
  double prevRightMileage;
  double prevLeftMileage;
  int prevRightTime;
  int prevLeftTime;
  static constexpr double K_P = 0.001;
  static constexpr double K_I = 0.000000001;
  static constexpr double K_D = 0.0001;

  /**
   * @brief 走行速度を算出する
   * @param diffMileage 移動距離[mm]
   * @param diffTime 移動時間[ms]
   * @return 走行速度[mm/s]
   */
  double calcSpeed(double diffMileage, double diffTime);
};
#endif
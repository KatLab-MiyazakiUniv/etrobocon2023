/**
 * @file   Straight.h
 * @brief  直進動作の抽象クラス
 * @author Negimarge
 */

#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "Motion.h"
#include "Pid.h"
#include "Mileage.h"
#include "SpeedCalculator.h"
#include "Timer.h"
#include "SystemInfo.h"

class Straight : public Motion {
 public:
  /**
   * コンストラクタ　オーバーライド必須
   * @param _targetValue 目標値 目標距離か目標色　オーバーライド必須
   * @param _speed       目標速度[mm/s]
   */
  Straight(int _targetValue, double _speed);

  /**
   * @brief 直進する
   */
  void run();

  /**
   * オーバーライド必須
   * @brief 直進する際の事前条件判定をする
   */
  virtual bool runPreconditionJudgement();

  /**
   * オーバーライド必須
   * @brief 直進する際の事後条件判定をする　返り値が真でモーターが止まる
   * @param initialRightMotorCount 初期左モータ角位置
   * @param initialLeftMotorCount  初期右モータ角位置
   * @param currentRightMotorCount 現在左モータ角位置
   * @param currentLeftMotorCount  現在右モータ角位置
   */
  virtual bool runPostconditionJudgement(int initialRightMotorCount, int initialLeftMotorCount,
                                         int currentRightMotorCount, int currentLeftMotorCount)
      = 0;

  /**
   * @brief 実行のログを取る オーバーライド必須
   */
  virtual void logRunning();

 protected:
  // 目標値は継承後に追加する
  static constexpr int MIN_PWM = 40;  // 静止時から走行体がモーターを動かせないPWM値
  double targetSpeed;                 // 目標速度[mm/s]
  int targetValue;                    // 目標値 目標距離か目標色
  class Timer Timer;
};

#endif
/**
 * @file   Straight.h
 * @brief  直進動作の抽象クラス
 * @author Negimarge bizyutyu
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
   * @brief コンストラクタ
   * @param _speed       目標速度[mm/s]
   * @note オーバーライド必須
   */
  Straight(double _speed);

  /**
   * @brief 直進する
   */
  void run();

  /**
   * @brief 直進する際の事前条件判定をする
   * @note オーバーライド必須
   */
  virtual bool isRunPreconditionJudgement();

  /**
   * @brief 直進する際の終了条件判定をする　返り値が真でモーターが止まる
   * @note オーバーライド必須
   */
  virtual bool isRunPostconditionJudgement() = 0;

  /**
   * @brief 実行のログを取る
   * @note オーバーライド必須
   */
  virtual void logRunning();

 protected:
  // 目標値は継承後に追加する
  static constexpr double MIN_PWM = 40.0;  // 静止時から走行体がモーターを動かせないPWM値
  double targetSpeed;                      // 目標速度[mm/s]
  int initialRightMotorCount;              // 初期右輪モーター距離
  int initialLeftMotorCount;               // 初期左輪モーター距離
  double initialRightDistance;             // 初期右輪距離
  double initialLeftDistance;              // 初期左輪距離
  int currentRightMotorCount;              // 現在右輪モーター距離
  int currentLeftMotorCount;               // 現在左輪モーター距離
  double currentRightDistance;             // 現在右輪距離
  double currentLeftDistance;              // 現在左輪距離
  Timer timer;
};

#endif
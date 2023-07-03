/**
 * @file   DistanceStraight.h
 * @brief  目標距離まで直進するクラス
 * @author Negimarge
 */

#ifndef DISTANCESTRAIGHT_H
#define DISTANCESTRAIGHT_H

#include "Straight.h"

class DistanceStraight : public Straight {
 public:
  /**
   * コンストラクタ
   * @param _targetDistance 目標距離
   * @param _speed       目標速度[mm/s]
   */
  DistanceStraight(double _targetDiatance, double _speed);

  /**
   * @brief 直進する
   */
  void run();

  /**
   * @brief 直進する際の事前条件判定をする
   */
  virtual bool isRunPreconditionJudgement() override;

  /**
   * @brief 直進する際の事後条件判定をする　返り値が真でモーターが止まる
   */
  virtual bool isRunPostconditionJudgement() override;

  /**
   * @brief 実行のログを取る
   */
  virtual void logRunning() override;

 private:
  double targetDistance;  // 目標距離
};

#endif
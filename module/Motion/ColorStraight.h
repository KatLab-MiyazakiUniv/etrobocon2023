/**
 * @file   ColorStraight.cpp
 * @brief  色まで直進するクラス
 * @author Negimarge
 */

#ifndef COLORSTRAIGHT_H
#define COLORSTRAIGHT_H

#include "Straight.h"
#include "ColorJudge.h"

class ColorStraight : public Straight {
 public:
  /**
   * コンストラクタ
   * @param _targetColor 目標色
   * @param _speed       目標速度[mm/s]
   */
  ColorStraight(COLOR _targetColor, double _speed);

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
  COLOR targetColor;  // 目標色
};

#endif
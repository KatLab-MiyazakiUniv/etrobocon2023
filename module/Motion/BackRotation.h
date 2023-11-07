/**
 * @file BackRotation.h
 * @brief 後ろを向くクラス
 * @author miyashita64
 */

#ifndef BACK_ROTATION_H
#define BACK_ROTATION_H

#include "PwmRotation.h"
#include "EdgeChanging.h"
#include "BlockAreaMotion.h"

class BackRotation : public BlockAreaMotion {
 public:
  /**
   * コンストラクタ
   */
  BackRotation(bool& _isLeftEdge);

  /**
   * @brief 交点サークルから後ろを見る
   */
  void run() override;

  /**
   * @brief 後ろを向く際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  int targetAngle = 120;  // 目標回頭角度(180度回頭したい。90度が60指定なので暫定で120)
  int prPwm = 100;          // 角度指定回頭の目標PWM
  bool isClockwise = true;  // 回頭の方向は左回り
  bool& isLeftEdge;
};

#endif
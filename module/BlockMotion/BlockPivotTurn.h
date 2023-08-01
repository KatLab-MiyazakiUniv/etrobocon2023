/**
 * @file BlockPivotTurn.h
 * @brief ピボットターン設置クラス
 * @author YKhm20020
 */

#ifndef BLOCK_PIVOT_TURN_H
#define BLOCK_PIVOT_TURN_H

#include "DistanceStraight.h"
#include "AngleRotation.h"
#include "BlockMotion.h"
// #include "ArmMotion.h"
// #include "BlockThrower.h"
#include "InCrossLeft.h"
#include "InCrossRight.h"
#include "Measurer.h"
#include "Timer.h"

class BlockPivotTurn : public BlockMotion {
 public:
  // コンストラクタ
  BlockPivotTurn();

  /**
   * @brief ピボットターン設置する
   * @param isClockwise ピボットターンする向き true: 右回転
   */
  void setBlockPivotTurn(bool isClockwise);

 private:
  // BlockThrower blockThrower;
  Timer timer;

  const double TREAD = 140;  // 走行体のトレッド幅（両輪の間の距離）[mm]
  const int MIN_PWM = 10;    // モーターパワーの最小値
};

#endif
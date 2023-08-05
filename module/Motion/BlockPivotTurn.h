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
  BlockPivotTurn(bool _isClockwise);

  /**
   * @brief ピボットターン設置する
   */
  void setBlockPivotTurn();

 private:
  // BlockThrower blockThrower;
  bool isClockwise;  // 回頭方向 true:時計回り, false:反時計回り
  Timer timer;

  const double TREAD = 140;  // 走行体のトレッド幅（両輪の間の距離）[mm]
  const int MIN_PWM = 10;    // モーターパワーの最小値
};

#endif
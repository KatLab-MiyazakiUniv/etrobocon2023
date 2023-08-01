/**
 * @file InCrossStraight.h
 * @brief 交点内移動（直進）のクラス
 * @author YKhm20020
 */

#ifndef IN_CROSS_STRAIGHT_H
#define IN_CROSS_STRAIGHT_H

#include "DistanceStraight.h"
#include "BlockMotion.h"

class InCrossStraight : public BlockMotion {
 public:
  // コンストラクタ
  InCrossStraight();

  /**
   * @brief 交点サークル上を前に進む
   */
  void runForward(void);
};

#endif
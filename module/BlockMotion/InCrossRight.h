/**
 * @file InCrossRight.h
 * @brief 交点内移動（右折）のクラス
 * @author YKhm20020
 */

#ifndef IN_CROSS_RIGHT_H
#define IN_CROSS_RIGHT_H

#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "EdgeChanging.h"
#include "BlockMotion.h"

class InCrossRight : public BlockMotion {
 public:
  InCrossRight();

  /**
   * @brief 交点サークル上を右に進む
   */
  void runRight(void);
};

#endif
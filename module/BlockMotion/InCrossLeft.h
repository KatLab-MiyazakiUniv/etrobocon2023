/**
 * @file InCrossLeft.h
 * @brief 交点内移動（左折）のクラス
 * @author YKhm20020
 */

#ifndef IN_CROSS_LEFT_H
#define IN_CROSS_LEFT_H

#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "EdgeChanging.h"
#include "BlockMotion.h"

class InCrossLeft : public BlockMotion {
 public:
  InCrossLeft();

  /**
   * @brief 交点サークル上を左に進む
   */
  void runLeft(void);
};

#endif
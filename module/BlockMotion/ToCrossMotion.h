/**
 * @file ToCrossMotion.h
 * @brief 交点間移動のクラス
 * @author YKhm20020
 */

#ifndef TO_CROSS_MOTION_H
#define TO_CROSS_MOTION_H

#include "LineTracer.h"
#include "StraightRunner.h"
#include "BlockMotion.h"

class ToCrossMotion : public BlockMotion {
 public:
  ToCrossMotion();

  /**
   * @brief 交点間を移動する
   */
  void runToCross(void);
};

#endif
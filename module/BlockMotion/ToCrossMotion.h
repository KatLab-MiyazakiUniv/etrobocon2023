/**
 * @file ToCrossMotion.h
 * @brief 交点間移動のクラス
 * @author YKhm20020
 */

#ifndef TO_CROSS_MOTION_H
#define TO_CROSS_MOTION_H

#include "ColorLineTracing.h"
#include "DistanceStraight.h"
#include "ColorJudge.h"
#include "Measurer.h"
#include "BlockMotion.h"

class ToCrossMotion : public BlockMotion {
 public:
  ToCrossMotion(bool& _isLeftEdge);

  /**
   * @brief 交点間を移動する
   */
  void runToCross(void);
};

#endif
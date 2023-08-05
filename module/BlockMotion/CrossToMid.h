/**
 * @file CrossToMid.h
 * @brief サークルの交点から交点へ移動するクラス
 * @author YKhm20020
 */

#ifndef CROSS_TO_MID_H
#define CROSS_TO_MID_H

#include "InCrossStraight.h"
#include "InCrossLeft.h"
#include "InCrossRight.h"
#include "DistanceLineTracing.h"

class CrossToMid : public BlockMotion {
 public:
  CrossToMid(int _direction, bool& _isLeftEdge);

  /**
   * @brief サークルの交点から交点へ移動
   */
  void run(void);

 private:
  int direction;  // どの方向にある直線の中点に進むか。時計の数字。
  bool& isLeftEdge;   // エッジの左右判定(true:左エッジ, false:右エッジ) 
};

#endif

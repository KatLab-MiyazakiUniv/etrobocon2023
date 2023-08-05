/**
 * @file ToCrossMotion.h
 * @brief 交点間移動のクラス
 * @author YKhm20020
 */

#ifndef TO_CROSS_MOTION_H
#define TO_CROSS_MOTION_H

#include "ColorLineTracing.h"
#include "ColorJudge.h"
#include "Measurer.h"
#include "BlockMotion.h"

class ToCrossMotion : public BlockMotion {
 public:
  ToCrossMotion(COLOR _targetColor, bool& _isLeftEdge);

  /**
   * @brief 交点間を移動する
   */
  void runToCross();

 private:
  COLOR targetColor;  // 目標色
  bool& isLeftEdge;   // エッジの左右判定(true:左エッジ, false:右エッジ)
};

#endif
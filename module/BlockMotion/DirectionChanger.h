/**
 * @file DirectionChanger.h
 * @brief 方向転換クラス
 * @author YKhm20020
 */

#ifndef DIRECTION_CHANGER_H
#define DIRECTION_CHANGER_H

#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "ColorStraight.h"
#include "EdgeChanging.h"
#include "ColorJudge.h"
#include "Measurer.h"
#include "BlockMotion.h"

class DirectionChanger : public BlockMotion {
 public:
  DirectionChanger(COLOR _targetColor, bool& _isLeftEdge);

  /**
   * @brief 方向転換する
   * @param rotateAngle 回転角度（deg） -180 < angle <= 180 45度刻み
   * @param changeAngle 設置動作の直前方向から見て回頭する角度
   */
  void changeDirection(int rotateAngle, int changeAngle);

 private:
  COLOR targetColor;  // 目標色
  bool& isLeftEdge;   // エッジの左右判定(true:左エッジ, false:右エッジ)
};

#endif
/**
 * @file DirectionChanger.h
 * @brief 方向転換クラス
 * @author YKhm20020
 */

#ifndef DIRECTION_CHANGER_H
#define DIRECTION_CHANGER_H

#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "EdgeChanging.h"
#include "BlockMotion.h"

class DirectionChanger : public BlockMotion {
 public:
  DirectionChanger();

  /**
   * @brief 方向転換する
   * @param ratateAngle 回転角度（deg） -180 < angle <= 180 45度刻み
   * @param changeAngle 設置動作の直前方向から見て回頭する角度
   */
  void changeDirection(int rotateAngle, int changeAngle);
};

#endif
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
  DirectionChanger(COLOR _targetColor, double _targetDistance, double _targetSpeed,
                   bool& _isLeftEdge, int _rotateAngle, int _changeAngle);

  /**
   * @brief 方向転換する
   */
  void run() override;

  /**
   * @brief 方向転換する際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  COLOR targetColor;      // 目標色
  double targetDistance;  // 目標距離
  double targetSpeed;     // 目標速度 [mm/s]
  bool& isLeftEdge;       // エッジの左右判定(true:左エッジ, false:右エッジ)
  int rotateAngle;        // 回転角度（deg） -180 < angle <= 180 45度刻み
  int changeAngle;        // 設置動作の直前方向から見て回頭する角度
};

#endif
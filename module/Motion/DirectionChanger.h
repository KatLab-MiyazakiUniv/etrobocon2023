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
                   int _targetAngle, bool _isClockwise, bool& _isLeftEdge, bool _nextEdge);

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
  int targetAngle;        // 目標回頭角度
  bool isClockwise;       // 回頭方向 (true:時計回り, false:反時計回り)
  bool& isLeftEdge;       // エッジの左右判定 (true:左エッジ, false:右エッジ)
  bool nextEdge;          // 方向転換後のエッジ
};

#endif
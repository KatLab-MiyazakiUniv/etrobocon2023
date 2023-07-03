/**
 * @file   AngleRotation.h
 * @brief  角度指定による回頭動作
 * @author desty505
 */

#ifndef ANGLEROTATION_H
#define ANGLEROTATION_H

#include "Rotation.h"

class AngleRotation : public Rotation {
 public:
  /**
   * コンストラクタ
   * @param _targetAngle 目標角度
   * @param _targetSpeed 目標速度
   * @param _isClockwise 回頭方向 true:時計回り, false:反時計回り
   */
  AngleRotation(int _targetAngle, double _targetSpeed, bool _isClockwise);

  /**
   * @brief 回頭する
   */
  using Rotation::run;

  /**
   * @brief 回頭する際の事前条件判定をする
   */
  bool isMetPrecondition() override;

  /**
   * @brief 回頭する際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @param leftSign 左車輪の回転方向
   * @param rightSign 右車輪の回転方向
   */
  bool isMetPostcondition(int leftSign, int rightSign) override;

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  int targetAngle;  // 目標角度
};
#endif
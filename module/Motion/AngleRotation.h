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
   * @param _isClockwise 回頭方向 ture:時計回り, false:反時計回り
   */
  AngleRotation(int _targetAngle, double _targetSpeed, bool _isClockwise);

  /**
   * @brief 回頭する
   */
  void run();

  /**
   * @brief 回頭する際の事前条件判定をする
   * @param targetAngle 目標角度
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */
  bool isMetPrecondition(int targetAngle, double targetSpeed) override;

  /**
   * @brief 回頭する際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @param targetAngle 目標角度
   * @param initLeftMileage   回頭を始めた時点での左車輪の走行距離
   * @param initRightMileage  回頭を始めた時点での右車輪の走行距離
   * @param leftSign 左車輪の回転方向
   * @param rightSign 右車輪の回転方向
   * @note オーバーライド必須
   */
  bool isMetPostcondition(int targetAngle, double initLeftMileage, double initRightMileage,
                          int leftSign, int rightSign) override;

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  int targetAngle;          // 目標角度
  double targetSpeed;       // 目標速度
  bool isClockwise;         // 回頭方向 ture:時計回り, false:反時計回り
  double initLeftMileage;   // クラス呼び出し時の左車輪の走行距離
  double initRightMileage;  // クラス呼び出し時の右車輪の走行距離
};
#endif
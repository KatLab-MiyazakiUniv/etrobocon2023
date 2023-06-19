/**
 * @file   Rotation.h
 * @brief  回頭動作の中間クラス
 * @author desty505
 */

#ifndef ROTATION_H
#define ROTATION_H

#include "Motion.h"
#include "Mileage.h"
#include "SpeedCalculator.h"
#include "SystemInfo.h"

class Rotation : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _targetValue 回転角度(deg) 0~360
   * @param _targetSpeed 目標速度
   * @param _isClockwise 回頭方向 ture:時計回り, false:反時計回り
   */
  Rotation(int _targetValue, double _targetSpeed, bool _isClockwise);

  /**
   * @brief 回頭する
   */
  void run();

  /**
   * @brief 直進する際の事前条件判定をする
   * @param targetValue 回頭の条件値　色または回頭角度
   * @note オーバーライド必須
   */
  virtual bool runPreconditionJudgement(int targetValue);

  /**
   * @brief 直進する際の事後条件判定をする　返り値が真でモーターが止まる
   * @param leftMileage   回頭を始めた時点での左車輪の走行距離
   * @param rightMileage  回頭を始めた時点での右車輪の走行距離
   * @note オーバーライド必須
   */
  virtual bool runPostconditionJudgement(double leftMileage, double rightMileage);

  /**
   * @brief 実行のログを取る
   */
  void logRunning();

 private:
  int targetValue;   // 回転角度(deg) 0~360 もしくは 指定色
  int targetSpeed;   // 目標速度
  bool isClockwise;  // 回頭方向 ture:時計回り, false:反時計回り
};
#endif
/**
 * @file   Rotation.h
 * @brief  回頭動作の中間クラス
 * @author desty505
 */

#ifndef ROTATION_H
#define ROTATION_H

#include "Motion.h"
#include "Mileage.h"
#include "Timer.h"
#include "SpeedCalculator.h"
#include "SystemInfo.h"

class Rotation : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _targetValue 目標値
   * @param _targetSpeed 目標速度
   * @param _isClockwise 回頭方向 ture:時計回り, false:反時計回り
   */
  Rotation(int _targetValue, double _targetSpeed, bool _isClockwise);

  /**
   * @brief 回頭する
   */
  void run();

  /**
   * @brief 回頭する際の事前条件判定をする
   * @param targetValue
   * @param targetSpeed 目標速度
   * @note オーバーライド必須
   */
  virtual bool isMetPrecondition();

  /**
   * @brief 回頭する際の継続条件判定をする　返り値がfalseでモーターが止まる
   * @param targetValue 目標値
   * @param initLeftMileage   回頭を始めた時点での左車輪の走行距離
   * @param initRightMileage  回頭を始めた時点での右車輪の走行距離
   * @param leftSign 左車輪の回転方向
   * @param rightSign 右車輪の回転方向
   * @note オーバーライド必須
   */
  virtual bool isMetPostcondition();

  /**
   * @brief 実行のログを取る
   */
  virtual void logRunning();

 protected:
  int targetValue;          // 回転角度(deg) 0~360 もしくは 指定色
  double targetSpeed;       // 目標速度
  bool isClockwise;         // 回頭方向 ture:時計回り, false:反時計回り
  int leftSign;             // 左車輪の回転方向
  int rightSign;            // 右車輪の回転方向
  double initLeftMileage;   // クラス呼び出し時の左車輪の走行距離
  double initRightMileage;  // クラス呼び出し時の右車輪の走行距離
  class Timer Timer;
};
#endif
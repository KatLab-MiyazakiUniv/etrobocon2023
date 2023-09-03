/**
 * @file   CameraAction.h
 * @brief  ミニフィグ撮影動作
 * @author bizyutyu YKhm20020
 */

#ifndef CAMERAACTION_H
#define CAMERAACTION_H

#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "AngleRotation.h"
#include "DistanceStraight.h"

class CameraAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _isA 撮影対象　true:ミニフィグA false:ミニフィグB
   * @param _isClockwise リアカメラをミニフィグに向けるための回頭方向　true:時計回り,
   * false:反時計回り
   * @param _preTargetAngle 撮影のための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   */
  CameraAction(bool _isA, bool _isClockwise, int _preTargetAngle, int _postTargetAngle);

  /**
   * @brief 撮影動作を行う
   */
  void run() override;

  /**
   * @brief 撮影動作を行う際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  bool isA;  // フラグ確認を行うかの判断に用いる撮影対象(true:ミニフィグA, false:ミニフィグB)
  bool isClockwise;     // リアカメラをミニフィグに向けるための回頭方向
  int preTargetAngle;   // 撮影のための目標角度
  int postTargetAngle;  // 黒線復帰のための目標角度
  const int rotationSpeed = 100;  // 回頭速度
  double targetDistance = 50;     // ミニフィグ接近・黒線復帰のための目標距離
  double targetSpeed = 150;       // ミニフィグ接近・黒線復帰のための目標速度
};
#endif
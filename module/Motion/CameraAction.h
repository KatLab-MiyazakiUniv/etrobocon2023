/**
 * @file   CameraAction.h
 * @brief  ミニフィグ撮影動作
 * @author bizyutyu
 */

#ifndef CAMERAACTION_H
#define CAMERAACTION_H

#include "CompositeMotion.h"
#include "AngleRotation.h"

class CameraAction : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _isClockwise リアカメラをミニフィグに向けるための回頭方向　true:時計回り,
   * false:反時計回り
   * @param _preTargetAngle 撮影のための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   */
  CameraAction(bool _isClockwise, int _preTargetAngle, int _postTargetAngle);

  /**
   * @brief 撮影動作を行う
   */
  void run() override;

  /**
   * @brief 撮影動作を行う際の事前条件判定をする
   */
  bool isMetPrecondition(bool cameraActionSkipFlag);

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  bool isClockwise;     // リアカメラをミニフィグに向けるための回頭方向
  int preTargetAngle;   // 撮影のための目標角度
  int postTargetAngle;  // 黒線復帰のための目標角度
  const int rotationSpeed = 100;  // 回頭速度
};
#endif
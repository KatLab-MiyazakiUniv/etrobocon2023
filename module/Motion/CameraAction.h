/**
 * @file   CameraAction.h
 * @brief  ミニフィグ撮影動作
 * @author bizyutyu
 */

#ifndef CAMERAACTION_H
#define CAMERAACTION_H

#include "UniMotion.h"
#include "AngleRotation.h"

class CameraAction : public UniMotion {
 public:
  /**
   * コンストラクタ
   * @param _isClockwis ミニフィグがある方向　true:車体の右側, false:車体の左側
   * @param _preTargetAngle 撮影のための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   */
  CameraAction(bool _isClockwis, int _preTargetAngle, int _postTargetAngle);

  /**
   * @brief 撮影動作を行う
   */
  void run() override;

  /**
   * @brief 撮影動作を行う際の事前条件判定をする
   */
  bool isMetPrecondition() override;

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  bool isClockwis;                // ミニフィグがある方向
  int preTargetAngle;             // 撮影のための目標角度
  int postTargetAngle;            // 黒線復帰のための目標角度
  const int rotationSpeed = 150;  // 回頭速度
};
#endif
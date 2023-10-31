/**
 * @file   BlockThrowing
 * @brief  ブロック投げ入れ動作
 * @author KakinokiKanta
 */

#ifndef BLOCK_THROWING_H
#define BLOCK_THROWING_H

#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "PwmRotation.h"
#include "ArmMotion.h"
#include "Sleeping.h"

class BlockThrowing : public CompositeMotion {
 public:
  // コンストラクタ
  BlockThrowing();

  /**
   * @brief ブロック投げ入れ動作を行う
   */
  void run() override;

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  const bool isClockwise = true;            // ブロック投げ入れの回頭方向
  const int armTargetAngle = 60;    // アームの目標回転角度
  const int preTargetAngle = 40;          // ブロック投げ入れのための目標回頭角度
  const int postTargetAngle = 38;         // 復帰するための目標回頭角度
  const int armPwm = 40;    // アーム動作のPWM
  const int rotationPwm = 80;  // 回頭PWM
};
#endif
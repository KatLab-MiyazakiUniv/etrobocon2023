/**
 * @file ArmMotion.h
 * @brief アーム動作クラス
 * @author KakinokiKanta
 */

#ifndef ARM_MOTION_H
#define ARM_MOTION_H

#include "CompositeMotion.h"

class ArmMotion : public CompositeMotion {
 public:
  /**
   * コンストラクタ
   * @param _angle アームを上下する角度 0~60
   * @param _pwm PWM値 0~40
   */
  ArmMotion(int _angle, int _pwm);

  /**
   * @brief アームを動かす
   */
  void run() override;

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  int angle;  // 回転角度(deg) -60~60
  int pwm;    // PWM値 0~40
};

#endif
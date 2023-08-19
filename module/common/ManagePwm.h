/**
 * @file ManagePwm.h
 * @brief 走行体の左右車輪のPWMを所持するクラス
 * @author bizyutyu
 */

#ifndef MANAGEPWM_H
#define MANAGEPWM_H

#include <stdio.h>

class ManagePwm {
 public:
  ManagePwm();

   /**
   * @brief 右車輪のPWMを返す
   */
  double returnRightPwm();

  /**
   * @brief 左車輪のPWMを返す
   */
  double returnLeftPwm();

  /**
   * @brief 入力された右車輪のPWMに更新する
   * @param rightPwm 右車輪のPWM
   */
  void updateRightPwm(double inputRightPwm);

  /**
   * @brief 入力された左車輪のPWMに更新する
   * @param leftPwm 左車輪のPWM
   */
  void updateLeftPwm(double inputLeftPwm);

 private:
  double rightPwm;  // 右車輪PWM
  double leftPwm;  // 左車輪PWM
};

#endif
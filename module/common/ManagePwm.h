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
  /**
   * @brief 右車輪のPWMを返す
   * @return 右車輪のPWM
   */
  static double getRightPwm();

  /**
   * @brief 左車輪のPWMを返す
   * @return 左車輪のPWM
   */
  static double getLeftPwm();

  /**
   * @brief 入力された右車輪のPWMに更新する
   * @param _inputRightPwm 入力された右車輪のPWM
   */
  static void updateRightPwm(double _inputRightPwm);

  /**
   * @brief 入力された左車輪のPWMに更新する
   * @param _inputLeftPwm 入力された左車輪のPWM
   */
  static void updateLeftPwm(double _inputLeftPwm);

 private:
  static double manageRightPwm;  // 右車輪PWM
  static double manageLeftPwm;   // 左車輪PWM
};

#endif
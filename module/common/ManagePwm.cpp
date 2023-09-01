/**
 * @file ManagePwm.cpp
 * @brief 動作確認に用いるprintf()関数を所持するクラス
 * @author bizyutyu
 */

#include "ManagePwm.h"

ManagePwm::ManagePwm()
{
  manageRightPwm = 0.0;
  manageLeftPwm = 0.0;
}

double ManagePwm::getRightPwm()
{
  return manageRightPwm;
}

double ManagePwm::getLeftPwm()
{
  return manageLeftPwm;
}

void ManagePwm::updateRightPwm(double _inputRightPwm)
{
  manageRightPwm = _inputRightPwm;
}

void ManagePwm::updateLeftPwm(double _inputLeftPwm)
{
  manageLeftPwm = _inputLeftPwm;
}

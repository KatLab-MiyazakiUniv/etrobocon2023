/**
 * @file ManagePwm.cpp
 * @brief 動作確認に用いるprintf()関数を所持するクラス
 * @author bizyutyu
 */

 #include "ManagePwm.h"

 ManagePwm::ManagePwm () {
    rightPwm = 0.0;
    leftPwm = 0.0;
 }

 double ManagePwm::returnRightPwm(){
   return rightPwm;
 }

 double ManagePwm::returnLeftPwm(){
   return leftPwm;
 }

void ManagePwm::updateRightPwm(double inputRightPwm){
   rightPwm = inputRightPwm;
}


void ManagePwm::updateLeftPwm(double inputLeftPwm){
   leftPwm = inputLeftPwm;
}

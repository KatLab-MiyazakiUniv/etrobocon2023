/**
 * @file   ArmMotion.cpp
 * @brief  アーム動作クラス
 * @author KakinokiKanta
 */

#include "ArmMotion.h"

using namespace std;

ArmMotion::ArmMotion(int _angle, int _pwm) : angle(_angle), pwm(_pwm) {}

void ArmMotion::run()
{
  int initCount = measurer.getArmMotorCount();

  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // pwm値が0以下の場合はwarningを出して終了する
  if(pwm <= 0) {
    snprintf(buf, BUF_SIZE, "The pwm value passed to ArmMotion is %d", pwm);
    logger.logWarning(buf);
    return;
  }

  // pwmが40より大きい場合はwarningを出してpwmを40にする
  if(pwm > 40) {
    snprintf(buf, BUF_SIZE, "The pwm value passed to ArmMotion is %d", pwm);
    logger.logWarning(buf);
    pwm = 40;
  }

  // angleが0以下の場合はwarningを出して終了する
  if(angle <= 0) {
    snprintf(buf, BUF_SIZE, "The angle value passed to ArmMotion is %d", angle);
    logger.logWarning(buf);
    return;
  }

  // angleが60より大きい場合はwarningを出してangleを60にする
  if(angle > 60) {
    snprintf(buf, BUF_SIZE, "The angle value passed to ArmMotion is %d", angle);
    logger.logWarning(buf);
    angle = 60;
  }

  // angleが-60より小さい場合はwarningを出してangleを-60にする
  if(angle < -60) {
    snprintf(buf, BUF_SIZE, "The angle value passed to ArmMotion is %d", angle);
    logger.logWarning(buf);
    angle = -60;
  }

  while(true) {
    int currentCount = measurer.getArmMotorCount();

    if(angle > 0) {
      if(currentCount < initCount - angle) {
        break;
      }
    }else {
      if(currentCount > initCount - angle) {
        break;
      }
    }
    
    controller.setArmMotorPwm(-pwm);
    // 10ミリ秒待機
    controller.sleep();
  }

  //アームモータの停止
  controller.stopArmMotor();
}

void ArmMotion::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run ArmMotion (angle: %d, pwm: %d)", angle, pwm);
  logger.log(buf);
}

/**
 * @file   Straight.cpp
 * @brief  直進動作の抽象クラス
 * @author Negimarge
 */

#include "Straight.h"
using namespace std;

Straight::Straight(double _targetSpeed) : targetSpeed(_targetSpeed) {}

void Straight::run()
{
  // 事前条件判定が真でないときは終了する
  if(isRunPreconditionJudgement() == false) {
    return;
  }

  // 直進前の走行距離
  initialRightMotorCount = Measurer::getRightCount();
  initialLeftMotorCount = Measurer::getLeftCount();
  initialDistance = Mileage::calculateMileage(initialRightMotorCount, initialLeftMotorCount);

  // 直進中の走行距離
  currentRightMotorCount = initialRightMotorCount;
  currentLeftMotorCount = initialLeftMotorCount;
  currentDistance = initialDistance;

  int currentPwm = 0;  // 現在のpwd値

  // 走行距離が目標値に到達するまで繰り返す
  SpeedCalculator SpeedCalculator(targetSpeed);
  while(true) {
    // 終了条件が満たされたときループから抜ける オーバーライド必須
    if(isRunPostconditionJudgement() == true) {
      break;
    }

    // PWM値を目標速度値に合わせる
    currentPwm = SpeedCalculator.calcPwmFromSpeed();

    // モータにPWM値をセット
    Controller::setLeftMotorPwm(currentPwm);
    Controller::setRightMotorPwm(currentPwm);

    // 10ミリ秒待機
    Timer.sleep(10);
  }
  // モータの停止
  Controller::stopMotor();
}

bool Straight::isRunPreconditionJudgement()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];
  // \"target\"をオーバーライド必須
  // 目標速度値が0の場合は終了する
  if(targetSpeed == 0) {
    logger.logWarning("The speed value passed to \"target\" Straight is 0");
    return false;
  }
  // \"target\"をオーバーライド必須
  // pwmの絶対値がMIN_PWMより小さい場合はwarningを出す
  SpeedCalculator SpeedCalculator(targetSpeed);
  int pwm = SpeedCalculator.calcPwmFromSpeed();
  if(abs(pwm) < MIN_PWM) {
    snprintf(buf, BUF_SIZE, "The pwm value passed to \"target\" Straight is %d", pwm);
    logger.logWarning(buf);
  }
  // \"target\"に応じたエラー処理が必須
  // オーバーライド必須
  if(false) {
    // logger.logWarning("");
    return false;
  }

  return true;
}

void Straight::logRunning()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // targetValueと%~のオーバーライド必須
  snprintf(buf, BUF_SIZE, "Run \"targetValue\"Straight (\"targetValue\": , targetSpeed: %f)",
           targetSpeed);
  logger.log(buf);
}

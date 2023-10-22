/**
 * @file   BlockThrowing.cpp
 * @brief  ブロック投げ入れ動作
 * @author KakinokiKanta
 */

#include "BlockThrowing.h"

using namespace std;

BlockThrowing::BlockThrowing(){};

void BlockThrowing::run()
{
  ArmMotion raiseAM(armTargetAngle, armPwm);
  ArmMotion lowerAM(-armTargetAngle, armPwm);
  PwmRotation prePR(preTargetAngle, rotationPwm, isClockwise);
  PwmRotation postPR(postTargetAngle, rotationPwm, !isClockwise);
  Sleeping sl(500);

  // ブロック投げ入れのための回頭をする
  prePR.run();
  sl.run();

  // アームを上げてブロックを投げ入れる
  raiseAM.run();

  // 復帰のための回頭をする
  sl.run();
  postPR.run();

  // アームを下げる
  sl.run();
  lowerAM.run();
}

void BlockThrowing::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run BlockThrowing");
  logger.log(buf);
}
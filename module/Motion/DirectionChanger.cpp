/**
 * @file DirectionChanger.cpp
 * @brief 方向転換クラス
 * @author YKhm20020
 */

#include "DirectionChanger.h"

using namespace std;

DirectionChanger::DirectionChanger(COLOR _targetColor, double _targetDistance, double _targetSpeed,
                                   bool& _isLeftEdge, int _rotateAngle, int _changeAngle)
  : BlockMotion(0.88, 0.92),
    targetColor(_targetColor),
    targetDistance(_targetDistance),
    targetSpeed(_targetSpeed),
    isLeftEdge(_isLeftEdge),
    rotateAngle(_rotateAngle),
    changeAngle(_changeAngle)
{
}

void DirectionChanger::run()
{
  bool isClockwise = false;  // 回転方向
  bool nextEdge = false;     // 方向転換後のエッジ

  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  DistanceStraight ds(targetDistance, targetSpeed);
  ColorStraight cs(targetColor, targetSpeed);

  // 回転方向を判定
  if(rotateAngle >= 0) {
    isClockwise = true;
    AngleRotation rotation(rotateAngle, targetSpeed, isClockwise);
    rotation.run();
  } else {
    isClockwise = false;
    AngleRotation rotation2(rotateAngle, targetSpeed, isClockwise);
    rotation2.run();
  }

  //エッジ切り替え
  switch(changeAngle) {
    case 90:  // 右方向に方向転換する場合
    {
      nextEdge = true;
      EdgeChanging ec(isLeftEdge, nextEdge);
      ec.run();
      break;
    }
    case -90:  // 左方向に方向転換する場合
    {
      nextEdge = false;
      EdgeChanging ec2(isLeftEdge, nextEdge);
      ec2.run();
      break;
    }
    case 180:  // 後ろ方向に方向転換する場合
    case -180: {
      nextEdge = !isLeftEdge;
      EdgeChanging ec3(isLeftEdge, nextEdge);
      ec3.run();
      break;
    }
  }

  // 色サークルに乗るまで直進
  ds.run();

  // 指定色（白または黒）を判定するまで直進
  cs.run();
}

bool DirectionChanger::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to DirectionChanger is 0");
    logger.logWarning(buf);
    return false;
  }

  // rotateAngleが0以下の場合はwarningを出して終了する
  if(rotateAngle <= 0 || rotateAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The rotateAngle value passed to DirectionChanger is %d", rotateAngle);
    logger.logWarning(buf);
    return false;
  }

  // changeAngleが0以下の場合はwarningを出して終了する
  if(changeAngle <= 0 || changeAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The changeAngle value passed to DirectionChanger is %d", changeAngle);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void DirectionChanger::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run DirectionChanger (targetColor: %s, targetDistance: %.2f, targetSpeed: %.2f, "
           "isLeftEdge: %s, rotateAngle: %d, changeAngle: %d)",
           ColorJudge::colorToString(targetColor), targetDistance, targetSpeed, str, rotateAngle,
           changeAngle);
  logger.log(buf);
}
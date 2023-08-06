/**
 * @file DirectionChanger.cpp
 * @brief 方向転換クラス
 * @author YKhm20020
 */

#include "DirectionChanger.h"

using namespace std;

DirectionChanger::DirectionChanger(COLOR _targetColor, double _targetDistance, double _targetSpeed,
                                   int _targetAngle, bool _isClockwise, bool& _isLeftEdge,
                                   bool _nextEdge)
  : BlockMotion(0.88, 0.92),  // 動作時間, 失敗リスク TODO: 測定し直す
    targetColor(_targetColor),
    targetDistance(_targetDistance),
    targetSpeed(_targetSpeed),
    targetAngle(_targetAngle),
    isClockwise(_isClockwise),
    isLeftEdge(_isLeftEdge),
    nextEdge(_nextEdge){};

void DirectionChanger::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  DistanceStraight ds(targetDistance, targetSpeed);
  ColorStraight cs(targetColor, targetSpeed);

  // 回転方向を判定
  if(isClockwise == true) {
    AngleRotation rotation(targetAngle, targetSpeed, isClockwise);
    rotation.run();
  } else {
    AngleRotation rotation2(targetAngle, targetSpeed, isClockwise);
    rotation2.run();
  }

  //エッジ切り替え
  if(targetAngle == 180) {  // 後ろ方向に方向転換する場合
    nextEdge = !isLeftEdge;
    EdgeChanging ec(isLeftEdge, nextEdge);
    ec.run();

  } else if(isClockwise == false) {  // 左方向に方向転換する場合
    nextEdge = false;
    EdgeChanging ec2(isLeftEdge, nextEdge);
    ec2.run();

  } else {  // 右方向に方向転換する場合
    nextEdge = true;
    EdgeChanging ec3(isLeftEdge, nextEdge);
    ec3.run();
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

  // 目標の色がNoneのときwarningを出して終了する
  if(targetColor == COLOR::NONE) {
    logger.logWarning("The targetColor passed to DirectionChanger is NONE");
    return false;
  }

  // targetDistance値が0以下の場合はwarningを出して終了する
  if(targetDistance <= 0.0) {
    snprintf(buf, BUF_SIZE, "The targetDistance value passed to DirectionChanger is %.2f",
             targetDistance);
    logger.logWarning(buf);
    return false;
  }

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to DirectionChanger is 0");
    logger.logWarning(buf);
    return false;
  }

  // targetAngleが0以下の場合はwarningを出して終了する
  if(targetAngle <= 0 || targetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The targetAngle value passed to DirectionChanger is %d", targetAngle);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void DirectionChanger::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* isClockwiseStr = isClockwise ? "true" : "false";
  const char* isLeftEdgeStr = isLeftEdge ? "true" : "false";
  const char* nextEdgeStr = nextEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run DirectionChanger (targetColor: %s, targetDistance: %.2f, targetSpeed: %.2f, "
           "targetAngle: %d, isClockwise: %s, isLeftEdge: %s, nextEdge: %s)",
           ColorJudge::colorToString(targetColor), targetDistance, targetSpeed, targetAngle,
           isClockwiseStr, isLeftEdgeStr, nextEdgeStr);
  logger.log(buf);
}
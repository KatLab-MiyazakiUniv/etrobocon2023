/**
 * @file   ColorStraight.cpp
 * @brief  目標色まで直進するクラス
 * @author Negimarge
 */

#include "ColorStraight.h"
using namespace std;

ColorStraight::ColorStraight(COLOR _targetColor, double _targetSpeed)
  : Straight(_targetSpeed), targetColor(_targetColor)
{
}

bool ColorStraight::isRunPostconditionJudgement()
{
  COLOR color = ColorJudge::getColor(Measurer::getRawColor());
  if(color == targetColor) {
    return true;
  }

  return false;
}

bool ColorStraight::isRunPreconditionJudgement()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];
  // 目標速度値が0の場合は終了する
  if(targetSpeed == 0) {
    logger.logWarning("The targetSpeed value passed to ColorStraight is 0");
    return false;
  }
  // 目標の色がNoneのときwarningを出して終了する
  if(targetColor == COLOR::NONE) {
    logger.logWarning("The targetColor passed to ColorStraight is NONE");
    return false;
  }

  return true;
}

void ColorStraight::logRunning()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run ColorStraight (Color: %s, targetSpeed: %f)",
           ColorJudge::colorToString(targetColor), targetSpeed);
  logger.log(buf);
}

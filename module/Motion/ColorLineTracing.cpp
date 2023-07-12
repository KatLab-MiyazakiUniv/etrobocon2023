/**
 * @file   ColorLineTracing.cpp
 * @brief  指定色ライントレース動作
 * @author YKhm20020 bizyutyu
 */

#include "ColorLineTracing.h"

using namespace std;

ColorLineTracing::ColorLineTracing(COLOR _targetColor, double _targetSpeed, int _targetBrightness,
                                   const PidGain& _gain, bool& _isLeftEdge)
  : LineTracing(_targetSpeed, _targetBrightness, _gain, _isLeftEdge), targetColor(_targetColor){};

bool ColorLineTracing::isMetPrecondition(double targetSpeed)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // targetSpeed値が0の場合はwarningを出して終了する
  if(targetSpeed == 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to ColorLineTracing is 0");
    logger.logWarning(buf);
    return false;
  }

  // 目標の色がNoneのときwarningを出して終了する
  if(targetColor == COLOR::NONE) {
    logger.logWarning("The targetColor passed to ColorLineTracing is NONE");
    return false;
  }

  return true;
}

bool ColorLineTracing::isMetPostcondition()
{
  COLOR currentColor = COLOR::NONE;

  currentColor = ColorJudge::getColor(Measurer::getRawColor());
  if(currentColor == targetColor) {
    colorCount++;
  } else {
    colorCount = 0;
  }
  // 指定された色をJUDGE_COUNT回連続で取得
  if(colorCount >= JUDGE_COUNT) return false;

  return true;
}

void ColorLineTracing::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* str = isLeftEdge ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run ColorLineTracing (targetColor: %s, targetSpeed: %.2f, targetBrightness: %d, "
           "gain: (%.2f,%.2f,%.2f), isLeftEdge: %s)",
           ColorJudge::colorToString(targetColor), targetSpeed, targetBrightness, gain.kp, gain.ki,
           gain.kd, str);
  logger.log(buf);
}
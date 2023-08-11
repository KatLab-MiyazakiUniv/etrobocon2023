/**
 * @file BlockAreaMotion.cpp
 * @brief ブロックエリア内動作の親クラス
 * @author YKhm20020
 */

#include "BlockAreaMotion.h"

using namespace std;

BlockAreaMotion::BlockAreaMotion(double time, double risk)
  : MOTION_TIME(time), FAILURE_RISK(risk){};

// 動作時間の取得
double BlockAreaMotion::getMotionTime(void)
{
  return MOTION_TIME;
}

// 失敗リスクの取得
double BlockAreaMotion::getFailureRisk(void)
{
  return FAILURE_RISK;
}
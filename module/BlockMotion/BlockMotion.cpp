/**
 * @file BlockMotion.cpp
 * @brief ブロックエリア内動作の親クラス
 * @author YKhm20020
 */

#include "BlockMotion.h"

BlockMotion::BlockMotion(double time, double risk) : MOTION_TIME(time), FAILURE_RISK(risk){};

// 動作時間の取得
double BlockMotion::getMotionTime(void)
{
  return MOTION_TIME;
}

// 失敗リスクの取得
double BlockMotion::getFailureRisk(void)
{
  return FAILURE_RISK;
}
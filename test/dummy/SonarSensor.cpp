/**
 * @file SonarSensor.cpp
 * @brief ソナーセンサクラスで用いる関数（ダミー）
 * @author YKhm20020
 */

#include "SonarSensor.h"
using namespace ev3api;

// コンストラクタ
SonarSensor::SonarSensor(ePortS port) : distance(100) {}

// センサーまでの距離を取得
int SonarSensor::getDistance()
{
  // 呼び出すたびにdistanceを1引く（0~100）
  if(distance == 0) {
    distance = 100;
  } else {
    distance--;
  }

  return distance;
}
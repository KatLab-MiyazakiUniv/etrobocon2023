/**
 * @file SonarSensor.cpp
 * @brief ソナーセンサクラスで用いる関数（ダミー）
 * @author YKhm20020
 */

#include "SonarSensor.h"
using namespace ev3api;

// コンストラクタ
SonarSensor::SonarSensor(ePortS port) : distance(3) {}

// センサーまでの距離を取得
int SonarSensor::getDistance()
{
  return distance;
}
/**
 * @file Motor.cpp
 * @brief モータクラスで用いる関数（ダミー）
 * @author YKhm20020
 */

#include "Motor.h"
using namespace ev3api;

// コンストラクタ
Motor::Motor(ePortM _port, bool brake, motor_type_t type) : port(_port) {}

// PORT_B　右モータ
// PORT_C　左モータ
// モータ角位置取得
int Motor::getCount()
{
  if(port == PORT_C) {
    return static_cast<int>(motorCount);
  } else if(port == PORT_B) {
    return static_cast<int>(motorCount);
  } else {
    return static_cast<int>(motorCount);
  }
}

// pwm値設定
void Motor::setPWM(int pwm)
{
  if(port == PORT_C) {
    motorCount += pwm * 0.05;
  } else if(port == PORT_B) {
    motorCount += pwm * 0.05;
  } else {
    motorCount += pwm * 0.05;
  }
}

// 　motorCountのリセット
void Motor::reset()
{
  motorCount = 0;
}

double Motor::motorCount = 0.0;
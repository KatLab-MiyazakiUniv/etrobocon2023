/**
 * @file Motor.cpp
 * @brief モータクラスで用いる関数（ダミー）
 * @author YKhm20020 miyashita64 bizyutyu
 */

#include "Motor.h"
using namespace ev3api;

// コンストラクタ
Motor::Motor(ePortM _port, bool brake, motor_type_t type)
{
  motorCount = 0;
}

// PORT_B　右モータ
// PORT_C　左モータ
// モータ角位置取得
int Motor::getCount()
{
  return static_cast<int>(motorCount);
}

// pwm値設定
void Motor::setPWM(int pwm)
{
  int _pwm = (pwm > 100) ? 100 : (pwm < -100) ? -100 : pwm;
  motorCount += static_cast<double>(_pwm) * 0.05;
}

// motorCountのリセット
void Motor::reset()
{
  motorCount = 0;
}
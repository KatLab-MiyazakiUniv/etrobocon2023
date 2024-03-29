/**
 * @file Measurer.cpp
 * @brief 計測に用いる関数をまとめたラッパークラス
 * @author YKhm20020 miyashita64
 */

#include "Measurer.h"

ev3api::ColorSensor* Measurer::colorSensor = nullptr;
ev3api::SonarSensor* Measurer::sonarSensor = nullptr;
ev3api::Motor* Measurer::rightMotor = nullptr;
ev3api::Motor* Measurer::leftMotor = nullptr;
ev3api::Motor* Measurer::armMotor = nullptr;

// 明るさを取得
// 参考: https://tomari.org/main/java/color/ccal.html
int Measurer::getBrightness()
{
  // RGBモードと光センサモードを併用すると動作が悪くなるためRGBモードで取得する
  // 参考: https://qiita.com/kawanon868/items/5d52eb291c3f71af0419
  rgb_raw_t rgb = getRawColor();
  int brightness = std::max({ rgb.r, rgb.g, rgb.b }) * 100 / 255;  // 明度を取得して0-100に正規化
  return brightness;
}

// RGB値を取得
rgb_raw_t Measurer::getRawColor()
{
  rgb_raw_t rgb;
  colorSensor->getRawColor(rgb);
  return rgb;
}

// 左モータ角位置取得
int Measurer::getLeftCount()
{
  return leftMotor->getCount();
}

// 左モータ角位置更新
void Measurer::setLeftCount(int count)
{
  leftMotor->setCount(count);
}

// 右モータ角位置取得
int Measurer::getRightCount()
{
  return rightMotor->getCount();
}

// 右モータ角位置更新
void Measurer::setRightCount(int count)
{
  rightMotor->setCount(count);
}

// 左右モータ角位置の初期化
void Measurer::resetCount()
{
  Measurer::setLeftCount(0);
  Measurer::setRightCount(0);
}

// アームモータ角位置取得
int Measurer::getArmMotorCount()
{
  return armMotor->getCount();
}

// アームモータ角位置更新
void Measurer::setArmMotorCount(int count)
{
  return armMotor->setCount(count);
}

// アームモータ角位置の初期化
void Measurer::resetArmMotorCount()
{
  return armMotor->setCount(0);
}

// 正面から見て左ボタンの押下状態を取得
bool Measurer::getLeftButton()
{
  return ev3_button_is_pressed(LEFT_BUTTON);
}

// 正面から見て右ボタンの押下状態を取得
bool Measurer::getRightButton()
{
  return ev3_button_is_pressed(RIGHT_BUTTON);
}

// 中央ボタンの押下状態を取得
bool Measurer::getEnterButton()
{
  return ev3_button_is_pressed(ENTER_BUTTON);
}

// 超音波センサからの距離を取得
int Measurer::getForwardDistance()
{
  int distance = sonarSensor->getDistance();

  // センサが認識していない時が-1になる
  if(distance == -1) distance = 1000;

  return distance;
}

// SPIKEの電圧を取得
double Measurer::getVoltage()
{
  return (double)ev3_battery_voltage_mV() / 1000.0;
}
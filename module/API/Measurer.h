/**
 * @file Measurer.h
 * @brief 計測に用いる関数をまとめたラッパークラス
 * @author YKhm20020 miyashita64
 */

#ifndef MEASURER_H
#define MEASURER_H

#include <algorithm>
#include "ev3api.h"
#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Motor.h"

class Measurer {
 public:
  static ev3api::ColorSensor* colorSensor;
  static ev3api::SonarSensor* sonarSensor;
  static ev3api::Motor* rightMotor;
  static ev3api::Motor* leftMotor;
  static ev3api::Motor* armMotor;

  Measurer() = delete;  // 明示的にインスタンス化を禁止

  /**
   * 明るさを取得
   * @return 反射光の強さ(0-100)
   */
  static int getBrightness();

  /**
   * RGB値を取得
   * @return RGB値
   */
  static rgb_raw_t getRawColor();

  /**
   * 左モータ角位置を取得
   * @return 左モータ角位置[deg]
   */
  static int getLeftCount();

  /**
   * 左モータ角位置を更新
   * @param 左モータ角位置[deg]
   */
  static void setLeftCount(int count);

  /**
   * 右モータ角位置を取得
   * @return 右モータ角位置[deg]
   */
  static int getRightCount();

  /**
   * 右モータ角位置を更新
   * @param 右モータ角位置[deg]
   */
  static void setRightCount(int count);

  /**
   * アームモータ角位置を取得
   * @return アームモータ角位置[deg]
   */
  static int getArmMotorCount();

  /**
   * 正面から見て左ボタンの押下状態を取得
   * @return 左ボタンの押下状態 true:押されている, false：押されていない
   */
  static bool getLeftButton();

  /**
   * 正面から見て右ボタンの押下状態を取得
   * @return 右ボタンの押下状態 true:押されている, false：押されていない
   */
  static bool getRightButton();

  /**
   * 中央ボタンの押下状態を取得
   * @return 中央ボタンの押下状態 true:押されている, false：押されていない
   */
  static bool getEnterButton();

  /**
   * 超音波センサからの距離を取得
   * @return 超音波センサからの距離[cm]
   * @note センサが認識していない時は1000を返す
   */
  static int getForwardDistance();

  /**
   * SPIKEの電圧を取得
   * @return SPIKEの電圧[V]
   */
  static double getVoltage();
};

#endif
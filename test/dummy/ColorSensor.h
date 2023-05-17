/**
 * @file ColorSensor.h
 * @brief カラーセンサクラス（ダミー）
 * @author YKhm20020
 */

#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include "Port.h"
#include <stdlib.h>

typedef struct {
  int r, g, b;
} rgb_raw_t;

namespace ev3api {

  // カラーセンサクラス
  class ColorSensor {
   public:
    /**
     * コンストラクタ
     * @param port カラーセンサポート番号
     * @return -
     */
    explicit ColorSensor(ePortS port);

    /**
     * RGB値を取得
     * @param rgb RGB値を代入する変数（参照渡し）
     * @return RGBを保持するクラス
     */
    void getRawColor(rgb_raw_t& rgb);
  };
}  // namespace ev3api

#endif
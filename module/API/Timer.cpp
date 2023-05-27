/**
 * @file Timer.cpp
 * @brief 時間測定に用いる関数をまとめたラッパークラス
 * @author miyashita64 YKhm20020
 */

#include "Timer.h"

// 自タスクスリープ（デフォルトは10ミリ秒）
void Timer::sleep(int milliSec)
{
  // clock.sleep()はマイクロ秒指定なので，単位を合わせて呼び出す
  ev3api::Clock().sleep(milliSec * 1000);
}

int Timer::now()
{
  return ev3api::Clock().now() / 1000;
}

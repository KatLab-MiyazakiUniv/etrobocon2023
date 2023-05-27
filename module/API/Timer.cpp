/**
 * @file Timer.cpp
 * @brief 時間測定に用いる関数をまとめたラッパークラス
 * @author miyashita64 YKhm20020
 */

#include "Timer.h"

Timer::Timer() {}

// 自タスクスリープ（デフォルトは10ミリ秒）
void Timer::sleep(int milliSec)
{
  // clock.sleep()はマイクロ秒指定なので，単位を合わせて呼び出す
  clock.sleep(milliSec * 1000);
}

// 走行時間を測定（ミリ秒）
int Timer::now()
{
  // マイクロ秒をミリ秒になおしてreturn
  return clock.now() / 1000;
}

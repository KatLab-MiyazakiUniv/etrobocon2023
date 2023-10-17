/**
 * @file   Stop.h
 * @brief  モーターストップ
 * @author bizyutyu
 */

#ifndef STOP_H
#define STOP_H

#include "Motion.h"

class Stop : public Motion {
 public:
  /**
   * コンストラクタ
   */
  Stop();

  /**
   * @brief 左右モーターを止める
   */
  void run();

  /**
   * @brief 実行のログを取る
   */
  void logRunning();
};

#endif
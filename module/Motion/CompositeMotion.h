/**
 * @file   CompositeMotion.h
 * @brief  合成動作クラス
 * @author bizyutyu
 */

#ifndef COMPOSITEMOTION_H
#define COMPOSITEMOTION_H

#include "Motion.h"

class CompositeMotion : public Motion {
 public:
  /**
   * コンストラクタ
   */
  CompositeMotion();

  /**
   * @brief 合成動作を行う
   * @note オーバーライド必須
   */
  virtual void run() = 0;

  /**
   * @brief 実行のログを取る
   * @note オーバーライド必須
   */
  virtual void logRunning() = 0;
};
#endif
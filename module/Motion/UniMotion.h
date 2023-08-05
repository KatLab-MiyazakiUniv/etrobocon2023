/**
 * @file   UniMotion.h
 * @brief  統合動作の中間クラス
 * @author bizyutyu
 */

#ifndef UNIMOTION_H
#define UNIMOTION_H

#include "Motion.h"

class UniMotion : public Motion {
 public:
  /**
   * コンストラクタ
   */
  UniMotion();

  /**
   * @brief 統合動作を行う
   * @note オーバーライド必須
   */
  virtual void run() = 0;

  /**
   * @brief 統合動作の事前条件判定をする
   * @note オーバーライド必須
   */
  virtual bool isMetPrecondition() = 0;

  /**
   * @brief 実行のログを取る
   * @note オーバーライド必須
   */
  virtual void logRunning() = 0;
};
#endif
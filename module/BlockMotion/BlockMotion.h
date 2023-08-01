/**
 * @file BlockMotion.h
 * @brief ブロックエリア内動作の親クラス
 * @author YKhm20020
 */

#ifndef BLOCK_MOTION_H
#define BLOCK_MOTION_H

class BlockMotion {
 public:
  // コンストラクタ
  BlockMotion(double time = 0, double risk = 0);

  /**
   * @brief 動作時間を取得する
   * @return 動作時間
   */
  double getMotionTime(void);

  /**
   * @brief 失敗リスクを取得する
   * @return 失敗リスク
   */
  double getFailureRisk(void);

 private:
  double MOTION_TIME;
  double FAILURE_RISK;
};

#endif
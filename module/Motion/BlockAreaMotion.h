/**
 * @file BlockAreaMotion.h
 * @brief ブロックエリア内動作の親クラス
 * @author YKhm20020
 */

#ifndef BLOCK_MOTION_H
#define BLOCK_MOTION_H

#include "CompositeMotion.h"
#include "Pid.h"

class BlockAreaMotion : public CompositeMotion {
 public:
  // コンストラクタ
  BlockAreaMotion(){};
};

#endif
/**
 * @file   AreaMaster.h
 * @brief  エリアを攻略するクラス
 * @author aridome222
 */

#ifndef AREA_MASTER_H
#define AREA_MASTER_H

#include <vector>
#include <stdio.h>
#include <string.h>
#include "MotionParser.h"
#include "Logger.h"

class AreaMaster {
 public:
  /**
   * @brief エリアを走行する
   * @param isLeftCourse コースのLR判定(true:Lコース, false:Rコース)
   * @param isLeftEdge エッジのLR判定(true:左エッジ, false:右エッジ)
   * @param targetBrightness 目標輝度
   */
  void runAreaMaster(const bool isLeftCourse, bool& isLeftEdge, const int targetBrightness);

 private:
  // エリアのコマンドファイルのパス
  const char* AreaMasterLeft = "etrobocon2023/datafiles/AreaMasterLeft.csv";
  const char* AreaMasterRight = "etrobocon2023/datafiles/AreaMasterRight.csv";
};

#endif
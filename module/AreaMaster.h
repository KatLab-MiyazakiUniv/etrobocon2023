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

enum Area { LineTrace, DoubleLoop, BlockDeTresure };

class AreaMaster {
 public:
  /**
   * コンストラクタ
   * @param area エリア名
   * @param isLeftCourse コースのLR判定(true:Lコース, false:Rコース)
   * @param isLeftEdge エッジのLR判定(true:左エッジ, false:右エッジ)
   * @param targetBrightness 目標輝度
   */
  AreaMaster(Area area, bool isLeftCourse, bool& isLeftEdge, int targetBrightness);

  /**
   * @brief エリアを走行する
   * @param area エリア名
   * @param isLeftCourse コースのLR判定(true:Lコース, false:Rコース)
   * @param isLeftEdge エッジのLR判定(true:左エッジ, false:右エッジ)
   * @param targetBrightness 目標輝度
   */
  void run(Area area, bool isLeftCourse, bool& isLeftEdge, int targetBrightness);

 private:
  // 各エリアのコマンドファイルベースパス
  const char* basePath = "etrobocon2023/datafiles/";
  const char* commandFileNames[3] = { "LineTrace", "DoubleLoop", "BlockDeTresure" };
};

#endif
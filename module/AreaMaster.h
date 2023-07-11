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

// エリア名を持つ列挙型変数（LineTrace = 0, DoubleLoop = 1, BlockDeTreasure = 2）
enum Area { LineTrace, DoubleLoop, BlockDeTreasure };

class AreaMaster {
 public:
  /**
   * コンストラクタ
   * @param area エリアの指定(Enum型のArea)
   * @param isLeftCourse コースのLR判定(true:Lコース, false:Rコース)
   * @param isLeftEdge エッジのLR判定(true:左エッジ, false:右エッジ)
   * @param targetBrightness 目標輝度
   */
  AreaMaster(Area area, bool isLeftCourse, bool& isLeftEdge, int targetBrightness);

  /**
   * @brief エリアを走行する
   */
  void run();

 private:
  enum Area area;
  bool isLeftCourse;
  bool isLeftEdge;
  int targetBrightness;

  // 各エリアのコマンドファイルベースパス
  const char* basePath = "etrobocon2023/datafiles/";
  // コマンドファイル名（各エリア名）
  const char* commandFileNames[3] = { "LineTrace", "DoubleLoop", "BlockDeTreasure" };
};

#endif
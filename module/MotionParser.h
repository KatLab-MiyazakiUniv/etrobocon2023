/**
 * @file   MotionParser.h
 * @brief  動作コマンドファイルを解析するクラス
 * @author aridome222
 */

#ifndef MOTION_PARSER_H
#define MOTION_PARSER_H

#include <vector>
#include <stdio.h>
#include <string.h>
#include "Logger.h"
#include "Motion.h"
#include "DistanceLineTracing.h"
#include "ColorLineTracing.h"
#include "DistanceStraight.h"
#include "ColorStraight.h"
#include "AngleRotation.h"
#include "Sleeping.h"
#include "EdgeChanging.h"
#include "InCrossStraight.h"
#include "InCrossLeft.h"
#include "InCrossRight.h"
#include "DirectionChanger.h"
#include "ToCrossMotion.h"
#include "CrossToMid.h"
#include "MidToMid.h"
#include "StringOperator.h"

enum class COMMAND {
  DL,  // 指定距離ライントレース
  CL,  // 指定色ライントレース
  DS,  // 指定距離直進
  CS,  // 指定色直進
  AR,  // 指定角度回頭
  DT,  // 旋回
  EC,  // エッジ切り替え
  SL,  // 自タスクスリープ
  AU,  // アームを上げる
  AD,  // アームを下げる
  XR,  // 角度補正回頭
  IS,  // サークルの交点から交点までの直進
  IL,  // 交点内移動（左折）
  IR,  // 交点内移動（右折）
  DC,  // 方向転換
  TC,  // 交点サークル間移動
  CM,  // サークルの交点から交点へ移動
  MM,  // サークル間直線の中点から別の直線の中点へ移動
  NONE
};

class MotionParser {
 public:
  /**
   * @brief ファイルを解析して動作インスタンスのリストを生成する
   * @param filePath ファイルパス
   * @param targetBrightness 目標輝度
   * @param isLeftEdge エッジのLR判定(true:Lコース, false:Rコース)
   * @return 動作インスタンスリスト
   */
  static std::vector<Motion*> createMotions(const char* filePath, int targetBrightness,
                                            bool& isLeftEdge);

 private:
  MotionParser();  // インスタンス化を禁止する

  /**
   * @brief 文字列を列挙型COMMANDに変換する
   * @param str 文字列のコマンド
   * @return コマンド
   */
  static COMMAND convertCommand(char* str);

  /**
   * @brief 文字列をbool型に変換する
   * @param command 文字列のコマンド
   * @param stringParameter 文字列のパラメータ
   * @return bool値
   */
  static bool convertBool(char* command, char* stringParameter);
};

#endif
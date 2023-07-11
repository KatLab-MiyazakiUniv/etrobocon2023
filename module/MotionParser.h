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
#include "StringOperator.h"

enum class COMMAND {
  DL,  // 指定距離ライントレース
  CL,  // 指定色ライントレース
  DS,  // 指定距離直進
  CS,  // 指定色直進
  AR,  // 指定角度回頭
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
   * @param binaryParameter 文字列のパラメータ
   * @return bool値
   */
  static bool convertBool(char* command, char* binaryParameter);
};

#endif
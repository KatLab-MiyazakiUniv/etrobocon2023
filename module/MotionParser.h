/**
 * @file   MotionParser.h
 * @brief  動作コマンドファイルを解析するクラス
 * @author aridome222 bizyutyu
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
#include "CorrectingRotation.h"
#include "BlockAreaMotion.h"
#include "InCrossStraight.h"
#include "InCrossLeft.h"
#include "InCrossRight.h"
#include "CrossToCross.h"
#include "CrossToMid.h"
#include "StringOperator.h"
#include "CameraAction.h"
#include "PwmRotation.h"
#include "Stop.h"
#include "ArmMotion.h"

enum class COMMAND {
  DL,  // 指定距離ライントレース
  CL,  // 指定色ライントレース
  DS,  // 指定距離直進
  CS,  // 指定色直進
  AR,  // 指定角度回頭
  DT,  // 旋回
  EC,  // エッジ切り替え
  SL,  // 自タスクスリープ
  AM,  // アーム動作
  XR,  // 角度補正回頭
  CA,  // 撮影動作
  IS,  // 交点内移動（直進）
  IL,  // 交点内移動（左折）
  IR,  // 交点内移動（右折）
  CC,  // 交点サークルから交点サークル
  CM,  // 交点サークルから直線の中点
  PR,  // Pwm値指定回頭
  ST,  // 左右モーターストップ
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
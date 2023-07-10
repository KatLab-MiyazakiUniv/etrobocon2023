/**
 * @file   AreaMaster.cpp
 * @brief  エリアを攻略するクラス
 * @author aridome222
 */

#include "AreaMaster.h"

using namespace std;

void AreaMaster::runAreaMaster(const bool isLeftCourse, bool& isLeftEdge,
                               const int targetBrightness)
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  Logger logger;

  // ファイルから受け取る動作リスト
  vector<Motion*> motionList;

  // LとRどちらのパラメータを読み込むかを設定
  const char* filePath = isLeftCourse ? AreaMasterLeft : AreaMasterRight;
  // 動作インスタンスのリストを生成する
  motionList = MotionParser::createMotions(filePath, targetBrightness, isLeftEdge);

  // 動作実行のメッセージログを出す
  snprintf(buf, BUF_SIZE, "\nRun the commands in '%s'\n", filePath);
  logger.logHighlight(buf);

  // 各動作を実行する
  for(const auto& motion : motionList) {
    motion->logRunning();
    motion->run();
  }
}
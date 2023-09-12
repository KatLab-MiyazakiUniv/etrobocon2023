/**
 * @file SystemInfo.cpp
 * @brief 走行システムで統一する情報をまとめたファイル
 * @author miyashita64
 */
#include "SystemInfo.h"

Logger::Logger() {}

void Logger::log(const char* logMessage)
{
  const int BUF_SIZE = 256;
  char message[BUF_SIZE];  // 表示するメッセージ
  snprintf(message, BUF_SIZE, "%s\n", logMessage);
  printf("%s", message);

  strncat(logs, message, sizeof(logs));  // logsとmessageを結合する
}
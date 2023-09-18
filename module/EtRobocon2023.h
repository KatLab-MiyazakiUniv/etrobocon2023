/**
 * @file   EtRobocon2023.h
 * @brief  全体を制御するクラス
 * @author KakinokiKanta miyashita64 bizyutyu
 */

#ifndef ETROBOCON2023_H
#define ETROBOCON2023_H

// ev3api.hを読み込むヘッダは.cppに記述する
#include <signal.h>
#include <unistd.h>

class EtRobocon2023 {
 public:
  static void start();

 private:
  /**
   * @brief ログファイルを生成して終了するシグナルハンドラ
   * @param _ キャッチしたシグナルの値がセットされる(ここでは使用しない)
   */
  static void sigint(int _);

  /**
   * @brief 走行状況をサーバに反映する
   * @param state 走行状況
   */
  static void setState(char* state);
};

#endif

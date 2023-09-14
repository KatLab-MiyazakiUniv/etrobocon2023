/**
 * @file SystemInfo.h
 * @brief 走行システムで統一する情報をまとめたファイル
 * @author desty505 bizyutyu
 */

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

// ロボコンの部屋で取得した輝度
static constexpr int BLACK_BRIGHTNESS = 3;   // 黒の輝度
static constexpr int WHITE_BRIGHTNESS = 93;  // 白の輝度

static constexpr double RADIUS = 50.0;  // 車輪の半径[mm]
static constexpr double TREAD = 125.0;  // 走行体のトレッド幅（両輪の間の距離）[mm]

// 撮影終了フラグの定義
#pragma GCC diagnostic push  // 未使用の変数に対する警告への一時的な対処
#pragma GCC diagnostic ignored "-Wunused-variable"  // 未使用の変数に対する警告を無視
static bool cameraActionSkipFlag = false;           // 撮影終了フラグ
static int shootCountB = 0;
#pragma GCC diagnostic pop

static constexpr int ANGLE_SERVER_PORT = 10338;  // 角度算出用サーバのポート番号

#endif
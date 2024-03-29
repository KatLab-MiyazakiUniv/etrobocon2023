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

static constexpr char RAS_PI_IP[16] = "172.20.1.1";

static constexpr int ANGLE_SERVER_PORT = 10338;  // 角度算出用サーバのポート番号

#endif
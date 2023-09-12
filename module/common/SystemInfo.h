/**
 * @file SystemInfo.h
 * @brief 走行システムで統一する情報をまとめたファイル
 * @author desty505 bizyutyu miyashita64
 */

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


// ロボコンの部屋で取得した輝度
static constexpr int BLACK_BRIGHTNESS = 3;   // 黒の輝度
static constexpr int WHITE_BRIGHTNESS = 93;  // 白の輝度

static constexpr double RADIUS = 50.0;  // 車輪の半径[mm]
static constexpr double TREAD = 125.0;  // 走行体のトレッド幅（両輪の間の距離）[mm]

static constexpr int ANGLE_SERVER_PORT = 10338;  // 角度算出用サーバのポート番号

// 撮影終了フラグの定義
// #pragma GCC diagnostic push  // 未使用の変数に対する警告への一時的な対処
// #pragma GCC diagnostic ignored "-Wunused-variable"  // 未使用の変数に対する警告を無視
// static bool cameraActionSkipFlag = false;           // 撮影終了フラグ
// #pragma GCC diagnostic pop

struct SystemInfo {
    char state[256];
    bool skip_camera_action;
};

SystemInfo* initSystemInfo(){
    int shm_fd = shm_open("system_info_memory", O_RDWR, 00777);
    if (shm_fd == -1) {
        perror("shm_open");
        return nullptr;
    }

    // 共有メモリの権限を設定（読み取りと書き込み可能）
    if (fchmod(shm_fd, 00777) == -1) {
        perror("fchmod");
        return nullptr;
    }

    // 共有メモリをマップ
    SystemInfo* shm_data = (struct SystemInfo*)mmap(0, sizeof(struct SystemInfo), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_data == MAP_FAILED) {
        perror("mmap");
        return nullptr;
    }
    return shm_data;
}

void deleteSysteminfo(){
    // 共有メモリのアンマップとクローズ
    munmap(shm_data, sizeof(struct SystemInfo));
    close(shm_fd);
}

static const SystemInfo* shm_data = initSystemInfo();

#endif
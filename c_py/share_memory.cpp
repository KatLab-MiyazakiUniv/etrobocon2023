/**
 * g++ share_memory.cpp -o sm -lrt
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

// C言語の構造体に相当するデータ型を定義
struct SystemInfo {
    char state[256];
    bool skip_camera_action;
};

int main() {
    // 共有メモリセグメントをオープン
    int shm_fd = shm_open("system_info_memory", O_RDWR, 00777);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 共有メモリの権限を設定（読み取りと書き込み可能）
    if (fchmod(shm_fd, 00777) == -1) {
        perror("fchmod");
        return 1;
    }

    // 共有メモリをマップ
    struct SystemInfo* shm_data = (struct SystemInfo*)mmap(0, sizeof(struct SystemInfo), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 共有メモリからデータを取得して表示
    printf("State: %s\n", shm_data->state);
    printf("Skip Camera Action: %s\n", shm_data->skip_camera_action ? "True" : "False");
    // 共有メモリのデータを上書き
    strcpy(shm_data->state, "lap");

    // 共有メモリのアンマップとクローズ
    munmap(shm_data, sizeof(struct SystemInfo));
    close(shm_fd);

    return 0;
}

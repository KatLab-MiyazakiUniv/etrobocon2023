/**
 * g++ share_memory.cpp -o sm -lrt
*/

#include <stdio.h>
#include <stdbool.h>
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
    int shm_fd = shm_open("system_info_memory", O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 共有メモリをマップ
    struct SystemInfo* shm_data = (struct SystemInfo*)mmap(0, sizeof(struct SystemInfo), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 共有メモリからデータを取得して表示
    // while (1) {
        printf("State: %s\n", shm_data->state);
        printf("Skip Camera Action: %s\n", shm_data->skip_camera_action ? "True" : "False");
    //     sleep(1); // インターバルを設定して繰り返し表示
    // }
    // strcpy(shm_data->state, "lap");

    // 共有メモリのアンマップとクローズ
    munmap(shm_data, sizeof(struct SystemInfo));
    close(shm_fd);

    return 0;
}

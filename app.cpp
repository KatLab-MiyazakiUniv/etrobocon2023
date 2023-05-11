/**
 * @file app.cpp
 * @brief タスクを管理するクラス
 * @author miyashita64
 */

#include "EtRobocon2023.h"
#include "app.h"

// メインタスク
void main_task(intptr_t unused)
{
  EtRobocon2023::start();
  ext_tsk();
}

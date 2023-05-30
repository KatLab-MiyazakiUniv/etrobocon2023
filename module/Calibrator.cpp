/**
 * @file Calibrator.cpp
 * @brief キャリブレーションからスタートまでを担当するクラス
 * @author aridome222
 */

#include "Calibrator.h"

Calibrator::Calibrator() : isLeftCourse(true), targetBrightness(50) {}

void Calibrator::run()
{
  // 左右ボタンでコースのLRを選択する
  selectCourse();

  // 黒と白の輝度を測定して目標輝度を求める
  measureTargetBrightness();
}

void Calibrator::selectCourse()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  bool _isLeftCourse = true;

  printf("Select a Course");     // TODO logger.hを作成したら、printfをlogger.logに変更
  printf(">> Set Left Course");  // TODO logger.hを作成したら、printfをlogger.logに変更
  // 中央ボタンが押されたら確定する
  while(!measurer.getEnterButton()) {
    // 左ボタンが押されたらLコースをセットする
    if(measurer.getLeftButton() && !_isLeftCourse) {
      _isLeftCourse = true;
      printf(">> Set Left Course");  // TODO logger.hを作成したら、printfをlogger.logに変更
    }

    // 右ボタンが押されたらRコースをセットする
    if(measurer.getRightButton() && _isLeftCourse) {
      _isLeftCourse = false;
      printf(">> Set Right Course");  // TODO logger.hを作成したら、printfをlogger.logに変更
    }

    timer.sleep();  // 10ミリ秒スリープ
  }

  isLeftCourse = _isLeftCourse;
  const char* course = isLeftCourse ? "Left" : "Right";
  snprintf(buf, BUF_SIZE, "\nWill Run on the %s Course\n", course);
  printf("%s", buf);  // TODO logger.hを作成したら、printfをlogger.logHighlightに変更

  timer.sleep(1000);  // 1秒スリープ
}

void Calibrator::measureTargetBrightness()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // ライン上で中央ボタンを押して、黒と白の中間色の輝度を取得する
  printf("Press the Center Button on the Line");  // TODO
                                                  // logger.hを作成したら、printfをlogger.logに変更
  // 中央ボタンが押されるまで待機
  while(!measurer.getEnterButton()) {
    timer.sleep();  // 10ミリ秒スリープ
  }
  targetBrightness = measurer.getBrightness();
  snprintf(buf, BUF_SIZE, ">> Target Brightness Value is %d", targetBrightness);
  printf("%s", buf);  // TODO logger.hを作成したら、printfをlogger.logに変更
}

void Calibrator::waitForStart()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];               // log用にメッセージを一時保持する領域
  constexpr int startDistance = 5;  // 手などでスタート合図を出す距離[cm]

  printf("On standby.\n");  // TODO logger.hを作成したら、printfをlogger.logに変更
  snprintf(buf, BUF_SIZE, "On standby.\n\nSignal within %dcm from Sonar Sensor.", startDistance);
  printf("%s", buf);  // TODO logger.hを作成したら、printfをlogger.logに変更

  // startDistance以内の距離に物体がない間待機する
  while(measurer.getForwardDistance() > startDistance) {
    timer.sleep();  // 10ミリ秒スリープ
  }
}

bool Calibrator::getIsLeftCourse()
{
  return isLeftCourse;
}

int Calibrator::getTargetBrightness()
{
  return targetBrightness;
}
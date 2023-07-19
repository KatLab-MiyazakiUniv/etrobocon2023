/**
 * @file Calibrator.cpp
 * @brief キャリブレーションからスタートまでを担当するクラス
 * @author aridome222 KakinokiKanta
 */

#include "Calibrator.h"

Calibrator::Calibrator() : isLeftCourse(true), targetBrightness(50) {}

void Calibrator::run()
{
  // 左右ボタンでコースのLRを選択する
  selectCourse();

  // 目標輝度を測定する
  measureTargetBrightness();
}

void Calibrator::selectCourse()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  Logger logger;
  bool _isLeftCourse = true;

  logger.log("Select a Course");
  logger.log(">> Set Left Course");
  // 右ボタンが押されたら確定する
  while(!Measurer::getRightButton()) {
    if(Measurer::getLeftButton() && !_isLeftCourse) {
      // 左ボタンが押されたときRコースがセットされていれば、Lコースをセットする
      _isLeftCourse = true;
      logger.log(">> Set Left Course");
      timer.sleep(500);  // 500ミリ秒スリープ
    } else if(Measurer::getLeftButton() && _isLeftCourse) {
      // 左ボタンが押されたときLコースがセットされていれば、Rコースをセットする
      _isLeftCourse = false;
      logger.log(">> Set Right Course");
      timer.sleep(500);  // 500ミリ秒スリープ
    } else {
      timer.sleep();  // 10ミリ秒スリープ
    }
  }

  isLeftCourse = _isLeftCourse;
  const char* course = isLeftCourse ? "Left" : "Right";
  snprintf(buf, BUF_SIZE, "\nWill Run on the %s Course\n", course);
  logger.logHighlight(buf);

  timer.sleep(1000);  // 1秒スリープ
}

void Calibrator::measureTargetBrightness()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  Logger logger;
  int whiteBrightness = -1;
  int blackBrightness = -1;

  // ライン上で右ボタンを押して、黒と白の中間色の輝度を取得する
  logger.log("Press the Right Button on the Line");
  targetBrightness = -1;

  // 黒
  // 左ボタンで輝度を取得し、右ボタンで待機状態に入る
  while(blackBrightness < 0 || !Measurer::getRightButton()) {
    // 左ボタンが押されるまで待機
    while(blackBrightness < 0 && !Measurer::getLeftButton()) {
      timer.sleep();  // 10ミリ秒スリープ
    }

    // 輝度取得
    blackBrightness = Measurer::getBrightness();
    snprintf(buf, BUF_SIZE, ">> Black Brightness Value is %d", blackBrightness);
    logger.log(buf);
    timer.sleep();  // 10ミリ秒スリープ
  }

  // 白
    // 左ボタンで輝度を取得し、右ボタンで待機状態に入る
  while(whiteBrightness < 0 || !Measurer::getRightButton()) {
    // 左ボタンが押されるまで待機
    while(whiteBrightness < 0 && !Measurer::getLeftButton()) {
      timer.sleep();  // 10ミリ秒スリープ
    }

    // 輝度取得
    whiteBrightness = Measurer::getBrightness();
    snprintf(buf, BUF_SIZE, ">> White Brightness Value is %d", whiteBrightness);
    logger.log(buf);
    timer.sleep();  // 10ミリ秒スリープ
  }

  targetBrightness = (whiteBrightness + blackBrightness) / 2;
  snprintf(buf, BUF_SIZE, ">> Target Brightness Value is %d", targetBrightness);
  logger.log(buf);



}

void Calibrator::waitForStart()
{
  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  Logger logger;
  constexpr int startDistance = 5;  // 手などでスタート合図を出す距離[cm]

  logger.log("On standby.\n");
  snprintf(buf, BUF_SIZE, "On standby.\n\nSignal within %dcm from Sonar Sensor.", startDistance);
  logger.log(buf);

  // startDistance以内の距離に物体がない間待機する
  while(Measurer::getForwardDistance() > startDistance) {
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
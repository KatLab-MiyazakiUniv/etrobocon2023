/**
 * @file   CameraAction.cpp
 * @brief  ミニフィグ撮影動作
 * @author bizyutyu YKhm20020 kawanoichi miyashita64
 */

#include "CameraAction.h"

using namespace std;

// countShootAの初期化
int CameraAction::countShootA = 0;

CameraAction::CameraAction(bool _isA, bool _isClockwise, int _preTargetAngle, int _postTargetAngle)
  : isA(_isA),
    isClockwise(_isClockwise),
    preTargetAngle(_preTargetAngle),
    postTargetAngle(_postTargetAngle){};

void CameraAction::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  PwmRotation prePR(preTargetAngle, rotationPwm, isClockwise);
  PwmRotation postPR(postTargetAngle, rotationPwm, !isClockwise);
  Sleeping sl(500);

  // 撮影のための回頭をする
  if(preTargetAngle != 0) {
    prePR.run();
    sl.run();
  }

  // 撮影対象がBの場合は、前進でフィグから遠ざかる
  if(isA == false) {
    DistanceStraight dsToFig(targetDistance, targetSpeed);
    dsToFig.run();
  }

  // リアカメラで画像を取得する
  // 画像のファイル名を指定
  if(isA) {
    countShootA++;
    sprintf(imageName, "FigA_%d.png", countShootA);
  } else {
    sprintf(imageName, "FigB.png");
  }

  // 撮影に際してディレクトリ移動も行う
  char cmd[256];
  snprintf(cmd, 256, "cd etrobocon2023/rear_camera_py && make snap_image SAVE_NAME=%s && cd ../..",
           imageName);
  system(cmd);

  printf("%s\n", cmd);

  // 撮影対象がBの場合は、バックで黒線へ復帰
  if(isA == false) {
    DistanceStraight dsToLine((targetDistance - 25), -1.0 * targetSpeed);
    dsToLine.run();
  }

  // 黒線復帰のための回頭をする
  if(postTargetAngle != 0) {
    sl.run();
    postPR.run();
  }
}

bool CameraAction::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // preTargetAngleが0未満または360以上の場合はwarningを出して終了する
  if(preTargetAngle < 0 || preTargetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The preTargetAngle value passed to preRotation is %d", preTargetAngle);
    logger.logWarning(buf);
    return false;
  } else if(preTargetAngle == 0) {
    snprintf(buf, BUF_SIZE, "The preTargetAngle value passed to preRotation is %d", preTargetAngle);
    logger.logHighlight(buf);
    return true;
  }

  // postTargetAngleが0未満または360以上の場合はwarningを出して終了する
  if(postTargetAngle < 0 || postTargetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The postTargetAngle value passed to postRotation is %d",
             postTargetAngle);
    logger.logWarning(buf);
    return false;
  } else if(postTargetAngle == 0) {
    snprintf(buf, BUF_SIZE, "The postTargetAngle value passed to postRotation is %d",
             postTargetAngle);
    logger.logHighlight(buf);
    return true;
  }

  // 撮影対象がAの場合はフラグ確認を行う
  if(isA == true) {
    // フラグファイルの存在確認(ファイルはWebサーバが生成する)
    FILE* fp = fopen(SKIP_FLAG_PATH, "r");
    // ファイルが存在する場合は、撮影をスキップする
    if(fp != NULL) {
      snprintf(buf, BUF_SIZE, "The skip flag is exist as a file %s.", SKIP_FLAG_PATH);
      logger.logHighlight(buf);
      fclose(fp);
      return false;
    }
  }

  return true;
}

void CameraAction::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  const char* isClockwisestr = isClockwise ? "true" : "false";

  snprintf(buf, BUF_SIZE,
           "Run CameraAction (isClockwise: %s, preTargetAngle: %d, postTargetAngle: %d)",
           isClockwisestr, preTargetAngle, postTargetAngle);
  logger.log(buf);
}
/**
 * @file   CameraAction.cpp
 * @brief  ミニフィグ撮影動作
 * @author bizyutyu
 */

#include "CameraAction.h"

using namespace std;

CameraAction::CameraAction(bool _isClockwis, int _preTargetAngle, int _postTargetAngle)
  : isClockwis(_isClockwis), preTargetAngle(_preTargetAngle), postTargetAngle(_postTargetAngle){};

void CameraAction::run()
{
  bool cameraActionSkipFlag = false;

  // 事前条件を判定する
  if(!isMetPrecondition(cameraActionSkipFlag)) {
    return;
  }

  AngleRotation preAR(preTargetAngle, rotationSpeed, isClockwis);
  AngleRotation postAR(postTargetAngle, rotationSpeed, !isClockwis);

  // 撮影のための回頭をする
  preAR.run();

  // リアカメラで画像を取得する
  // 撮影に際してディレクトリ移動も行う
  char cmd[256];
  snprintf(cmd, 256, "cd etrobocon2023/rear_camera_py && make image && cd ../..");
  system(cmd);

  // 黒線復帰のための回頭をする
  postAR.run();
}

bool CameraAction::isMetPrecondition(bool cameraActionSkipFlag)
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];

  // preTargetAngleが0以下の場合はwarningを出して終了する
  if(preTargetAngle <= 0 || preTargetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The preTargetAngle value passed to preRotation is %d", preTargetAngle);
    logger.logWarning(buf);
    return false;
  }

  // postTargetAngleが0以下の場合はwarningを出して終了する
  if(postTargetAngle <= 0 || postTargetAngle >= 360) {
    snprintf(buf, BUF_SIZE, "The postTargetAngle value passed to postRotation is %d",
             postTargetAngle);
    logger.logWarning(buf);
    return false;
  }

  // 撮影終了フラグがtrueの場合は撮影動作をスキップする
  if(cameraActionSkipFlag == true) {
    snprintf(buf, BUF_SIZE, "The value of cameraActionSkipFlag is true.");
    logger.logHighlight(buf);
    return false;
  }
  return true;
}

void CameraAction::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run CameraAction (isClockwis, preTargetAngle, postTargetAngle)",
           isClockwis, preTargetAngle, postTargetAngle);
  logger.log(buf);
}
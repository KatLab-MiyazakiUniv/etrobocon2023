/**
 * @file   CorrectingRotation.h
 * @brief  角度補正回頭
 * @author aridome222
 */

#include "CorrectingRotation.h"

using namespace std;

CorrectingRotation::CorrectingRotation(int _targetAngle, double _targetSpeed)
  : targetAngle(_targetAngle), targetSpeed(_targetSpeed){};

void CorrectingRotation::run()
{
  // 事前条件を判定する
  if(!isMetPrecondition()) {
    return;
  }

  /*
   * rear_camera_request.shより直線の角度を取得
   * シミュレータ環境ではpopenが使えないため，
   * ファイルに出力して読み込む
   */
  FILE* fp = nullptr;
  char output[8];  // rear_camera_request.shの出力結果を保持する領域
  char cmd[1024];
  snprintf(cmd, 1024, "bash ./etrobocon2023/scripts/rear_camera_request.sh %d > response.txt",
           ANGLE_SERVER_PORT);
  system(cmd);
  if((fp = fopen("response.txt", "r")) == NULL) {
    // コマンドを実行できなかった場合Warningを出して終了する
    logger.logWarning("Could not open \"./response.txt\"");
    return;
  }
  // 実行結果をoutputにセット
  fgets(output, sizeof(output), fp);
  fclose(fp);

  // outputから改行を削除
  strcpy(output, StringOperator::removeEOL(output));

  // 文字列がNoneの場合は黒線を認識できていないので，Warningを出して終了する
  if(strcmp(output, "None") == 0) {
    logger.logWarning("Could not recognize black line");
    return;
  }

  double measuredAngle = atof(output);  // outputをdouble型に変換
  int tAngle = targetAngle;

  /*
   * 目標角度0の時，縦線を取得できなかった場合は横線を採用する
   * 計測角度(絶対値)が目標角度より45度より大きい場合，横線を採用したと判定する
   * 目標角度を計測角度の方向に90度ずらして再設定する
   */
  if(abs(tAngle - measuredAngle) > 45) {
    if(tAngle < measuredAngle)
      tAngle += 90;
    else
      tAngle -= 90;
  }

  // 計測角度と目標角度の符号を揃える
  if((measuredAngle > 0) ^ (tAngle > 0)) tAngle *= -1;  // 符号が不一致の場合

  // 補正角度を計算する
  double correctionAngle = tAngle - measuredAngle;

  // 補正角度が補正免除角度以下であれば角度補正は行わない
  if(abs(correctionAngle) <= NO_CORRECTION_ANGLE) return;

  // 回頭のパラメータを決定する
  int angle = abs(correctionAngle);  // 補正角度の小数点以下は切り捨てる
  bool isClockwise = correctionAngle > 0 ? true : false;

  // 補正角度をログに出力
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  snprintf(buf, BUF_SIZE, "%d degree angle correction", (int)correctionAngle);
  logger.log(buf);

  // 算出された補正角度だけ回頭する
  AngleRotation angleRotation(angle, targetSpeed, isClockwise);
  angleRotation.run();
}

bool CorrectingRotation::isMetPrecondition()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // 目標速度が0.0以下の場合はwarningを出して終了する
  if(targetSpeed <= 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to CorrectingRotation is %lf",
             targetSpeed);
    logger.logWarning(buf);
    return false;
  }

  // 目標角度が0未満or90以上の場合はwarningを出して終了する
  if(targetAngle < 0 || targetAngle >= 90) {
    snprintf(buf, BUF_SIZE, "The targetAngle value passed to CorrectingRotation is %d",
             targetAngle);
    logger.logWarning(buf);
    return false;
  }

  return true;
}

void CorrectingRotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run CorrectingRotation (targetAngle: %d, targetSpeed: %lf)", targetAngle,
           targetSpeed);
  logger.log(buf);
}
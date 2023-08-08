/**
 * @file   CorrectingRotation.h
 * @brief  角度補正回頭
 * @author aridome222
 */

#include "CorrectingRotation.h"

using namespace std;

CorrectingRotation::CorrectingRotation(int _targetAngle, double _targetSpeed)
  : targetAngle(_targetAngle), targetSpeed(_targetSpeed)
{
}

void CorrectingRotation::run()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  // 目標速度が0.0以下の場合はwarningを出して終了する
  if(targetSpeed <= 0.0) {
    snprintf(buf, BUF_SIZE, "The targetSpeed value passed to CorrectingRotation is %lf",
             targetSpeed);
    logger.logWarning(buf);
    return;
  }

  // 目標角度が0未満or90以上の場合はwarningを出して終了する
  if(targetAngle < 0 || targetAngle >= 90) {
    snprintf(buf, BUF_SIZE, "The targetAngle value passed to CorrectingRotation is %d",
             targetAngle);
    logger.logWarning(buf);
    return;
  }

  /*
   * rear_camera.shより直線の角度を取得
   * シミュレータ環境ではpopenが使えないため，
   * ファイルに出力して読み込む
   */
  FILE* fp = fopen("./etrobocon2023/rear_camera_py/result.txt", "r");
  char output[8];  // rear_camera.shの出力結果を保持する領域
  char cmd[1024];
  snprintf(
      cmd, 1024,
      "bash ./etrobocon2023/scripts/rear_camera.sh %d > ./etrobocon2023/rear_camera_py/result.txt",
      ANGLE_SERVER_PORT);
  system(cmd);
  if((fp = fopen("./etrobocon2023/rear_camera_py/result.txt", "r")) == NULL) {
    // コマンドを実行できなかった場合Warningを出して終了する
    logger.logWarning("Could not open \"./etrobocon2023/rear_camera_py/result.txt\"");
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
  snprintf(buf, BUF_SIZE, "%d degree angle correction", (int)correctionAngle);
  logger.log(buf);

  // 以降はRotation::run()と同様
  // isClockwiseがtrueなら時計回り，falseなら反時計回り
  // isClockwiseは回転方向の係数
  int leftSign = isClockwise ? 1 : -1;
  int rightSign = isClockwise ? -1 : 1;
  double targetDistance
      = M_PI * TREAD * angle / 360;  // 指定した角度に対する目標の走行距離(弧の長さ)

  // 目標距離（呼び出し時の走行距離 ± 指定された回転量に必要な距離）
  double targetLeftDistance
      = Mileage::calculateWheelMileage(Measurer::getLeftCount()) + targetDistance * leftSign;
  double targetRightDistance
      = Mileage::calculateWheelMileage(Measurer::getRightCount()) + targetDistance * rightSign;

  SpeedCalculator speedCalculator(targetSpeed * rightSign, targetSpeed * leftSign);

  // 継続条件を満たしている間ループ
  while(1) {
    // 残りの移動距離を算出
    double diffLeftDistance
        = (targetLeftDistance - Mileage::calculateWheelMileage(Measurer::getLeftCount()))
          * leftSign;
    double diffRightDistance
        = (targetRightDistance - Mileage::calculateWheelMileage(Measurer::getRightCount()))
          * rightSign;

    // 目標距離に到達した場合
    if(diffLeftDistance <= 0 && diffRightDistance <= 0) {
      break;
    }

    // PWM値を設定する
    int leftPwm = speedCalculator.calcLeftPwmFromSpeed();
    int rightPwm = speedCalculator.calcRightPwmFromSpeed();

    // モータにPWM値をセット
    Controller::setLeftMotorPwm(leftPwm);
    Controller::setRightMotorPwm(rightPwm);

    // 10ミリ秒待機
    timer.sleep(10);
  }

  // モータの停止
  Controller::stopMotor();
}

void CorrectingRotation::logRunning()
{
  const int BUF_SIZE = 256;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域

  snprintf(buf, BUF_SIZE, "Run CorrectingRotation (targetAngle: %d, targetSpeed: %lf)", targetAngle,
           targetSpeed);
  logger.log(buf);
}
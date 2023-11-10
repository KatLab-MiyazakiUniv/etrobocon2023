/**
 * @file   CameraAction.h
 * @brief  ミニフィグ撮影動作
 * @author bizyutyu YKhm20020 kawanoichi miyashita64
 */

#ifndef CAMERAACTION_H
#define CAMERAACTION_H

#include "SystemInfo.h"
#include "CompositeMotion.h"
#include "PwmRotation.h"
#include "DistanceStraight.h"
#include "Sleeping.h"

class CameraAction : public CompositeMotion {
 public:
  /**
   * 撮影対象判別の為の型
   * @param subject A:ミニフィグA B:ミニフィグB BLOCK_AREA:ブロックエリア
   */
  enum class Subject { A, B, BLOCK_AREA, UNDEFINED };

  /**
   * コンストラクタ
   * @param _subject 撮影対象　A:ミニフィグA B:ミニフィグB BLOCK_AREA:ブロックエリア
   * @param _isClockwise リアカメラをミニフィグに向けるための回頭方向　true:時計回り,
   * false:反時計回り
   * @param _preTargetAngle 撮影のための回頭角度
   * @param _postTargetAngle 黒線復帰のための回頭角度
   */
  CameraAction(Subject _subject, bool _isClockwise, int _preTargetAngle, int _postTargetAngle);

  /**
   * @brief 撮影動作を行う
   */
  void run() override;

  /**
   * @brief 撮影動作を行う際の事前条件判定をする
   */
  bool isMetPrecondition();

  /**
   * @brief 実行のログを取る
   */
  void logRunning() override;

 private:
  static constexpr char* SKIP_FLAG_PATH = "etrobocon2023/server/skip_camera_action.flag";
  Subject subject;  // フラグ確認を行うかの判断に用いる撮影対象(A:ミニフィグA, B:ミニフィグB,
                    // BLOCK_AREA:ブロックエリア)
  bool isClockwise;            // リアカメラをミニフィグに向けるための回頭方向
  int preTargetAngle;          // 撮影のための目標角度
  int postTargetAngle;         // 黒線復帰のための目標角度
  const int rotationPwm = 60;  // 回頭PWM
  const double targetDistance = 50;  // 撮影前後の前進・後退のための目標距離
  const double targetSpeed = 150;    // 撮影前後の前進・後退のための目標速度
  static int countShootA;            // Aでの撮影回数をカウント
};
#endif
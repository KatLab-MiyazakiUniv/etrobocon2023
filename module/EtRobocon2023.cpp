/**
 * @file   EtRobocon2023.cpp
 * @brief  全体を制御するクラス
 * @author KakinokiKanta miyashita64 bizyutyu aridome222
 */

#include "EtRobocon2023.h"
// ev3api.hをインクルードしているものは.cppに書く
#include "AreaMaster.h"
#include "Measurer.h"
#include "Controller.h"
#include "Calibrator.h"
#include "ev3api.h"
#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "Timer.h"

void EtRobocon2023::start()
{
  const ePortS colorSensorPort = PORT_2;
  const ePortS sonarSensorPort = PORT_3;
  const ePortM armMotorPort = PORT_A;
  const ePortM rightMotorPort = PORT_B;
  const ePortM leftMotorPort = PORT_C;

  ev3api::ColorSensor* _colorSensorPtr = new ev3api::ColorSensor(colorSensorPort);
  ev3api::SonarSensor* _sonarSensorPtr = new ev3api::SonarSensor(sonarSensorPort);
  ev3api::Motor* _rightMotorPtr = new ev3api::Motor(rightMotorPort);
  ev3api::Motor* _leftMotorPtr = new ev3api::Motor(leftMotorPort);
  ev3api::Motor* _armMotorPtr = new ev3api::Motor(armMotorPort);
  ev3api::Clock* _clockPtr = new ev3api::Clock();

  Controller::rightMotor = _rightMotorPtr;
  Controller::leftMotor = _leftMotorPtr;
  Controller::armMotor = _armMotorPtr;
  Measurer::colorSensor = _colorSensorPtr;
  Measurer::sonarSensor = _sonarSensorPtr;
  Measurer::rightMotor = _rightMotorPtr;
  Measurer::leftMotor = _leftMotorPtr;
  Measurer::armMotor = _armMotorPtr;
  Timer::clock = _clockPtr;

  const int BUF_SIZE = 128;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  Logger logger;

  bool isLeftCourse = false;
  bool isLeftEdge = false;
  int targetBrightness = (WHITE_BRIGHTNESS + BLACK_BRIGHTNESS) / 2;
  Calibrator calibrator;

  // 強制終了(CTRL+C)のシグナルを登録する
  signal(SIGINT, sigint);

  // キャリブレーションする
  calibrator.run();
  isLeftCourse = calibrator.getIsLeftCourse();
  isLeftEdge = isLeftCourse;
  targetBrightness = calibrator.getTargetBrightness();

  // 合図を送るまで待機する
  calibrator.waitForStart();

  // スタートのメッセージログを出す
  const char* course = isLeftCourse ? "Left" : "Right";
  snprintf(buf, BUF_SIZE, "\nRun on the %s Course\n", course);
  logger.logHighlight(buf);

  // 各エリアを走行する
  AreaMaster lineTraceAreaMaster(Area::LineTrace, isLeftCourse, isLeftEdge, targetBrightness);
  AreaMaster doubleLoopAreaMaster(Area::DoubleLoop, isLeftCourse, isLeftEdge, targetBrightness);
  AreaMaster blockDeTreasureAreaMaster(Area::BlockDeTreasure, isLeftCourse, isLeftEdge,
                                       targetBrightness);
  lineTraceAreaMaster.run();
  doubleLoopAreaMaster.run();
  blockDeTreasureAreaMaster.run();

  // 走行終了のメッセージログを出す
  logger.logHighlight("The run has been completed\n");

  // ログファイルを生成する
  logger.outputToFile();
}

void EtRobocon2023::sigint(int _)
{
  Logger logger;
  logger.log("Forced termination.");  // 強制終了のログを出力
  logger.outputToFile();              // ログファイルを生成
  _exit(0);                           // システムコールで強制終了
}

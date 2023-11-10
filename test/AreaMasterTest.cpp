/**
 * @file AreaMasterTest.cpp
 * @brief AreaMasterクラスのテスト
 * @author aridome222
 */

#include "AreaMaster.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <regex>

using namespace std;

namespace etrobocon2023_test {

  TEST(AreaMasterTest, runLineTraceLeftCourse)
  {
    Area area = Area::LineTrace;
    bool isLeftCourse = true;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster(area, isLeftCourse, isLeftEdge, targetBrightness);
    areaMaster.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 回頭補正でrear_camera_pyを呼び出せないことに対するエラーを握りつぶす
    regex pattern(
        R"(Run CorrectingRotation \(targetAngle: [-]?[0-9,.]+, targetSpeed: [-]?[0-9,.]+\)\n\x1B\[36mWarning: The parameter passed to StringOperator::removeEOL is empty)");
    output = regex_replace(output, pattern, "");

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;
    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
    // WarningやErrorが出ていた場合にoutputを出力する
    if(!actual) {
      EXPECT_EQ(output, "");
    }
  }

  TEST(AreaMasterTest, runLineTraceRightCourse)
  {
    Area area = Area::LineTrace;
    bool isLeftCourse = false;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster(area, isLeftCourse, isLeftEdge, targetBrightness);
    areaMaster.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 回頭補正でrear_camera_pyを呼び出せないことに対するエラーを握りつぶす
    regex pattern(
        R"(Run CorrectingRotation \(targetAngle: [-]?[0-9,.]+, targetSpeed: [-]?[0-9,.]+\)\n\x1B\[36mWarning: The parameter passed to StringOperator::removeEOL is empty)");
    output = regex_replace(output, pattern, "");

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;

    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
    // WarningやErrorが出ていた場合にoutputを出力する
    if(!actual) {
      EXPECT_EQ(output, "");
    }
  }

  TEST(AreaMasterTest, runDoubleLoopLeftCourse)
  {
    Area area = Area::DoubleLoop;
    bool isLeftCourse = true;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster(area, isLeftCourse, isLeftEdge, targetBrightness);
    areaMaster.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 回頭補正でrear_camera_pyを呼び出せないことに対するエラーを握りつぶす
    regex pattern(
        R"(Run CorrectingRotation \(targetAngle: [-]?[0-9,.]+, targetSpeed: [-]?[0-9,.]+\)\n\x1B\[36mWarning: The parameter passed to StringOperator::removeEOL is empty)");
    output = regex_replace(output, pattern, "");

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;
    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
    // WarningやErrorが出ていた場合にoutputを出力する
    if(!actual) {
      EXPECT_EQ(output, "");
    }
  }

  TEST(AreaMasterTest, runDoubleLoopRightCourse)
  {
    Area area = Area::DoubleLoop;
    bool isLeftCourse = false;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster(area, isLeftCourse, isLeftEdge, targetBrightness);
    areaMaster.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 回頭補正でrear_camera_pyを呼び出せないことに対するエラーを握りつぶす
    regex pattern(
        R"(Run CorrectingRotation \(targetAngle: [-]?[0-9,.]+, targetSpeed: [-]?[0-9,.]+\)\n\x1B\[36mWarning: The parameter passed to StringOperator::removeEOL is empty)");
    output = regex_replace(output, pattern, "");

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;

    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
    // WarningやErrorが出ていた場合にoutputを出力する
    if(!actual) {
      EXPECT_EQ(output, "");
    }
  }

  TEST(AreaMasterTest, runBlockDeTreasureLeftCourse)
  {
    Area area = Area::BlockDeTreasure;
    bool isLeftCourse = true;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster(area, isLeftCourse, isLeftEdge, targetBrightness);
    areaMaster.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 回頭補正でrear_camera_pyを呼び出せないことに対するエラーを握りつぶす
    regex pattern(
        R"(Run CorrectingRotation \(targetAngle: [-]?[0-9,.]+, targetSpeed: [-]?[0-9,.]+\)\n\x1B\[36mWarning: The parameter passed to StringOperator::removeEOL is empty)");
    output = regex_replace(output, pattern, "");

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;
    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
    // WarningやErrorが出ていた場合にoutputを出力する
    if(!actual) {
      EXPECT_EQ(output, "");
    }
  }

  TEST(AreaMasterTest, runBlockDeTreasureRightCourse)
  {
    Area area = Area::BlockDeTreasure;
    bool isLeftCourse = false;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster(area, isLeftCourse, isLeftEdge, targetBrightness);
    areaMaster.run();
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // 回頭補正でrear_camera_pyを呼び出せないことに対するエラーを握りつぶす
    regex pattern(
        R"(Run CorrectingRotation \(targetAngle: [-]?[0-9,.]+, targetSpeed: [-]?[0-9,.]+\)\n\x1B\[36mWarning: The parameter passed to StringOperator::removeEOL is empty)");
    output = regex_replace(output, pattern, "");

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;

    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
    // WarningやErrorが出ていた場合にoutputを出力する
    if(!actual) {
      EXPECT_EQ(output, "");
    }
  }
}  // namespace etrobocon2023_test
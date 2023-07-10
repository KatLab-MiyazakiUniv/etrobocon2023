/**
 * @file AreaMasterTest.cpp
 * @brief AreaMasterクラスのテスト
 * @author aridome222
 */

#include "AreaMaster.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2023_test {

  TEST(AreaMasterTest, runLeftCourse)
  {
    bool isLeftCourse = true;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster;
    areaMaster.runAreaMaster(isLeftCourse, isLeftEdge, targetBrightness);
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;
    printf("%s", output.c_str());
    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
  }

  TEST(AreaMasterTest, runRightCourse)
  {
    bool isLeftCourse = false;
    bool isLeftEdge = isLeftCourse;
    int targetBrightness = 45;

    testing::internal::CaptureStdout();  // 標準出力キャプチャ開始
    AreaMaster areaMaster;
    areaMaster.runAreaMaster(isLeftCourse, isLeftEdge, targetBrightness);
    string output = testing::internal::GetCapturedStdout();  // キャプチャ終了

    // find("str")はstrが見つからない場合string::nposを返す
    bool actual = output.find("Warning") == string::npos && output.find("Error") == string::npos;

    EXPECT_TRUE(actual);  // WarningやErrorが出ていないかテスト
  }
}  // namespace etrobocon2023_test
/**
 * @file EtRobocon2023Test.cpp
 * @brief EtRobocon2023クラスのテストケース
 * @author KakinokiKanta
 */

#include "../module/EtRobocon2023.h"  // このヘッダファイルのcppファイルをテスト
#include <gtest/gtest.h>

namespace etrobocon2023_test {

  class EtRobocon2023Test : public ::testing::Test {
    friend class EtRobocon2023;

   protected:
    virtual void SetUp() {}
    EtRobocon2023 et;
  };

  TEST_F(EtRobocon2023Test, startTest) {}
}  // namespace etrobocon2023_test
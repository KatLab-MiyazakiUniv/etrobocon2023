/**
 * @file Motor.h
 * @brief モータクラス（ダミー）
 * @author YKhm20020 miyashita64 bizyutyu
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "Port.h"
#include "ev3api.h"

namespace ev3api {

  // モータクラス
  class Motor {
   public:
    /**
     * コンストラクタ
     * @param port  モータポート番号
     * @param brake true:ブレーキモード/false:フロートモード
     * @param type  モータタイプ
     * @return -
     */
    explicit Motor(ePortM port, bool brake = true, motor_type_t type = LARGE_MOTOR);

    /**
     * モータ角位置取得
     * @return モータ角位置[deg]
     */
    int getCount();

    /**
     * モータ角位置更新
     */
    void setCount(int count){};

    /**
     * pwm値設定
     * @param pwm pwm値
     */
    void setPWM(int pwm);

    /**
     * 停止する
     */
    void stop(){};

    /**
     * モータカウントを初期化する
     */
    void reset();

   private:
    double motorCount;
  };
}  // namespace ev3api

#endif
/**
 * @file Pid.cpp
 * @brief PIDを計算するクラス
 * @author Negimarge bizyutyu
 */

#include "Pid.h"

PidGain::PidGain(double _kp, double _ki, double _kd) : kp(_kp), ki(_ki), kd(_kd) {}

Pid::Pid(double _kp, double _ki, double _kd, double _targetValue, double _initDeviation,
         double _timeConstant)
  : gain(_kp, _ki, _kd),
    preDeviation(_initDeviation),
    integral(0.0),
    targetValue(_targetValue),
    timeConstant(_timeConstant)
{
}

void Pid::setPidGain(double _kp, double _ki, double _kd)
{
  gain.kp = _kp;
  gain.ki = _ki;
  gain.kd = _kd;
}

double Pid::calculatePid(double currentValue, double delta)
{
  // delta 周期[ms](デフォルト値0.01[10ms]、省略可)
  // 0除算を避けるために0の場合はデフォルト周期0.01とする
  if(delta == 0) delta = 0.01;
  // 現在の偏差を求める(目標値と現在値の差)
  double currentDeviation = targetValue - currentValue;
  // 積分の処理を行う(前回の誤差上辺、今回の誤差を下辺とする台形の面積を求める)
  integral += (preDeviation + currentDeviation) * delta / 2;
  // 微分の処理を行う(偏差の時間(delta)に対する傾きを近似)
  double difference = (currentDeviation - preDeviation) / delta;
  // 前回の偏差を更新する
  preDeviation = currentDeviation;

  // P制御の計算を行う
  double p = gain.kp * currentDeviation;
  // I制御の計算を行う
  double i = gain.ki * integral;
  // D制御の計算を行う
  double d = gain.kd * difference;

  // 以下の2つの為の条件
  // 0除算によるNaN発生を防ぐ
  // 時定数が0の時の無駄な計算を避ける
  if(timeConstant != 0.0 && gain.kp != 0.0) {
    // D値に一次遅れフィルタを適用
    d = d / (1.0 + timeConstant * (fabs(d) / gain.kp));
  }

  // 操作量 = P制御 + I制御 + D制御
  return p + i + d;
}
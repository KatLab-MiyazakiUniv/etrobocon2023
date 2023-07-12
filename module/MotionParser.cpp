/**
 * @file   MotionParser.cpp
 * @brief  動作コマンドファイルを解析するクラス
 * @author aridome222
 */

#include "MotionParser.h"

using namespace std;

vector<Motion*> MotionParser::createMotions(const char* commandFilePath, int targetBrightness,
                                            bool& isLeftEdge)
{
  const int BUF_SIZE = 512;
  char buf[BUF_SIZE];  // log用にメッセージを一時保持する領域
  Logger logger;
  int lineNum = 1;  // Warning用の行番号

  vector<Motion*> motionList;  // 動作インスタンスのリスト

  // ファイル読み込み
  FILE* fp = fopen(commandFilePath, "r");
  // ファイル読み込み失敗
  if(fp == NULL) {
    snprintf(buf, BUF_SIZE, "%s file not open!\n", commandFilePath);
    logger.logWarning(buf);
    return motionList;
  }

  char row[BUF_SIZE];           // 各行の文字を一時的に保持する領域
  const char* separator = ",";  // 区切り文字

  // 行ごとにパラメータを読み込む
  while(fgets(row, BUF_SIZE, fp) != NULL) {
    vector<char*> params;
    // separatorを区切り文字にしてrowを分解し，paramに代入する
    char* param = strtok(row, separator);
    while(param != NULL) {
      // paramをパラメータとして保持する
      params.push_back(param);
      // 次のパラメータをparamに代入する
      // strtok()は第1引数にNULLを与えると、前回の続きのアドレスから処理が開始される
      param = strtok(NULL, separator);
    }

    // 取得したパラメータから動作インスタンスを生成する
    COMMAND command = convertCommand(params[0]);  // 行の最初のパラメータをCOMMAND型に変換
    if(command == COMMAND::DL) {  // 指定距離ライントレース動作の生成
      DistanceLineTracing* dl = new DistanceLineTracing(
          atof(params[1]),                                             // 目標距離
          targetBrightness + atoi(params[2]),                          // 目標輝度 + 調整
          atof(params[3]),                                             // 目標速度
          PidGain(atof(params[4]), atof(params[5]), atof(params[6])),  // PIDゲイン
          isLeftEdge);                                                 // エッジ

      motionList.push_back(dl);          // 動作リストに追加
    } else if(command == COMMAND::CL) {  // 指定色ライントレース動作の生成
      ColorLineTracing* cl = new ColorLineTracing(
          ColorJudge::stringToColor(params[1]),                        // 目標色
          targetBrightness + atoi(params[2]),                          // 目標輝度 + 調整
          atof(params[3]),                                             // 目標速度
          PidGain(atof(params[4]), atof(params[5]), atof(params[6])),  // PIDゲイン
          isLeftEdge);                                                 // エッジ

      motionList.push_back(cl);          // 動作リストに追加
    } else if(command == COMMAND::DS) {  // 指定距離直進動作の生成
      DistanceStraight* ds = new DistanceStraight(atof(params[1]),   // 目標距離
                                                  atof(params[2]));  // 目標速度

      motionList.push_back(ds);          // 動作リストに追加
    } else if(command == COMMAND::CS) {  // 指定色直進動作の生成
      ColorStraight* cs = new ColorStraight(ColorJudge::stringToColor(params[1]),  // 目標色
                                            atof(params[2]));                      // 目標速度

      motionList.push_back(cs);                               // 動作リストに追加
    } else if(command == COMMAND::AR) {                       // 指定角度回頭動作の生成
      AngleRotation* ar = new AngleRotation(atoi(params[1]),  // 目標角度
                                            atof(params[2]),  // 目標速度
                                            convertBool(params[0], params[3]));  // 回頭方向

      motionList.push_back(ar);                        // 動作リストに追加
    } else if(command == COMMAND::EC) {                // エッジ切り替えの生成
      EdgeChanging* ec = new EdgeChanging(isLeftEdge,  // エッジ
                                          convertBool(params[0], params[1]));  // 切り替え後のエッジ

      motionList.push_back(ec);          // 動作リストに追加
    } else if(command == COMMAND::SL) {  // 自タスクスリープの生成
      Sleeping* sl = new Sleeping(atoi(params[1]));

      motionList.push_back(sl);  // 動作リストに追加
    }
    // TODO: 後で作成する
    /*else if(command == COMMAND::DT) {  // 距離指定旋回動作の生成
      DistanceTurning* dt = new DistanceTurning(atof(params[1]),   // 目標距離
                                                atoi(params[2]),   // 左モータのPWM値
                                                atoi(params[3]));  // 右モータのPWM値

      motionList.push_back(dt);                                    // 動作リストに追加
    } else if(command == COMMAND::AU) {  // アームを上げる
      ArmUpping* au = new ArmUpping(atoi(params[1]), atoi(params[2]));

      motionList.push_back(au);          // 動作リストに追加
    } else if(command == COMMAND::AD) {  // アームを下げる
      ArmDownning* ad = new ArmDownnig(atoi(params[1]), atoi(params[2]));

      motionList.push_back(ad);          // 動作リストに追加
    } else if(command == COMMAND::XR) {  // 角度補正回頭の追加
      CorrectingRotation* xr = new CorrectingRotation(atoi(params[1]),   // 目標角度
                                                      atoi(params[2]));  // 目標速度

      motionList.push_back(xr);                                          // 動作リストに追加
    }
    */
    else {  // 未定義のコマンドの場合
      snprintf(buf, BUF_SIZE, "%s:%d: '%s' is undefined command", commandFilePath, lineNum,
               params[0]);
      logger.logWarning(buf);
    }
    lineNum++;  // 行番号をインクリメントする
  }

  // ファイルを閉じる
  fclose(fp);

  return motionList;
}

COMMAND MotionParser::convertCommand(char* str)
{
  if(strcmp(str, "DL") == 0) {  // 文字列がDLの場合
    return COMMAND::DL;
  } else if(strcmp(str, "CL") == 0) {  // 文字列がCLの場合
    return COMMAND::CL;
  } else if(strcmp(str, "DS") == 0) {  // 文字列がDSの場合
    return COMMAND::DS;
  } else if(strcmp(str, "CS") == 0) {  // 文字列がCSの場合
    return COMMAND::CS;
  } else if(strcmp(str, "AR") == 0) {  // 文字列がARの場合
    return COMMAND::AR;
  } else if(strcmp(str, "DT") == 0) {  // 文字列がDTの場合
    return COMMAND::DT;
  } else if(strcmp(str, "EC") == 0) {  // 文字列がECの場合
    return COMMAND::EC;
  } else if(strcmp(str, "SL") == 0) {  // 文字列がSLの場合
    return COMMAND::SL;
  } else if(strcmp(str, "AU") == 0) {  // 文字列がAUの場合
    return COMMAND::AU;
  } else if(strcmp(str, "AD") == 0) {  // 文字列がADの場合
    return COMMAND::AD;
  } else if(strcmp(str, "XR") == 0) {  // 文字列がXRの場合
    return COMMAND::XR;
  } else {  // 想定していない文字列が来た場合
    return COMMAND::NONE;
  }
}

bool MotionParser::convertBool(char* command, char* stringParameter)
{
  Logger logger;

  // 末尾の改行を削除
  char* param = StringOperator::removeEOL(stringParameter);

  if(strcmp(command, "AR") == 0) {         //  コマンドがARの場合
    if(strcmp(param, "clockwise") == 0) {  // パラメータがclockwiseの場合
      return true;
    } else if(strcmp(param, "anticlockwise") == 0) {  // パラメータがanticlockwiseの場合
      return false;
    } else {  // 想定していないパラメータが来た場合
      logger.logWarning("Parameter before conversion must be 'clockwise' or 'anticlockwise'");
      return true;
    }
  }

  if(strcmp(command, "EC") == 0) {    //  コマンドがECの場合
    if(strcmp(param, "left") == 0) {  // パラメータがleftの場合
      return true;
    } else if(strcmp(param, "right") == 0) {  // パラメータがrightの場合
      return false;
    } else {  // 想定していないパラメータが来た場合
      logger.logWarning("Parameter before conversion must be 'left' or 'right'");
      return true;
    }
  }
}
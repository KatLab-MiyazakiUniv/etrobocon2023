/**
 * @file   MotionParser.cpp
 * @brief  動作コマンドファイルを解析するクラス
 * @author aridome222 bizyutyu KakinokiKanta
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
          atof(params[2]),                                             // 目標速度
          targetBrightness + atoi(params[3]),                          // 目標輝度 + 調整
          PidGain(atof(params[4]), atof(params[5]), atof(params[6])),  // PIDゲイン
          isLeftEdge);                                                 // エッジ

      motionList.push_back(dl);          // 動作リストに追加
    } else if(command == COMMAND::CL) {  // 指定色ライントレース動作の生成
      ColorLineTracing* cl = new ColorLineTracing(
          ColorJudge::stringToColor(params[1]),                        // 目標色
          atof(params[2]),                                             // 目標速度
          targetBrightness + atoi(params[3]),                          // 目標輝度 + 調整
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

      motionList.push_back(sl);          // 動作リストに追加
    } else if(command == COMMAND::CA) {  // 距離指定旋回動作の生成
      CameraAction* ca = new CameraAction(
          convertSubject(params[1]),  // フラグ確認を行うかの判断に用いる撮影対象
          convertBool(params[0], params[2]),  // リアカメラをミニフィグに向けるための回頭方向
          atoi(params[3]),                    // 撮影のための目標角度
          atoi(params[4]));                   // 黒線復帰のための目標角度

      motionList.push_back(ca);  // 動作リストに追加
    }
    // TODO: 後で作成する
    /*else if(command == COMMAND::DT) {  // 距離指定旋回動作の生成
      DistanceTurning* dt = new DistanceTurning(atof(params[1]),   // 目標距離
                                                atoi(params[2]),   // 左モータのPWM値
                                                atoi(params[3]));  // 右モータのPWM値

      motionList.push_back(dt);                                    // 動作リストに追加
    }
    */
    else if(command == COMMAND::AM) {  // アーム動作の追加
      ArmMotion* am = new ArmMotion(atoi(params[1]), atoi(params[2]));

      motionList.push_back(am);          // 動作リストに追加
    } else if(command == COMMAND::XR) {  // 角度補正回頭の追加
      CorrectingRotation* xr = new CorrectingRotation(atoi(params[1]),   // 目標角度
                                                      atof(params[2]));  // 目標速度

      motionList.push_back(xr);                     // 動作リストに追加
    } else if(command == COMMAND::IS) {             // 交点内移動（直進）
      InCrossStraight* is = new InCrossStraight();  // 目標速度 [mm/s]

      motionList.push_back(is);          // 動作リストに追加
    } else if(command == COMMAND::IL) {  // 交点内移動（左折）
      InCrossLeft* il = new InCrossLeft(isLeftEdge);

      motionList.push_back(il);          // 動作リストに追加
    } else if(command == COMMAND::IR) {  // 交点内移動（右折）
      InCrossRight* ir = new InCrossRight(isLeftEdge);

      motionList.push_back(ir);          // 動作リストに追加
    } else if(command == COMMAND::BR) {  // 後ろを向く
      BackRotation* br = new BackRotation(isLeftEdge);

      motionList.push_back(br);          // 動作リストに追加
    } else if(command == COMMAND::CC) {  // 交点サークルから交点サークル
      CrossToCross* cc = new CrossToCross(ColorJudge::stringToColor(params[1]),  // 目標色
                                          targetBrightness + atoi(params[2]),    // 目標輝度
                                          isLeftEdge);                           // エッジ

      motionList.push_back(cc);                           // 動作リストに追加
    } else if(command == COMMAND::PR) {                   // Pwm値指定回頭動作
      PwmRotation* pr = new PwmRotation(atoi(params[1]),  // 目標角度
                                        atoi(params[2]),  // Pwm値
                                        convertBool(params[0], params[3]));  // 回頭方向

      motionList.push_back(pr);          // 動作リストに追加
    } else if(command == COMMAND::ST) {  // 左右モーターストップ
      Stop* st = new Stop();

      motionList.push_back(st);          // 動作リストに追加
    } else if(command == COMMAND::BT) {  // ブロック投げ入れ
      BlockThrowing* bt = new BlockThrowing();

      motionList.push_back(bt);  // 動作リストに追加
    } else {                     // 未定義のコマンドの場合
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
  } else if(strcmp(str, "AM") == 0) {  // 文字列がAMの場合
    return COMMAND::AM;
  } else if(strcmp(str, "XR") == 0) {  // 文字列がXRの場合
    return COMMAND::XR;
  } else if(strcmp(str, "CA") == 0) {  // 文字列がCAの場合
    return COMMAND::CA;
  } else if(strcmp(str, "IS") == 0) {  // 文字列がISの場合
    return COMMAND::IS;
  } else if(strcmp(str, "IL") == 0) {  // 文字列がILの場合
    return COMMAND::IL;
  } else if(strcmp(str, "IR") == 0) {  // 文字列がIRの場合
    return COMMAND::IR;
  } else if(strcmp(str, "BR") == 0) {  // 文字列がBRの場合
    return COMMAND::BR;
  } else if(strcmp(str, "CC") == 0) {  // 文字列がCCの場合
    return COMMAND::CC;
  } else if(strcmp(str, "PR") == 0) {  // 文字列がPRの場合
    return COMMAND::PR;
  } else if(strcmp(str, "ST") == 0) {  // 文字列がSTの場合
    return COMMAND::ST;
  } else if(strcmp(str, "BT") == 0) {  // 文字列がBTの場合
    return COMMAND::BT;
  } else {  // 想定していない文字列が来た場合
    return COMMAND::NONE;
  }
}

bool MotionParser::convertBool(char* command, char* stringParameter)
{
  Logger logger;

  // 末尾の改行を削除
  char* param = StringOperator::removeEOL(stringParameter);

  if(strcmp(command, "AR") == 0 || strcmp(command, "CM") == 0
     || strcmp(command, "PR") == 0) {      //  コマンドがAR, CM, PRの場合
    if(strcmp(param, "clockwise") == 0) {  // パラメータがclockwiseの場合
      return true;
    } else if(strcmp(param, "anticlockwise") == 0) {  // パラメータがanticlockwiseの場合
      return false;
    } else {  // 想定していないパラメータが来た場合
      logger.logWarning("Parameter before conversion must be 'clockwise' or 'anticlockwise'");
      return true;
    }
  }

  if(strcmp(command, "EC") == 0) {    //  コマンドがEC, CMの場合
    if(strcmp(param, "left") == 0) {  // パラメータがleftの場合
      return true;
    } else if(strcmp(param, "right") == 0) {  // パラメータがrightの場合
      return false;
    } else {  // 想定していないパラメータが来た場合
      logger.logWarning("Parameter before conversion must be 'left' or 'right'");
      return true;
    }
  }

  if(strcmp(command, "CA") == 0) {         //  コマンドがCAの場合
    if(strcmp(param, "clockwise") == 0) {  // パラメータがclockwiseの場合
      return true;
    } else if(strcmp(param, "anticlockwise") == 0) {  // パラメータがanticlockwiseの場合
      return false;
    } else {  // 想定していないパラメータが来た場合
      logger.logWarning("Parameter before conversion must be 'clockwise' or 'anticlockwise'");
      return true;
    }
  }
  logger.logWarning("Using a command that is not defined in convertBool.");
  return true;
}

CameraAction::Subject MotionParser::convertSubject(char* stringParameter)
{
  Logger logger;

  // 末尾の改行を削除
  char* param = StringOperator::removeEOL(stringParameter);

  if(strcmp(param, "A") == 0) {  // パラメータがAの場合
    return CameraAction::Subject::A;
  } else if(strcmp(param, "B") == 0) {  // パラメータがBの場合
    return CameraAction::Subject::B;
  } else if(strcmp(param, "BA") == 0) {  // パラメータがBAの場合
    return CameraAction::Subject::BLOCK_AREA;
  } else {  // 想定していないパラメータが来た場合
    logger.logWarning("Parameter before conversion must be 'A' or 'B' or 'BA'");
    return CameraAction::Subject::UNDEFINED;
  }
}
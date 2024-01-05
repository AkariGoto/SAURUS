/**
 *  ファイル名
 *		TripodGaitPlanner.cpp
 *  説明
 *		基準歩容計画クラス（トロット歩容での歩行）
 *  日付
 *		作成日: 2008/12/19(FRI)		更新日: 2018/12/19(FRI)
 */

 //  20200819  安定余裕関連コメントアウト
 //  20200820  TROTGAITをTRIPODGAITに置換・TrotGaitをTripodGaitに置換
 //  20200821  根本の位置を固定
 //  20200929  支持脚時のz軸方向を追加
 //  20200930  遊脚時の開始位置
 //  20201005
 //  20201016  歩行開始時の姿勢へのセット
 //  20201017  歩行開始時の姿勢へのセット
 //  20201018  歩行開始時の姿勢へのセット
 //  20201020  動作停止後の再動作
 //  20220713  関節の可動域チェック

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
#include "TripodGaitPlanner.h"
  //#include <winsock2.h>
using namespace std;
using namespace Math;
using namespace Asura;
using namespace Const;

namespace Plan
{
/**
 *	----------------------------------------------------------------------
 *		TripodGaitPlannerクラス
 *	----------------------------------------------------------------------
 */

 /**
  *	------------------------------------------------------------
  *		TripodGaitPlannerクラスのメンバ関数定義
  *	------------------------------------------------------------
  */

  /**
   *	----------------------------------------
   *	コンストラクタとデストラクタ
   *	----------------------------------------
   */
   /// デフォルトコンストラクタ
TripodGaitPlanner::TripodGaitPlanner(AsuraX* asuraPointer_, TimeManager* timeManagerPointer_)
    : Planner(asuraPointer_, timeManagerPointer_)
{
    /// オブジェクトの生成
    newTripodGaitItems();

    /// 初期化
    initializeTripodGaitPlanner();

}

/// デストラクタ
TripodGaitPlanner::~TripodGaitPlanner()
{
    /// オブジェクトの破棄
    deleteTripodGaitItems();

}

/**
 *	基準歩容の初期化
 */
void TripodGaitPlanner::initializeTripodGaitPlanner(void)
{
    /// メンバ変数の初期化
    //stabilityMargin = 0.0;  20200819
    walkingCounter = 0;

    swingLegWaitingToStop = 0;

    /// フラグ類の初期化
    isWalkingToGetSet = true;
    isTrajectoryToGetSet = true;
    isWalkingStarted = false;
    isWaitingToStop = false;
    isSetting = false;  //20201016

    //20201017  姿勢移行に使用する変数の初期化
    settingStartTime = 0;
    settingTime = 0;

    /// 時間
    walkingTime = 0.0;
    cycleStartTime = 0.0;
    normalizedWalkingTime = 0.0;
    cycleElapsedTime = 0.0;  //20201020

    /// デューティ比
    dutyFactor = TRIPODGAIT_DUTY_FACTOR;

    /// 歩行速度を計算
    walkingSpeed = TRIPODGAIT_WALKING_SPEED;

    /// ストライドのセット
    stride = TRIPODGAIT_STRIDE;

    /// 歩行方向のセット
    unitWalkingDirection = Vector(DEFAULT_LOCOMOTION_DIRECTION, THREE_DIMENSION);//<----

    //20200929  支持脚時に使用するz軸方向のセット
    unitUpDirection = Vector(UP_DIRECTION, THREE_DIMENSION);

    /// 位置ベクトルの初期化
    footReferencePosition[0] = Vector(TRIPODGAIT_FOOT_REF_POSITION_1, THREE_DIMENSION);
    footReferencePosition[1] = Vector(TRIPODGAIT_FOOT_REF_POSITION_2, THREE_DIMENSION);
    footReferencePosition[2] = Vector(TRIPODGAIT_FOOT_REF_POSITION_3, THREE_DIMENSION);
    footReferencePosition[3] = Vector(TRIPODGAIT_FOOT_REF_POSITION_4, THREE_DIMENSION);
    footReferencePosition[4] = Vector(TRIPODGAIT_FOOT_REF_POSITION_5, THREE_DIMENSION);
    footReferencePosition[5] = Vector(TRIPODGAIT_FOOT_REF_POSITION_6, THREE_DIMENSION);
    /// 遊脚運動
    swingUp = Vector(TRIPODGAIT_SWING_UP, THREE_DIMENSION);
    swingDown = Vector(TRIPODGAIT_SWING_DOWN, THREE_DIMENSION);

    /// デフォルト値による歩容パラメータの計算
    calculateGaitParameters();

    return;

}

/**
 *	歩容パラメータの計算
 */
void TripodGaitPlanner::calculateGaitParameters(void)
{
    /// 計算済みなので必要なし
    if (!isTrajectoryToGetSet)
        return;

    /// 歩行周期を計算
    cycleTime = (stride) / walkingSpeed / dutyFactor;//7/8;//<------!(stride-100)

    /// ローカル変数の宣言
    double swingTime = 0;
    double swingUpTime = 0;
    double swingReturnTime = 0;
    double swingDownTime = 0;

    /// 遊脚時間の計算
    swingTime = 1.0 - dutyFactor;//swingTime = (1+TRIPODGAIT_SWING_DOWN[2]/TRIPODGAIT_SWING_UP[2])/2;//SwingTime=0.5(1 - dutyFactor);//
    /// （ユーザによって配分は変更）
    swingUpTime = swingTime * 1 / 4;
    swingReturnTime = swingTime * 2 / 4;
    swingDownTime = swingTime * 1 / 4;

    /// 各脚の遊脚開始時間の設定
    //swingStartTime[0] = dutyFactor - 0.5;
    //swingStartTime[1] = 2*dutyFactor - 0.5;
    //swingStartTime[2] = 2*dutyFactor - 1.0;
    //swingStartTime[3] = dutyFactor;
    swingStartTime[0] = 0;
    swingStartTime[1] = dutyFactor;
    swingStartTime[2] = 0;
    swingStartTime[3] = dutyFactor;
    swingStartTime[4] = 0;
    swingStartTime[5] = dutyFactor;


    /// 遊脚時間をセットする
    int i;
    for (i = 0; i < LEG_NUM; i++)
    {
        setLegSwingTime(i + 1, swingStartTime[i], swingUpTime, swingReturnTime, swingDownTime);
        swingStopTime[i] = swingStartTime[i] + 1.0 - dutyFactor;
    }

    /// 遊脚開始位置と終了位置の計算および足初期位置計算
    double a[3];
    a[0] = TRIPODGAIT_SWING_UP[0] + TRIPODGAIT_SWING_DOWN[0];
    a[1] = TRIPODGAIT_SWING_UP[1] + TRIPODGAIT_SWING_DOWN[1];
    a[2] = TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2];

    int j;
    for (j = 0; j < LEG_NUM; j++)
    {
        swingStartPosition[j] = footReferencePosition[j] - (stride) / 2 * unitWalkingDirection;//<------!(stride-100) 20200930  遊脚開始位置
        //swingStopPosition[j] = footReferencePosition[j] + (stride)/2*unitWalkingDirection+Vector(a, THREE_DIMENSION);//<------!(stride-100)//ここなおす

        if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] != 0)  //20200930  上り下り時の遊脚終了位置
        {
            swingStopPosition[j] = footReferencePosition[j] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
        }
        else  //20200930  平地の遊脚終了位置
        {
            swingStopPosition[j] = footReferencePosition[j] + (stride) / 2 * unitWalkingDirection;
        }



    }

    /// 遊脚軌道をセットする
    int k;
    for (k = 0; k < LEG_NUM; k++)
    {
        setLegSwingTrajectory(k + 1,
                    swingStartPosition[k],
                    swingUp,
                    (stride)*unitWalkingDirection, //<------!(stride-100)
                    swingDown
        );
    }

    /// 初期位置のセット  
    /*  20200930
    initialFootPosition[0] = footReferencePosition[0] - (stride)/2*unitWalkingDirection;
    initialFootPosition[1] = footReferencePosition[1] + (stride)/2*unitWalkingDirection;//<------!(stride-100)
    initialFootPosition[2] = footReferencePosition[2] - (stride)/2*unitWalkingDirection;
    initialFootPosition[3] = footReferencePosition[3] + (stride)/2*unitWalkingDirection;//<------!(stride-100)
    initialFootPosition[4] = footReferencePosition[4] - (stride)/2*unitWalkingDirection;
    initialFootPosition[5] = footReferencePosition[5] + (stride)/2*unitWalkingDirection;//<------!(stride-100)
    */

    if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] > 0)
    {
        initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
        initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
        //initialFootPosition[4] = footReferencePosition[4] - (stride) / 2 * unitWalkingDirection;
        //initialFootPosition[5] = footReferencePosition[5] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
    }
    else if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] < 0)
    {
        initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
        initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
        //initialFootPosition[4] = footReferencePosition[4] - (stride) / 2 * unitWalkingDirection;
        //initialFootPosition[5] = footReferencePosition[5] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
    }
    else
    {
        initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection;
        initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection;
        //initialFootPosition[4] = footReferencePosition[4] - (stride) / 2 * unitWalkingDirection;
        //initialFootPosition[5] = footReferencePosition[5] + (stride) / 2 * unitWalkingDirection;
    }


    /// 胴体の初期位置
    int l;
    for (l = 0; l < THREE_DIMENSION; l++)
        initialBodyPosition(l + 1) = TRIPODGAIT_INITIAL_BODY_POSITION[l];

    isTrajectoryToGetSet = false;
    /*
      WSAData wsaData;
      WSAStartup(MAKEWORD(2,0), &wsaData);
    */
    return;
}

/**
 *	歩行初期姿勢に移行
 */
bool TripodGaitPlanner::shiftToInitialStandingPosture(void)
{
    Kinematics kine = NO_KINE_ERROR;

    /// 重心を初期位置に
    asuraPointer->initializeBodyPosition(initialBodyPosition);

    /// 足位置を初期姿勢に
    /*  20201016
    int i;
    for (i=0; i<LEG_NUM; i++)
    {
      kine = asuraPointer->placeLegFootPosition(i+1, initialFootPosition[i]);

      if (kine != NO_KINE_ERROR)
      {
        cerr << "[TripodGaitPlanner::shiftToInitialStandingPosture] Cannot shift to initial posture" << endl;
        Planner::printPlanErrorMessage();
      }
    }
    */
    isSetting = true;  //姿勢移行のフラグ

    //20201017
    //各関節角度取得
    for (int i = 0; i < LEG_NUM; i++)
    {
        for (int j = 0; j < LEG_JOINT_NUM; j++)
        {
            initialJointAngle[i][j] = asuraPointer->getLegJointAngle(i + 1)(j + 1);
        }
        initialFootJointAngle[i] = asuraPointer->getFootJointAngle(i + 1);
    }
    for (int i = 0; i < LEG_NUM; i++)
    {
        kine = asuraPointer->placeLegFootPosition(i + 1, initialFootPosition[i]);
        for (int j = 0; j < LEG_JOINT_NUM; j++)
        {
            finalJointAngle[i][j] = asuraPointer->getLegJointAngle(i + 1)(j + 1);
        }
        finalFootJointAngle[i] = asuraPointer->getFootJointAngle(i + 1);
    }

    //所要時間算出
    for (int i = 0; i < LEG_NUM; i++)
    {
        for (int j = 0; j < LEG_JOINT_NUM; j++)
        {
            if (finalJointAngle[i][j] >= initialJointAngle[i][j])
            {
                angularVelosity[i][j] = ANGULAR_VELOCITY;
            }
            else
            {
                angularVelosity[i][j] = -1 * ANGULAR_VELOCITY;
            }
            if (finalFootJointAngle[i] >= initialFootJointAngle[i])
            {
                angularVelosity[i][LEG_JOINT_NUM] = ANGULAR_VELOCITY;
            }
            else
            {
                angularVelosity[i][LEG_JOINT_NUM] = -1 * ANGULAR_VELOCITY;
            }
        }

        t01[i] = (finalJointAngle[i][0] - initialJointAngle[i][0]) / angularVelosity[i][0];
        t02[i] = t01[i] + (finalJointAngle[i][1] - initialJointAngle[i][1]) / angularVelosity[i][1];
        t03[i] = t02[i] + (finalJointAngle[i][2] - initialJointAngle[i][2]) / angularVelosity[i][2];
        t04[i] = t03[i] + (finalFootJointAngle[i] - initialFootJointAngle[i]) / angularVelosity[i][3];
        /*
        t01[i] = (finalJointAngle[i][0] - initialJointAngle[i][0]) / angularVelosity[i][0];
        t02[i] = (finalJointAngle[i][1] - initialJointAngle[i][1]) / angularVelosity[i][1];
        t03[i] = (finalJointAngle[i][2] - initialJointAngle[i][2]) / angularVelosity[i][2];
        t04[i] = (finalFootJointAngle[i] - initialFootJointAngle[i]) / angularVelosity[i][3];
        */
        if (t01[i] > t02[i] && t01[i] > t03[i] && t01[i] > t04[i])
        {
            t05[i] = t01[i];
        }
        if (t02[i] > t01[i] && t02[i] > t03[i] && t02[i] > t04[i])
        {
            t05[i] = t02[i];
        }
        if (t03[i] > t01[i] && t03[i] > t02[i] && t03[i] > t04[i])
        {
            t05[i] = t03[i];
        }
        else
        {
            t05[i] = t04[i];
        }
    }

    //姿勢移行開始時刻取得
    settingStartTime = timeManagerPointer->getRealTime();


    return true;
}

/**
 *	------------------------------------------------------------
 *	オーバーライド関数
 *		運動を具体的に生成する関数群
 *	------------------------------------------------------------
 */
 /**
  *	歩行開始のための初期化
  */

bool TripodGaitPlanner::setup(void)
{
    /// ポインタがセットされているかの確認
    if (asuraPointer == NULL || timeManagerPointer == NULL)
    {
        cerr << "[TripodGaitPlanner::setup] No control object\n" << endl;

        return false;
    }


    if (isTrajectoryToGetSet)
    {
        cerr << "[TripodGaitPlanner::setup] No trajectories are ready\n" << endl;

        return false;
    }

    if (!shiftToInitialStandingPosture())
    {
        cerr << "[TripodGaitPlanner::setup] Cannot shift to initial posture\n" << endl;

        return false;
    }

    /// 歩行準備完了
    isWalkingToGetSet = false;

    cycleElapsedTime = 0.0;  //20201020

    //changeflag=0x00;		//追加

    return Planner::setup();
}


/// 歩行を開始する
bool TripodGaitPlanner::startPlan(void)
{
    isRunning = true;  //20201020

    if (isWalkingToGetSet || !isRunning)
    {
        cerr << "[TripodGaitPlanner::startPlan] Not stand-by for walking\n" << endl;

        return false;
    }

    /// 歩行周期開始時間を更新
    //cycleStartTime = timeManagerPointer->getRealTime();  //20201020
    cycleStartTime = timeManagerPointer->getRealTime() - cycleElapsedTime;  //20201020

    isWalkingStarted = true;

    walkingCounter = 0;  //20201020

    return Planner::startPlan();
}

/// 歩行を停止する
bool TripodGaitPlanner::stopPlan(void)
{
    isWalkingStarted = false;
    //isWalkingToGetSet = true;  //20201020
    //walkingCounter = 0;  //20201020

    cycleElapsedTime = timeManagerPointer->getRealTime() - cycleStartTime;  //20201020

    isRunning = false;  //20201020

    return Planner::stopPlan();
}

/**
 *	歩行を1歩終了時に停止する
 */
bool TripodGaitPlanner::standByForStop(void)
{
    isWaitingToStop = true;

    //20201005
    /*
    if (0 < normalizedWalkingTime && normalizedWalkingTime<= swingStopTime[1])
    {
      swingLegWaitingToStop = 1;
    }
    else if (swingStopTime[1] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[0])
    {
      swingLegWaitingToStop = 2;
    }
    else if (swingStopTime[0] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[2])
    {
      swingLegWaitingToStop = 3;
    }
    else if (swingStopTime[2] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[3])
    {
      swingLegWaitingToStop = 4;
    }*/


    /*	if (0 < normalizedWalkingTime && normalizedWalkingTime<= swingStopTime[1])
      {
        swingLegWaitingToStop = 1;
      }
      else if (swingStopTime[1] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[0])
      {
        swingLegWaitingToStop = 2;
      }
      else if (swingStopTime[0] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[2])
      {
        swingLegWaitingToStop = 3;
      }
      else if (swingStopTime[2] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[3])
      {
        swingLegWaitingToStop = 4;
      }
      else if (swingStopTime[3] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[4])
      {
        swingLegWaitingToStop = 5;
      }
      else if(swingStopTime[4] < normalizedWalkingTime && normalizedWalkingTime <= swingStopTime[5])
      {
        swingLegWaitingToStop = 6;
      }
      */
    return isWaitingToStop;
}

/**
 *	脚運動を生成する
 */
PlanStatus TripodGaitPlanner::activateRobot(void)
{
    /**
     *		正規化した歩行時間により脚の運動を決定する
     */
     /// 動作状態
    PlanStatus plan = Plan::PlanStatus::WAIT;
    /// 逆運動学を解いた結果
    Kinematics kine = NO_KINE_ERROR;
    /// カウンタ
    int i;

    double swingTime = (1 + TRIPODGAIT_SWING_DOWN[2] / TRIPODGAIT_SWING_UP[2]) / 2;



    ///1,3,5遊脚　2,4,6支持
    if (0.00 <= normalizedWalkingTime && normalizedWalkingTime < TRIPODGAIT_DUTY_FACTOR)
    {
        for (i = 0; i < LEG_NUM; i++)
        {
            switch (i)
            {
                ///第 2, 4, 6脚の支持脚運動
                case 1://2
                case 3://4
                case 5://6
                {
                    if (TRIPODGAIT_SWING_UP[2] == -TRIPODGAIT_SWING_DOWN[2])  //20200929  平面時の支持脚運動
                    {
                        kine = asuraPointer->placeLegFootPosition(
                          i + 1,
                          initialFootPosition[i] - normalizedWalkingTime * cycleTime * walkingSpeed * unitWalkingDirection
                        );
                    }
                    else  //上り下りでの支持脚運動
                    {
                        kine = asuraPointer->placeLegFootPosition(
                          i + 1,
                          initialFootPosition[i] - normalizedWalkingTime * cycleTime * walkingSpeed * unitWalkingDirection
                          + normalizedWalkingTime * cycleTime * -(TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) / (cycleTime * dutyFactor) * unitUpDirection
                        );
                    }

                    /// 支持脚相にセット
                    asuraPointer->setLegPhase(i + 1, LegPhase::SUPPORT);

                    //if (kine != NO_KINE_ERROR)
                    if (kine != NO_KINE_ERROR && i == 1)  //脚2についてのみ可動域など確認
                    {
                        cerr << "[TripodGaitPlanner::activateRobot]" << endl;
                        Planner::printPlanErrorMessage();

                        suspendPlan();
                        return PlanStatus::SUSPEND;
                    }
                }
                break;
                ///第1,3,5 脚の遊脚運動
                case 0:
                case 2:
                case 4:
                {
                    /// 前周期からの続きのため遊脚運動だけは時間補正で1.0を加える
                    plan = swingLeg(i + 1, normalizedWalkingTime);
                    /// 遊脚相にセット
                    asuraPointer->setLegPhase(i + 1, LegPhase::SWING);

                    //if (plan == INVALID)
                    if (plan == PlanStatus::INVALID && i == 1)  //脚2についてのみ可動域など確認
                    {
                        Planner::printPlanErrorMessage();

                        suspendPlan();
                        return PlanStatus::SUSPEND;
                    }

                    break;
                }
                default:
                    break;
            }	/// end of switch (i)
        }	/// end of for (i) loop222
        asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection * normalizedWalkingTime * (stride));
    }
    ///1,3,5支持　2,4,6遊脚
    else if (TRIPODGAIT_DUTY_FACTOR <= normalizedWalkingTime && normalizedWalkingTime < 1.00)//TRIPODGAITDUTY_FACTOR
    {
        for (i = 0; i < LEG_NUM; i++)
        {
            switch (i)
            {
                /// 1, 3, 5脚の支持脚運動
                case 0:
                case 2:
                case 4:
                {
                    if (TRIPODGAIT_SWING_UP[2] == -TRIPODGAIT_SWING_DOWN[2])  //20200929  平面時の支持脚運動
                    {
                        kine = asuraPointer->placeLegFootPosition(
                          i + 1,
                          swingStopPosition[i] - (normalizedWalkingTime - swingStopTime[i]) * cycleTime * walkingSpeed * unitWalkingDirection
                        );
                    }
                    else  //上り下りでの支持脚運動
                    {
                        kine = asuraPointer->placeLegFootPosition(
                          i + 1,
                          swingStopPosition[i] - (normalizedWalkingTime - swingStopTime[i]) * cycleTime * walkingSpeed * unitWalkingDirection
                          + (normalizedWalkingTime - swingStopTime[i]) * cycleTime * -(TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) / (cycleTime * dutyFactor) * unitUpDirection
                        );
                    }


                    /// 支持脚相にセット
                    asuraPointer->setLegPhase(i + 1, LegPhase::SUPPORT);

                    //if (kine != NO_KINE_ERROR
                    if (kine != NO_KINE_ERROR && i == 1)  //脚2についてのみ可動域など確認
                    {
                        cerr << "[TripodGaitPlanner::activateRobot]" << endl;
                        Planner::printPlanErrorMessage();

                        suspendPlan();
                        return PlanStatus::SUSPEND;
                    }
                }
                break;
                /// 2, 4, 6脚の遊脚運動
                case 1:
                case 3:
                case 5:
                {
                    plan = swingLeg(i + 1, normalizedWalkingTime);
                    /// 遊脚相にセット
                    asuraPointer->setLegPhase(i + 1, LegPhase::SWING);

                    //if (plan == INVALID)
                    if (plan == PlanStatus::INVALID && i == 1)  //脚2についてのみ可動域など確認
                    {
                        Planner::printPlanErrorMessage();

                        suspendPlan();
                        return PlanStatus::SUSPEND;
                    }
                }
                //isWaitingToStop=true;
                break;

                default:
                    break;
            }	/// end of switch (i)
        }	/// end of for (i) loop
        asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection * normalizedWalkingTime * (stride));
    }/// end of if ( normalizedWalkingTime )

    /// ロボットの胴体位置を更新	old position + direction * Time + Stride
    //asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection*normalizedWalkingTime*(stride) + Vector(a, THREE_DIMENSION) );//<------!(stride-100)

    //本体を固定する場合  20200821
    asuraPointer->initializeBodyPosition(initialBodyPosition);

    //本体を固定しない場合  20200821
    //asuraPointer->initializeBodyPosition(initialBodyPosition + unitWalkingDirection * normalizedWalkingTime * (stride * 2) + Vector(a, THREE_DIMENSION));//<------!(stride-100)


    return PlanStatus::RUN;
}

/**
 *	ロボットの瞬間の状態を生成する
 *		指令値生成周期ごとに呼び出すことによりロボットの連続な動作を計画する
 */
PlanStatus TripodGaitPlanner::createPlanSnapshot()
{
    /// ローカル変数の宣言
    PlanStatus plan = PlanStatus::RUN;

    /// 歩行が開始されてなかったら終了
    if (!isWalkingStarted)
    {
        /// 動作を中断している時は歩行も中断
        if (isSuspended)
        {
            return PlanStatus::SUSPEND;
        }

        //20201016
        if (isSetting)
        {
            settingPlan();
        }

        return PlanStatus::WAIT;

    }

    /// 計画経過時間を計算
    Planner::elapsed_time = timeManagerPointer->getRealTime() - Planner::planStartTime;

    /// 歩行周期開始から現在時刻までの歩行経過時間を計算
    walkingTime = timeManagerPointer->getRealTime() - cycleStartTime;

    /// 正規化歩行時間を計算
    normalizedWalkingTime = walkingTime / cycleTime;


    /// 1周期終了時の時間計算
    if (normalizedWalkingTime > 1.0)
    {
        /// 歩行カウンタを増やす
        walkingCounter++;

        /// 歩行周期開始時間を更新
        cycleStartTime = timeManagerPointer->getRealTime();

        //20201020

        /// 胴体の絶対座標を更新
        initialBodyPosition = asuraPointer->getBodyPosition();

        /// 歩行時間と正規化歩行時間をリセット
        walkingTime = 0.0;
        normalizedWalkingTime = 0.0;

        //20201005
        //stopPlan();  //1周期でやめるときはコメントアウトを解除する
        if (isWaitingToStop)
        {
            stopPlan();
            isWaitingToStop = false;
            isWalkingToGetSet = true;
        }
    }

    /// ロボットの歩行周期に合わせた脚運動を生成する
    plan = activateRobot();

    if (plan == PlanStatus::SUSPEND)
    {
        stopPlan();
    }

    /// 安定余裕を計算
    //calculateStabilityMargin();  20200819

    return plan;
}

//20201016
bool TripodGaitPlanner::settingPlan()
{
    //20201017
    //経過時間を得る
    settingTime = timeManagerPointer->getRealTime() - settingStartTime;

    Kinematics kine = NO_KINE_ERROR;

    //関節角度を算出する
    for (int i = 0; i < LEG_NUM; i++)
    {//１関節ごと
        if (settingTime < 0)  //どの関節も動く前
        {
            settingJointAngle[i][0] = initialJointAngle[i][0];
            settingJointAngle[i][1] = initialJointAngle[i][1];
            settingJointAngle[i][2] = initialJointAngle[i][2];
            settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (0 <= settingTime && settingTime <= t01[i])  //第1関節が駆動中
        {
            settingJointAngle[i][0] = angularVelosity[i][0] * settingTime + initialJointAngle[i][0];
            settingJointAngle[i][1] = initialJointAngle[i][1];
            settingJointAngle[i][2] = initialJointAngle[i][2];
            settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (t01[i] <= settingTime && settingTime <= t02[i])  //第2関節が駆動中
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = angularVelosity[i][1] * (settingTime - t01[i]) + initialJointAngle[i][1];
            settingJointAngle[i][2] = initialJointAngle[i][2];
            settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (t02[i] <= settingTime && settingTime <= t03[i])  //第3関節が駆動中
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = finalJointAngle[i][1];
            settingJointAngle[i][2] = angularVelosity[i][2] * (settingTime - t02[i]) + initialJointAngle[i][2];
            settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (t03[i] <= settingTime && settingTime <= t04[i])  //第4関節が駆動中
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = finalJointAngle[i][1];
            settingJointAngle[i][2] = finalJointAngle[i][2];
            settingFootJointAngle[i] = angularVelosity[i][3] * (settingTime - t03[i]) + initialFootJointAngle[i];
        }
        else if (t04[i] <= settingTime)
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = finalJointAngle[i][1];
            settingJointAngle[i][2] = finalJointAngle[i][2];
            settingFootJointAngle[i] = finalFootJointAngle[i];
        }


        //全部
        /*
        if (settingTime < 0)  //どの関節も動く前
        {
          settingJointAngle[i][0] = initialJointAngle[i][0];
          settingJointAngle[i][1] = initialJointAngle[i][1];
          settingJointAngle[i][2] = initialJointAngle[i][2];
          settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (0 <= settingTime)
        {

          if (t01[i] <= settingTime)
          {
            settingJointAngle[i][0] = finalJointAngle[i][0];
          }
          else
          {
            settingJointAngle[i][0] = angularVelosity[i][0] * settingTime + initialJointAngle[i][0];
          }

          if (t02[i] <= settingTime)
          {
            settingJointAngle[i][1] = finalJointAngle[i][1];
          }
          else
          {
            settingJointAngle[i][1] = angularVelosity[i][1] * settingTime + initialJointAngle[i][1];
          }

          if (t03[i] <= settingTime)
          {
            settingJointAngle[i][2] = finalJointAngle[i][2];
          }
          else
          {
            settingJointAngle[i][2] = angularVelosity[i][2] * settingTime + initialJointAngle[i][2];
          }

          if (t04[i] <= settingTime)
          {
            settingFootJointAngle[i] = finalFootJointAngle[i];
          }
          else
          {
            settingFootJointAngle[i] = angularVelosity[i][3] * settingTime + initialFootJointAngle[i];
          }
        }
        */
    }

    //すべての関節が移行し終わったらisSettingをfalseにする
    /*
    if (t04[0] < settingTime && t04[1] < settingTime && t04[2] < settingTime && t04[3] < settingTime && t04[4] < settingTime && t04[5] < settingTime)
    {
      isSetting = false;
      settingOver();  //20201022
    }
    */

    if (t05[0] < settingTime && t05[1] < settingTime && t05[2] < settingTime && t05[3] < settingTime && t05[4] < settingTime && t05[5] < settingTime)
    {
        isSetting = false;
        //settingOver();  //20201022
    }

    //関節角度やそこから算出した脚先位置などを記録する
    //
    /*
    for (int i = 0; i < LEG_NUM; i++)
    {
      kine = asuraPointer->placeLegJointAngles(i+1, settingJointAngle[i], settingFootJointAngle[i]);

    }
    */

    //脚2のみ
    kine = asuraPointer->placeLegJointAngles(2, settingJointAngle[1], settingFootJointAngle[1]);
    if (kine != NO_KINE_ERROR)
    {
        isSetting = false;
    }
    return true;
}


/**
 *	------------------------------------------------------------
 *		TripodGaitPlannerクラスの privateなメンバ関数
 *	------------------------------------------------------------
 */
 /// 歩行のためのオブジェクト生成
void TripodGaitPlanner::newTripodGaitItems(void)
{
    /// 脚基準位置
    footReferencePosition = new Vector[LEG_NUM];

    /// 足の初期位置
    initialFootPosition = new Vector[LEG_NUM];

    /// 遊脚開始位置
    swingStartPosition = new Vector[LEG_NUM];

    /// 遊脚終了位置
    swingStopPosition = new Vector[LEG_NUM];

    /// ベクトルのサイズ決定
    initialBodyPosition.setSize(THREE_DIMENSION);

    unitWalkingDirection.setSize(THREE_DIMENSION);
    unitUpDirection.setSize(THREE_DIMENSION);  //20200929
    swingUp.setSize(THREE_DIMENSION);
    swingDown.setSize(THREE_DIMENSION);

    /// 脚位置関係
    int i;
    for (i = 0; i < LEG_NUM; i++)
    {
        footReferencePosition[i].setSize(THREE_DIMENSION);
        initialFootPosition[i].setSize(THREE_DIMENSION);
        swingStartPosition[i].setSize(THREE_DIMENSION);
        swingStopPosition[i].setSize(THREE_DIMENSION);
    }

    /// 遊脚開始時間
    swingStartTime = new double[LEG_NUM];
    /// 遊脚終了時間
    swingStopTime = new double[LEG_NUM];

    //20201017  姿勢移行
    initialJointAngle = new Vector[LEG_NUM];
    finalJointAngle = new Vector[LEG_NUM];
    settingJointAngle = new Vector[LEG_NUM];
    initialFootJointAngle = new double[LEG_NUM];
    finalFootJointAngle = new double[LEG_NUM];
    settingFootJointAngle = new double[LEG_NUM];
    angularVelosity = new Vector[LEG_NUM];
    t01 = new double[LEG_NUM];
    t02 = new double[LEG_NUM];
    t03 = new double[LEG_NUM];
    t04 = new double[LEG_NUM];
    t05 = new double[LEG_NUM];

    for (int i = 0; i < LEG_NUM; i++)
    {
        initialJointAngle[i].setSize(LEG_JOINT_NUM);
        finalJointAngle[i].setSize(LEG_JOINT_NUM);
        settingJointAngle[i].setSize(LEG_JOINT_NUM);
        initialFootJointAngle[i] = 0;
        finalFootJointAngle[i] = 0;
        settingFootJointAngle[i] = 0;
        angularVelosity[i].setSize(LEG_JOINT_NUM + 1);
        t01[i] = 0;
        t02[i] = 0;
        t03[i] = 0;
        t04[i] = 0;
        t05[i] = 0;
    }


    return;

}

/// 歩行のためのオブジェクト消去
void TripodGaitPlanner::deleteTripodGaitItems(void)
{
    delete[] footReferencePosition;
    delete[] initialFootPosition;
    delete[] swingStartPosition;
    delete[] swingStopPosition;

    delete[] swingStartTime;
    delete[] swingStopTime;

    //20201017
    delete[] initialJointAngle;
    delete[] finalJointAngle;
    delete[] initialFootJointAngle;
    delete[] finalFootJointAngle;
    delete[] angularVelosity;
    delete[] t01;
    delete[] t02;
    delete[] t03;
    delete[] t04;
    delete[] t05;

    return;
}

}	/// end of namespace Plan

#include "Plan/tripod_gait_planner.h"

#include "Math/math_constant.h"


namespace designlab_robot_gui::plan
{

TripodGaitPlanner::TripodGaitPlanner(AsuraX* asuraPointer_, TimeManager* timeManagerPointer_)
    : Planner(asuraPointer_, timeManagerPointer_)
{
    /// オブジェクトの生成
    newTripodGaitItems();

    /// 初期化
    initializeTripodGaitPlanner();

}

TripodGaitPlanner::~TripodGaitPlanner()
{
    /// オブジェクトの破棄
    deleteTripodGaitItems();

}

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
    unitWalkingDirection = Vector(DEFAULT_LOCOMOTION_DIRECTION, math::THREE_DIMENSION);

    //20200929  支持脚時に使用するz軸方向のセット
    unitUpDirection = Vector(UP_DIRECTION, math::THREE_DIMENSION);

    /// 位置ベクトルの初期化
    footReferencePosition[0] = Vector(TRIPODGAIT_FOOT_REF_POSITION_1, math::THREE_DIMENSION);
    footReferencePosition[1] = Vector(TRIPODGAIT_FOOT_REF_POSITION_2, math::THREE_DIMENSION);
    footReferencePosition[2] = Vector(TRIPODGAIT_FOOT_REF_POSITION_3, math::THREE_DIMENSION);
    footReferencePosition[3] = Vector(TRIPODGAIT_FOOT_REF_POSITION_4, math::THREE_DIMENSION);
    footReferencePosition[4] = Vector(TRIPODGAIT_FOOT_REF_POSITION_5, math::THREE_DIMENSION);
    footReferencePosition[5] = Vector(TRIPODGAIT_FOOT_REF_POSITION_6, math::THREE_DIMENSION);
    /// 遊脚運動
    swingUp = Vector(TRIPODGAIT_SWING_UP, math::THREE_DIMENSION);
    swingDown = Vector(TRIPODGAIT_SWING_DOWN, math::THREE_DIMENSION);

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
    swingStartTime[0] = 0;
    swingStartTime[1] = dutyFactor;
    swingStartTime[2] = 0;
    swingStartTime[3] = dutyFactor;
    swingStartTime[4] = 0;
    swingStartTime[5] = dutyFactor;


    /// 遊脚時間をセットする
    int i;
    for (i = 0; i < asura::LEG_NUM; i++)
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
    for (j = 0; j < asura::LEG_NUM; j++)
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
    for (k = 0; k < asura::LEG_NUM; k++)
    {
        setLegSwingTrajectory(k + 1,
                    swingStartPosition[k],
                    swingUp,
                    (stride)*unitWalkingDirection, //<------!(stride-100)
                    swingDown
        );
    }

    if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] > 0)
    {
        initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
        initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
    }
    else if (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2] < 0)
    {
        initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
        initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection + (TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) * unitUpDirection;
    }
    else
    {
        initialFootPosition[0] = footReferencePosition[0] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[1] = footReferencePosition[1] + (stride) / 2 * unitWalkingDirection;
        initialFootPosition[2] = footReferencePosition[2] - (stride) / 2 * unitWalkingDirection;
        initialFootPosition[3] = footReferencePosition[3] + (stride) / 2 * unitWalkingDirection;
    }


    /// 胴体の初期位置
    for (int l = 0; l < math::THREE_DIMENSION; l++)
    {
        initialBodyPosition(l + 1) = TRIPODGAIT_INITIAL_BODY_POSITION[l];
    }


    isTrajectoryToGetSet = false;

    return;
}

bool TripodGaitPlanner::shiftToInitialStandingPosture()
{
    asura::Kinematics kine = asura::NO_KINE_ERROR;

    /// 重心を初期位置に
    asuraPointer->initializeBodyPosition(initialBodyPosition);

    /// 足位置を初期姿勢に
    isSetting = true;  //姿勢移行のフラグ

    //20201017
    //各関節角度取得
    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        for (int j = 0; j < asura::LEG_JOINT_NUM; j++)
        {
            initialJointAngle[i][j] = asuraPointer->getLegJointAngle(i + 1)(j + 1);
        }
        initialFootJointAngle[i] = asuraPointer->getFootJointAngle(i + 1);
    }
    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        kine = asuraPointer->placeLegFootPosition(i + 1, initialFootPosition[i]);
        for (int j = 0; j < asura::LEG_JOINT_NUM; j++)
        {
            finalJointAngle[i][j] = asuraPointer->getLegJointAngle(i + 1)(j + 1);
        }
        finalFootJointAngle[i] = asuraPointer->getFootJointAngle(i + 1);
    }

    //所要時間算出
    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        for (int j = 0; j < asura::LEG_JOINT_NUM; j++)
        {
            if (finalJointAngle[i][j] >= initialJointAngle[i][j])
            {
                angularVelocity[i][j] = ANGULAR_VELOCITY;
            }
            else
            {
                angularVelocity[i][j] = -1 * ANGULAR_VELOCITY;
            }
            if (finalFootJointAngle[i] >= initialFootJointAngle[i])
            {
                angularVelocity[i][asura::LEG_JOINT_NUM] = ANGULAR_VELOCITY;
            }
            else
            {
                angularVelocity[i][asura::LEG_JOINT_NUM] = -1 * ANGULAR_VELOCITY;
            }
        }

        t01[i] = (finalJointAngle[i][0] - initialJointAngle[i][0]) / angularVelocity[i][0];
        t02[i] = t01[i] + (finalJointAngle[i][1] - initialJointAngle[i][1]) / angularVelocity[i][1];
        t03[i] = t02[i] + (finalJointAngle[i][2] - initialJointAngle[i][2]) / angularVelocity[i][2];
        t04[i] = t03[i] + (finalFootJointAngle[i] - initialFootJointAngle[i]) / angularVelocity[i][3];

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

bool TripodGaitPlanner::setup(void)
{
    /// ポインタがセットされているかの確認
    if (asuraPointer == NULL || timeManagerPointer == NULL)
    {
        std::cerr << "[TripodGaitPlanner::setup] No control object\n" << std::endl;

        return false;
    }


    if (isTrajectoryToGetSet)
    {
        std::cerr << "[TripodGaitPlanner::setup] No trajectories are ready\n" << std::endl;

        return false;
    }

    if (!shiftToInitialStandingPosture())
    {
        std::cerr << "[TripodGaitPlanner::setup] Cannot shift to initial posture\n" << std::endl;

        return false;
    }

    /// 歩行準備完了
    isWalkingToGetSet = false;

    cycleElapsedTime = 0.0;


    return Planner::setup();
}


/// 歩行を開始する
bool TripodGaitPlanner::startPlan()
{
    isRunning = true;  //20201020

    if (isWalkingToGetSet || !isRunning)
    {
        std::cerr << "[TripodGaitPlanner::startPlan] Not stand-by for walking\n" << std::endl;

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

bool TripodGaitPlanner::standByForStop(void)
{
    isWaitingToStop = true;

    return isWaitingToStop;
}

PlanStatus TripodGaitPlanner::activateRobot(void)
{
    /**
     *		正規化した歩行時間により脚の運動を決定する
     */
     /// 動作状態
    PlanStatus plan = plan::PlanStatus::WAIT;
    /// 逆運動学を解いた結果
    asura::Kinematics kinematics = asura::NO_KINE_ERROR;
    /// カウンタ
    int i;

    double swingTime = (1 + TRIPODGAIT_SWING_DOWN[2] / TRIPODGAIT_SWING_UP[2]) / 2;



    ///1,3,5遊脚　2,4,6支持
    if (0.00 <= normalizedWalkingTime && normalizedWalkingTime < TRIPODGAIT_DUTY_FACTOR)
    {
        for (i = 0; i < asura::LEG_NUM; i++)
        {
            switch (i)
            {
                case 1:
                case 3:
                case 5:
                {
                    ///第 2, 4, 6脚の支持脚運動

                    if (TRIPODGAIT_SWING_UP[2] == -TRIPODGAIT_SWING_DOWN[2])  //20200929  平面時の支持脚運動
                    {
                        kinematics = asuraPointer->placeLegFootPosition(
                          i + 1,
                          initialFootPosition[i] - normalizedWalkingTime * cycleTime * walkingSpeed * unitWalkingDirection
                        );
                    }
                    else  //上り下りでの支持脚運動
                    {
                        kinematics = asuraPointer->placeLegFootPosition(
                          i + 1,
                          initialFootPosition[i] - normalizedWalkingTime * cycleTime * walkingSpeed * unitWalkingDirection
                          + normalizedWalkingTime * cycleTime * -(TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) / (cycleTime * dutyFactor) * unitUpDirection
                        );
                    }

                    /// 支持脚相にセット
                    asuraPointer->setLegPhase(i + 1, asura::LegPhase::SUPPORT);

                    //if (kinematics != NO_KINE_ERROR)
                    if (kinematics != asura::NO_KINE_ERROR && i == 1)  //脚2についてのみ可動域など確認
                    {
                        std::cerr << "[TripodGaitPlanner::activateRobot]" << std::endl;
                        Planner::printPlanErrorMessage();

                        suspendPlan();
                        return PlanStatus::SUSPEND;
                    }

                    break;
                }
                case 0:
                case 2:
                case 4:
                {
                    // 第1,3,5 脚の遊脚運動

                    /// 前周期からの続きのため遊脚運動だけは時間補正で1.0を加える
                    plan = swingLeg(i + 1, normalizedWalkingTime);
                    /// 遊脚相にセット
                    asuraPointer->setLegPhase(i + 1, asura::LegPhase::SWING);

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
        for (i = 0; i < asura::LEG_NUM; i++)
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
                        kinematics = asuraPointer->placeLegFootPosition(
                          i + 1,
                          swingStopPosition[i] - (normalizedWalkingTime - swingStopTime[i]) * cycleTime * walkingSpeed * unitWalkingDirection
                        );
                    }
                    else  //上り下りでの支持脚運動
                    {
                        kinematics = asuraPointer->placeLegFootPosition(
                          i + 1,
                          swingStopPosition[i] - (normalizedWalkingTime - swingStopTime[i]) * cycleTime * walkingSpeed * unitWalkingDirection
                          + (normalizedWalkingTime - swingStopTime[i]) * cycleTime * -(TRIPODGAIT_SWING_UP[2] + TRIPODGAIT_SWING_DOWN[2]) / (cycleTime * dutyFactor) * unitUpDirection
                        );
                    }


                    /// 支持脚相にセット
                    asuraPointer->setLegPhase(i + 1, asura::LegPhase::SUPPORT);

                    //if (kinematics != NO_KINE_ERROR
                    if (kinematics != asura::NO_KINE_ERROR && i == 1)  //脚2についてのみ可動域など確認
                    {
                        std::cerr << "[TripodGaitPlanner::activateRobot]" << std::endl;
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
                    asuraPointer->setLegPhase(i + 1, asura::LegPhase::SWING);

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

bool TripodGaitPlanner::settingPlan(void)
{
    //20201017
    //経過時間を得る
    settingTime = timeManagerPointer->getRealTime() - settingStartTime;

    asura::Kinematics kine = asura::NO_KINE_ERROR;

    //関節角度を算出する
    for (int i = 0; i < asura::LEG_NUM; i++)
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
            settingJointAngle[i][0] = angularVelocity[i][0] * settingTime + initialJointAngle[i][0];
            settingJointAngle[i][1] = initialJointAngle[i][1];
            settingJointAngle[i][2] = initialJointAngle[i][2];
            settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (t01[i] <= settingTime && settingTime <= t02[i])  //第2関節が駆動中
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = angularVelocity[i][1] * (settingTime - t01[i]) + initialJointAngle[i][1];
            settingJointAngle[i][2] = initialJointAngle[i][2];
            settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (t02[i] <= settingTime && settingTime <= t03[i])  //第3関節が駆動中
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = finalJointAngle[i][1];
            settingJointAngle[i][2] = angularVelocity[i][2] * (settingTime - t02[i]) + initialJointAngle[i][2];
            settingFootJointAngle[i] = initialFootJointAngle[i];
        }
        else if (t03[i] <= settingTime && settingTime <= t04[i])  //第4関節が駆動中
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = finalJointAngle[i][1];
            settingJointAngle[i][2] = finalJointAngle[i][2];
            settingFootJointAngle[i] = angularVelocity[i][3] * (settingTime - t03[i]) + initialFootJointAngle[i];
        }
        else if (t04[i] <= settingTime)
        {
            settingJointAngle[i][0] = finalJointAngle[i][0];
            settingJointAngle[i][1] = finalJointAngle[i][1];
            settingJointAngle[i][2] = finalJointAngle[i][2];
            settingFootJointAngle[i] = finalFootJointAngle[i];
        }
    }

    if (t05[0] < settingTime && t05[1] < settingTime && t05[2] < settingTime && t05[3] < settingTime && t05[4] < settingTime && t05[5] < settingTime)
    {
        isSetting = false;
        //settingOver();  //20201022
    }

    //脚2のみ
    kine = asuraPointer->placeLegJointAngles(2, settingJointAngle[1], settingFootJointAngle[1]);
    if (kine != asura::NO_KINE_ERROR)
    {
        isSetting = false;
    }
    return true;
}


void TripodGaitPlanner::newTripodGaitItems()
{
    /// 脚基準位置
    footReferencePosition = new Vector[asura::LEG_NUM];

    /// 足の初期位置
    initialFootPosition = new Vector[asura::LEG_NUM];

    /// 遊脚開始位置
    swingStartPosition = new Vector[asura::LEG_NUM];

    /// 遊脚終了位置
    swingStopPosition = new Vector[asura::LEG_NUM];

    /// ベクトルのサイズ決定
    initialBodyPosition.setSize(math::THREE_DIMENSION);

    unitWalkingDirection.setSize(math::THREE_DIMENSION);
    unitUpDirection.setSize(math::THREE_DIMENSION);
    swingUp.setSize(math::THREE_DIMENSION);
    swingDown.setSize(math::THREE_DIMENSION);

    /// 脚位置関係
    int i;
    for (i = 0; i < asura::LEG_NUM; i++)
    {
        footReferencePosition[i].setSize(math::THREE_DIMENSION);
        initialFootPosition[i].setSize(math::THREE_DIMENSION);
        swingStartPosition[i].setSize(math::THREE_DIMENSION);
        swingStopPosition[i].setSize(math::THREE_DIMENSION);
    }

    /// 遊脚開始時間
    swingStartTime = new double[asura::LEG_NUM];
    /// 遊脚終了時間
    swingStopTime = new double[asura::LEG_NUM];

    //20201017  姿勢移行
    initialJointAngle = new Vector[asura::LEG_NUM];
    finalJointAngle = new Vector[asura::LEG_NUM];
    settingJointAngle = new Vector[asura::LEG_NUM];
    initialFootJointAngle = new double[asura::LEG_NUM];
    finalFootJointAngle = new double[asura::LEG_NUM];
    settingFootJointAngle = new double[asura::LEG_NUM];
    angularVelocity = new Vector[asura::LEG_NUM];
    t01 = new double[asura::LEG_NUM];
    t02 = new double[asura::LEG_NUM];
    t03 = new double[asura::LEG_NUM];
    t04 = new double[asura::LEG_NUM];
    t05 = new double[asura::LEG_NUM];

    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        initialJointAngle[i].setSize(asura::LEG_JOINT_NUM);
        finalJointAngle[i].setSize(asura::LEG_JOINT_NUM);
        settingJointAngle[i].setSize(asura::LEG_JOINT_NUM);
        initialFootJointAngle[i] = 0;
        finalFootJointAngle[i] = 0;
        settingFootJointAngle[i] = 0;
        angularVelocity[i].setSize(asura::LEG_JOINT_NUM + 1);
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
    delete[] angularVelocity;
    delete[] t01;
    delete[] t02;
    delete[] t03;
    delete[] t04;
    delete[] t05;

    return;
}

}	/// end of namespace Plan
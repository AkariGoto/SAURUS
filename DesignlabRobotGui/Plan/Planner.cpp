
#include "Planner.h"
#include "Math/MathLibrary.h"
#include "Math/Matrix/Matrix.h"


namespace designlab_robot_gui::plan
{

Planner::Planner(AsuraX* asuraPointer_, TimeManager* timeManagerPointer_)
{
    /// ポインタのセット
    asuraPointer = asuraPointer_;
    timeManagerPointer = timeManagerPointer_;

    /// 軌道要素を生成
    newTrajectories();

    /// メンバ変数の初期化
    isRunning = false;
    isSuspended = false;

    //stabilityMargin = 0.0;  20200819
    elapsed_time = 0.0;
    planStartTime = 0.0;
}

Planner::~Planner()
{
    /// ヌルポインタをセット
    asuraPointer = NULL;

    /// 軌道要素を消去
    deleteTrajectories();
}

void Planner::acquireAsura(AsuraX* ptr)
{
    /// 現在のポインタをリセット
    releaseAsura();

    asuraPointer = ptr;

    return;
}

void Planner::releaseAsura()
{
    asuraPointer = NULL;

    return;
}

void Planner::acquireTimeManager(TimeManager* ptr)
{
    /// 現在のポインタをリセット
    releaseTimeManager();

    timeManagerPointer = ptr;

    return;
}

void Planner::releaseTimeManager()
{
    timeManagerPointer = NULL;

    return;
}

void Planner::setLegSwingTrajectory(
    int legNo,
    const Vector& start,
    const Vector& upPhase,
    const Vector& returnPhase,
    const Vector& downPhase
)
{
    /// 脚の引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    /// 軌道要素をセット
    legTrajectory[legNo - 1].setLegTrajectory(start, upPhase, returnPhase, downPhase);

    return;
}

void Planner::setLegSwingTime(int legNo,
                              double start,
                              double upPhase,
                              double returnPhase,
                              double downPhase)
{
    // 脚の引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    // 軌道要素をセット
    legTrajectory[legNo - 1].setLegTrajectoryTime(start, upPhase, returnPhase, downPhase);
}

Math::Vector Planner::getLegSwingPosition(int legNo, double splitTime)
{
    /// 脚の引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return legTrajectory[legNo - 1].getPosition(splitTime);
}

PlanStatus Planner::swingLeg(int legNo, double splitTime)
{
    // 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    // 運動学の結果
    asura::Kinematics kine;

    // エラーを起こした関節番号
    int errorJointNo = 0;

    // 動作停止中だったら遊脚しない
    if (isSuspended)
    {
        return PlanStatus::SUSPEND;
    }

    // 遊脚時間内かどうかのチェック
    if (legTrajectory[legNo - 1].getStartTime() < splitTime &&
        splitTime < legTrajectory[legNo - 1].getGoalTime())
    {
        // 引数の時間での軌道上の位置を取得
        // PTP制御により，足を目標位置にセット
        kine = asuraPointer->placeLegFootPosition(legNo,
                                                  legTrajectory[legNo - 1].getPosition(splitTime));

        // 逆運動学が解けたかどうかのチェック
        if (kine != asura::NO_KINE_ERROR)
        {
            printPlanErrorMessage();
            return PlanStatus::INVALID;
        }

        // 脚の状態を遊脚にセット
        asuraPointer->setLegPhase(legNo, asura::LegPhase::SWING);
    }
    else
    {
        // 引数の時間での軌道上の位置を取得
        // PTP制御により，足を目標位置にセット
        kine = asuraPointer->placeLegFootPosition(legNo,
                                                  legTrajectory[legNo - 1].getPosition(splitTime));

        // 逆運動学が解けたかどうかのチェック
        if (kine != asura::NO_KINE_ERROR)
        {
            printPlanErrorMessage();
            return PlanStatus::INVALID;
        }

        // 脚の状態を支持脚にセット
        asuraPointer->setLegPhase(legNo, asura::LegPhase::SUPPORT);

        // 動作終了
        return PlanStatus::END;
    }

    // 動作実行中
    return PlanStatus::RUN;
}

PlanStatus Planner::moveBody(double splitTime)
{
    // 動作停止中だったら遊脚しない
    if (isSuspended)
    {
        return PlanStatus::SUSPEND;
    }

    // 軌道上の目標脚先位置
    Vector targetPosition(Const::THREE_DIMENSION);
    /// 運動学の結果
    asura::Kinematics kine;
    /// エラーを起こした脚番号
    int errorLegNo = 0;
    /// エラーを起こした関節番号
    int errorJointNo = 0;

    /// 引数の時間での軌道上の位置を計算
    targetPosition = bodyTrajectory->getPosition(splitTime);

    /// PTP制御により重心を目標位置にセット
    kine = asuraPointer->placeBodyPosition(targetPosition);

    /// 逆運動が解けるかどうかのチェック
    if (kine != asura::NO_KINE_ERROR)
    {
        Planner::printPlanErrorMessage();

        return PlanStatus::INVALID;
    }

    /// 胴体推進時間内にあるかどうかチェック
    if (!(bodyTrajectory->getStartTime() < splitTime) || !(splitTime < bodyTrajectory->getGoalTime()))
    {
        return PlanStatus::END;
    }

    return PlanStatus::RUN;


}

void Planner::printPlanErrorMessage()
{
    asura::Kinematics kine = asuraPointer->getLastKineError();
    int legNo = asuraPointer->getLastErrorTrackLegNo();
    int jointNo = asuraPointer->getLegLastErrorJointNo(legNo);
    char error[256] = "No error";

    switch (kine)
    {
        case asura::KINE_ERROR_UNREACHABLE:
            sprintf_s(error, "Unreachable operation");
            break;

        case asura::KINE_ERROR_SINGULAR:
            sprintf_s(error, "Singular posture");
            break;

        case asura::KINE_ERROR_ELSE:
            sprintf_s(error, "Uncertain error");
            break;

        case asura::KINE_ERROR_REACH_RADIUS_UNDER:
            sprintf_s(error, "Under minimum horizontal reach");
            break;

        case asura::KINE_ERROR_REACH_RADIUS_OVER:
            sprintf_s(error, "Over maximum horizontal reach");
            break;

        case asura::KINE_ERROR_REACH_HEIGHT_UNDER:
            sprintf_s(error, "Under minimum vertical reach");
            break;

        case asura::KINE_ERROR_REACH_HEIGHT_OVER:
            sprintf_s(error, "Under maximum vertical reach");
            break;

        case asura::KINE_ERROR_JOINT_UNDER_LIMIT:
            sprintf_s(error, "Under minimum joint angle");
            break;

        case asura::KINE_ERROR_JOINT_OVER_LIMIT:
            sprintf_s(error, "Over maximum joint angle");
            break;

        case asura::KINE_ERROR_JOINT_VELOCITY_LIMIT:
            sprintf_s(error, "Over maximum joint angular velocity");
            break;

        case asura::KINE_ERROR_JOINT_TORQUE_LIMIT:
            sprintf_s(error, "Over maximum joint torque velocity");
            break;
    }

    std::cout << " Joint-" << jointNo << " of " << "Leg-" << legNo << ":"
        << error << std::endl;
}

bool Planner::setup()
{
    elapsed_time = 0.0;
    stopElapsedTime = elapsed_time;

    // 動作開始
    isRunning = true;

    return true;
}

bool Planner::startPlan()
{
    // プランニング開始時間を設定
    // elapsedTimeは起動時は0
    planStartTime = timeManagerPointer->getRealTime() - stopElapsedTime;

    return true;
}

bool Planner::stopPlan()
{
    stopElapsedTime = elapsed_time;

    return true;
}

bool Planner::standByForStop()
{
    return true;
}

void Planner::newTrajectories()
{
    legTrajectory = new LegTrajectory[asura::LEG_NUM];
    bodyTrajectory = new BodyTrajectory;
}

void Planner::deleteTrajectories()
{
    delete[] legTrajectory;
    delete bodyTrajectory;
}


}  // namespace designlab_robot_gui::plan

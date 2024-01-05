
#ifndef DESIGNLABROBOTGUI_PLAN_TRIPOD_GAIT_PLANNER_H_
#define DESIGNLABROBOTGUI_PLAN_TRIPOD_GAIT_PLANNER_H_

#include "Kinematics/asura_x.h"
#include "Math/MathLibrary.h"
#include "Plan/plan_parameter.h"
#include "Plan/planner.h"


namespace designlab_robot_gui::plan
{

class TripodGaitPlanner final : public Planner
{
    using AsuraX = asura::AsuraX;
    using Vector = Math::Vector;

public:
    explicit TripodGaitPlanner(asura::AsuraX* asuraPointer_ = nullptr,
                               TimeManager* timeManagerPointer_ = nullptr);
    TripodGaitPlanner(const TripodGaitPlanner& tripodtGaitPlanner) = delete;
    TripodGaitPlanner& operator=(const TripodGaitPlanner& tripodGaitPlanner) = delete;

    virtual ~TripodGaitPlanner();


    //! 基準歩容の初期化
    void initializeTripodGaitPlanner();

    //! 歩容パラメータの計算
    void calculateGaitParameters();

    //! 歩行初期姿勢に移行
    bool shiftToInitialStandingPosture();

    //! 歩行開始のための初期化
    virtual bool setup();

    //! 歩行を開始する
    virtual bool startPlan();

    //! 歩行を即座に停止する
    virtual bool stopPlan();

    //! 歩行を1歩終了時に停止する
    virtual bool standByForStop();

    //! 脚運動を生成する
    virtual PlanStatus activateRobot();

    //! ロボットの瞬間の状態を生成する
    //! 指令値生成周期ごとに呼び出すことによりロボットの連続な動作を計画する
    virtual PlanStatus createPlanSnapshot();

    //! 姿勢の移行
    virtual bool settingPlan();

    //! 歩行を開始したかどうか
    constexpr bool isWalking() const { return isWalkingStarted; }

private:
    // 歩行のためのオブジェクト生成
    void newTripodGaitItems();
    // 歩行のためのオブジェクト消去
    void deleteTripodGaitItems();


    // 周期歩容に関係するもの

    double cycleTime;  //!< 歩行周期．単位：秒

    double dutyFactor;  //!< デューティ比，1歩行周期中における，ある脚の支持脚期間の比率

    //! ストライド．1周期中に胴体が移動する距離．
    //! ストロークはデューティ比 x ストライド
    double stride;

    double walkingSpeed;  //!< 歩行速度．

    int walkingCounter;  //!< 歩行回数のカウンタ

    Vector unitWalkingDirection;  //!< 歩行方向の単位ベクトル

    Vector unitUpDirection;  //!< 支持脚時に使用するz軸方向の単位ベクトル

    Vector* footReferencePosition;  //!< 脚基準位置．脚の遊脚運動軌跡の中点位置

    Vector initialBodyPosition;  //!< 胴体の初期位置

    Vector* initialFootPosition;  //!< 足の初期位置

    Vector* swingStartPosition;  //!< 遊脚開始位置

    Vector* swingStopPosition;  //!< 遊脚終了位置

    Vector swingUp;  //!< 遊脚振り上げ

    Vector swingDown;  //!< 遊脚振り下げ

    double walkingTime;  //!< 歩行時間（0 < t < 周期）

    double cycleStartTime;  //!< 歩行周期開始時間

    double normalizedWalkingTime;  //!< 正規化した歩行時間（0 < t < 1）

    // 遊脚開始時間
    double* swingStartTime;
    double* swingStopTime;

    // 停止時の歩行周期の経過時間
    double cycleElapsedTime;

    bool isWalkingToGetSet;  //!< 歩行開始のための準備をする必要があるかどうかのフラグ

    bool isTrajectoryToGetSet;  //!< 脚軌道が決定されているかどうかのフラグ

    bool isWalkingStarted;  //!< 歩行開始のフラグ

    bool isWaitingToStop;  //!< 歩行停止待機フラグ

    bool isSetting;  //!< 姿勢移行中かどうかのフラグ

    Vector* initialJointAngle;  //!< 移行前の関節角度
    Vector* finalJointAngle;  //!< 移行後の関節角度
    Vector* settingJointAngle;  //!< 移行中の関節角度

    //[i][j]で(i+1)番目の脚の第(j+1)関節の角度を示す

    double* initialFootJointAngle;  //!< 移行前の関節角度
    double* finalFootJointAngle;  //!< 移行後の関節角度
    double* settingFootJointAngle;  //!< 移行中の関節角度

    Math::Vector* angularVelosity;  //!< 姿勢移行中の角速度

    //! それぞれの関節を動かし終わるまでの所要時間
    double* t01;
    double* t02;
    double* t03;
    double* t04;
    double* t05;

    double settingStartTime;  //!< 姿勢移行開始時間
    double settingTime;  //!< 姿勢移行開始からの経過時間

    int swingLegWaitingToStop;  //!< 歩行停止直前の遊脚
};

}  // namespace designlab_robot_gui::plan


#endif  // DESIGNLABROBOTGUI_PLAN_TRIPOD_GAIT_PLANNER_H_

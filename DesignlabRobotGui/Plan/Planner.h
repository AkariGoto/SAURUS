
#ifndef DESIGNLABROBOTGUI_PLAN_PLANNER_H_
#define DESIGNLABROBOTGUI_PLAN_PLANNER_H_

#include <stdio.h>

#include "Math/Matrix/matrix.h"
#include "Kinematics/asura_x.h"
#include "Plan/plan_parameter.h"
#include "Plan/time_manager.h"
#include "Trajectory/leg_trajectory.h"
#include "Trajectory/body_trajectory.h"


namespace designlab_robot_gui::plan
{

class Planner
{
    using AsuraX = asura::AsuraX;
    using Vector = math::Vector;

protected:
    asura::AsuraX* asuraPointer;  //!< 制御するロボットオブジェクトへのポインタ

    TimeManager* timeManagerPointer;  //!< 時間を取得するための時間管理オブジェクトへのポインタ

    LegTrajectory* legTrajectory;  //!< 軌道オブジェクト．脚軌道

    BodyTrajectory* bodyTrajectory;  //!< 軌道オブジェクト．胴体軌道


    double elapsed_time;  //!< 行動開始からの経過時間

    double planStartTime;  //!< プランニング開始時間

    double stopElapsedTime;  //!< 停止時の経過時間

    bool isRunning;  //!< 動作制御のためのフラグ
    bool isSuspended;  //!< 動作停止のためのフラグ

public:
    Planner(AsuraX* asuraPointer_ = nullptr,
            TimeManager* timeManagerPointer_ = nullptr);
    Planner(const Planner& planner) = delete;
    Planner& operator=(const Planner& planner) = delete;

    /// デストラクタ
    virtual ~Planner();

    /// 制御する歩行ロボットへのポインタを取得する
    void acquireAsura(AsuraX* ptr);
    /// 制御する歩行ロボットへのポインタを解放する
    void releaseAsura();

    /// 使用する時間管理オブジェクトへのポインタを取得する
    void acquireTimeManager(TimeManager* ptr);
    /// 使用する時間管理オブジェクトへのポインタを解放する
    void releaseTimeManager();


    /// 計画開始からの経過時間
    double getElapsedTime() const { return elapsed_time; }
    /// 開始時間
    double getPlanStartTime() const { return planStartTime; }

    /**
     *		脚軌道そのものを得る
     */
    LegTrajectory& getLegTrajectory(int legNo)
    {
        assert(1 <= legNo && legNo <= designlab_robot_gui::asura::LEG_NUM);

        return legTrajectory[legNo - 1];
    }

    /// constオブジェクトに対する添え字演算子
    LegTrajectory& getLegTrajectory(int legNo) const
    {
        assert(1 <= legNo && legNo <= designlab_robot_gui::asura::LEG_NUM);

        return legTrajectory[legNo - 1];
    }

    /// 経過時間
    void setElapsedTime(double time) { elapsed_time = time; return; }
    /// 開始時間
    void setPlanStartTime(double time) { planStartTime = time; return; }


    /**
     *		遊脚軌道を設定する
     *			legNo：遊脚
     *			upPhase：UP相
     *			returnPhase：復帰相
     *			downPhase：DOWN相
     */
    void setLegSwingTrajectory(int legNo,
                        const math::Vector& start,
                        const math::Vector& upPhase,
                        const math::Vector& returnPhase,
                        const math::Vector& downPhase);

    /**
     *		遊脚時間を設定する
     *			legNo：遊脚
     *			upPhase：UP相
     *			returnPhase：復帰相
     *			downPhase：DOWN相
     */
    void setLegSwingTime(int legNo,
                    double start,
                    double upPhase,
                    double returnPhase,
                    double downPhase
    );
    /**
     *		遊脚の軌道上の位置を取得する
     */
    math::Vector getLegSwingPosition(int legNo, double splitTime);

    /**
     *		遊脚する
     */
    PlanStatus swingLeg(int legNo, double splitTime);

    /**
     *		胴体推進
    */
    PlanStatus moveBody(double splitTime);

    /**
     *		動作停止
     */
    void suspendPlan() { isSuspended = true; }

    /**
     *		動作再開
     */
    void resumePlan() { isSuspended = false; }

    /**
     *		フラグの判定
     */
    bool isActive() const { return isRunning; }

    /**
     *		エラーメッセージの出力
     */
    void printPlanErrorMessage();

    /**
     *	----------------------------------------
     *	運動を具体的に生成する関数群
     *	----------------------------------------
     */
     /// 歩行開始のための初期化
    virtual bool setup(void);

    /// 歩行を開始する
    virtual bool startPlan(void);

    /// 歩行を停止する
    virtual bool stopPlan(void);

    /// 歩行停止の待機状態にする
    virtual bool standByForStop(void);

    /// ロボットを駆動する
    virtual PlanStatus activateRobot(void) = 0;

    /// ロボットの瞬間の状態を生成する
    /// 指令値生成周期ごとに呼び出すことによりロボットの連続な動作を計画する
    virtual PlanStatus createPlanSnapshot(void) = 0;

private:


    /**
     *		軌道オブジェクト
     */
     /// 軌道オブジェクトの生成
    void newTrajectories();
    /// 軌道オブジェクトの破棄
    void deleteTrajectories();
};

}  // namespace designlab_robot_gui::plan


#endif  // DESIGNLABROBOTGUI_PLAN_PLANNER_H_

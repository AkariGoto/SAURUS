
#ifndef DESIGNLABROBOTGUI_PLAN_PLANNER_MANAGER_H_
#define DESIGNLABROBOTGUI_PLAN_PLANNER_MANAGER_H_

#include "Plan/plan_parameter.h"
#include "Plan/planner.h"


namespace designlab_robot_gui::plan
{

class PlannerManager
{
public:
    /// デフォルトコンストラクタ
    PlannerManager();
    /// デストラクタ
    virtual ~PlannerManager();

    /**
     *	目的の動作を切替
     */
    bool switchPlan(Planner* planner, Strategy strategy);

    Strategy getCurrentStrategy() const { return currentStrategy; }
    Strategy getLastStrategy() const { return lastStrategy; }

    /**
     *	実行中かどうか
     */
    bool isActive();

    /**
     *	運動を具体的に生成する関数群
     */
     /// 計画開始のための初期化
    bool setup();

    /// 計画を開始する
    bool startMotion();

    /// 計画を停止する
    bool stopMotion();

    /// 停止の待機状態にする
    bool standByForStop();

    /// 運動を生成する
    PlanStatus activateRobot();

    /// ロボットの瞬間の状態を生成する
    PlanStatus createSnapshot();

private:
    Planner* plannerPointer;  //!< 目的の動作計画へのポインタ

    Strategy lastStrategy;  //!< 前回の計画
    Strategy currentStrategy;  //!< 選択した計画
};

}  // namespace designlab_robot_gui::plan


#endif  // DESIGNLABROBOTGUI_PLAN_PLANNER_MANAGER_H_


#ifndef __Planner_h__
#define __Planner_h__

#include<stdio.h>

#include "Math/Matrix/Matrix.h"
#include "Kinematics/asura_x.h"

#include "Plan/plan_parameter.h"
#include "TimeManager.h"

#include "Trajectory/LegTrajectory.h"
#include "Trajectory/BodyTrajectory.h"


namespace Plan
{

class Planner
{
protected:
    /**
     *		制御するロボットオブジェクトへのポインタ
     */
    Asura::AsuraX* asuraPointer;

    /**
     *		時間を取得するための時間管理オブジェクトへのポインタ
     */
    TimeManager* timeManagerPointer;

    /**
     *		軌道オブジェクト
     */
     /// 脚軌道
    LegTrajectory* legTrajectory;
    /// 胴体軌道
    BodyTrajectory* bodyTrajectory;

    /**
     *		計画に用いる変数
     */
     /// 安定余裕
     //double stabilityMargin;  20200819
     /// 行動開始からの経過時間
    double elapsed_time;
    /// プランニング開始時間
    double planStartTime;

    //20201020  停止時の経過時間
    double stopElapsedTime;
    /**
     *		動作制御のためのフラグ
     */
    bool isRunning;
    bool isSuspended;


private:

    /**
     *	------------------------------------------------------------
     *		メンバ関数
     *	------------------------------------------------------------
     */
public:
    /**
     *	----------------------------------------
     *	コンストラクタとデストラクタ
     *	----------------------------------------
     */
     /// デフォルトコンストラクタ
    explicit Planner(Asura::AsuraX* asuraPointer_ = NULL, TimeManager* timeManagerPointer_ = NULL);

    /// デストラクタ
    virtual ~Planner();

    /**
     *	----------------------------------------
     *	ポインタの操作
     *	----------------------------------------
     */
     /// 制御する歩行ロボットへのポインタを取得する
    void acquireAsura(Asura::AsuraX* ptr);
    /// 制御する歩行ロボットへのポインタを解放する
    void releaseAsura(void);

    /// 使用する時間管理オブジェクトへのポインタを取得する
    void acquireTimeManager(TimeManager* ptr);
    /// 使用する時間管理オブジェクトへのポインタを解放する
    void releaseTimeManager(void);

    /**
     *	----------------------------------------
     *	アクセス関数
     *	----------------------------------------
     */
     /**
      *		メンバ変数の取得
      */
      /// 安定余裕
      //double getStabilityMargin(void) const{return stabilityMargin;}  20200819
      /// 計画開始からの経過時間
    double getElapsedTime(void) const { return elapsed_time; }
    /// 開始時間
    double getPlanStartTime(void) const { return planStartTime; }

    /**
     *		脚軌道そのものを得る
     */
    LegTrajectory& getLegTrajectory(int legNo)
    {
        assert(1 <= legNo && legNo <= Asura::LEG_NUM);

        return legTrajectory[legNo - 1];
    }

    /// constオブジェクトに対する添え字演算子
    LegTrajectory& getLegTrajectory(int legNo) const
    {
        assert(1 <= legNo && legNo <= Asura::LEG_NUM);

        return legTrajectory[legNo - 1];
    }

    /**
     *	----------------------------------------
     *	セット関数
     *	----------------------------------------
     */
     /// 安定余裕
     //void setStabilityMargin(double margin) {stabilityMargin = margin; return;}  20200819
     /// 経過時間
    void setElapsedTime(double time) { elapsed_time = time; return; }
    /// 開始時間
    void setPlanStartTime(double time) { planStartTime = time; return; }

    /**
     *	----------------------------------------
     *	軌道パラメータの設定
     *	----------------------------------------
     */
     /**
      *		遊脚軌道を設定する
      *			legNo：遊脚
      *			upPhase：UP相
      *			returnPhase：復帰相
      *			downPhase：DOWN相
      */
    void setLegSwingTrajectory(int legNo,
                        const Math::Vector& start,
                        const Math::Vector& upPhase,
                        const Math::Vector& returnPhase,
                        const Math::Vector& downPhase
    );

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
    Math::Vector getLegSwingPosition(int legNo, double splitTime);

    /**
     *	----------------------------------------
     *	実際に動作を行う関数
     *	----------------------------------------
     */
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
    void suspendPlan(void) { isSuspended = true; return; }

    /**
     *		動作再開
     */
    void resumePlan(void) { isSuspended = false; return; }

    /**
     *		フラグの判定
     */
    bool isActive(void) const { return isRunning; }

    /**
     *		エラーメッセージの出力
     */
    void printPlanErrorMessage(void);

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

    /**
     *		安定余裕の計算
     *			引数: 重力方向成分
     */
     //virtual double calculateStabilityMargin(double gx=0.0, double gy=0.0, double gz=1.0);  20200819

private:
    /// コピーコンストラクタ無効
    Planner(const Planner& planner);
    /// 代入演算子無効
    Planner& operator=(const Planner& planner) = delete;

    /**
     *		軌道オブジェクト
     */
     /// 軌道オブジェクトの生成
    void newTrajectories(void);
    /// 軌道オブジェクトの破棄
    void deleteTrajectories(void);

    /**
     *		安定余裕の計算のためのヘルプ関数
     */
     /*  20200819
     /// 支持脚三角形時の安定余裕
     double stabilityMarginForTriangle(const Math::Vector& foot1, const Math::Vector& foot2, const Math::Vector& foot3,
                           const Math::Vector& cog, const Math::Vector& gravity);
     /// 支持脚四角形時の安定余裕
     double stabilityMarginForQuadrangle(const Math::Vector& foot1, const Math::Vector& foot2,
                             const Math::Vector& foot3, const Math::Vector& foot4,
                             const Math::Vector& cog, const Math::Vector& gravity);
     */

};	/// end of class Planner

}	/// end of namespace Plan

#endif	/// __Planner_h__


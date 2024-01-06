
#ifndef DESIGNLABROBOTGUI_PLAN_TRAJECTORY_LEG_TRAJECTORY_H_
#define DESIGNLABROBOTGUI_PLAN_TRAJECTORY_LEG_TRAJECTORY_H_

#include "Trajectory.h"
#include "Math/Matrix/Matrix.h"


namespace designlab_robot_gui::plan
{

class LegTrajectory final : public Trajectory
{
    using Vector = math::Vector;

public:
    LegTrajectory();
    LegTrajectory(const LegTrajectory& trajectory);

    ~LegTrajectory() = default;  //!< デストラクタ

    /// 代入演算子
    virtual LegTrajectory& operator=(const LegTrajectory& trajectory);
    /// 等価演算子
    bool operator==(const LegTrajectory& trajectory) const;
    /// 非等価演算子
    bool operator!=(const LegTrajectory& trajectory) const
    {
        return !(*this == trajectory);
    }

    /// 遊脚ストローク
    const Vector& getReturnStroke() const { return returnStroke; }
    /// 遊脚振り上げ高さ
    const Vector& getUpSwing() const { return upSwing; }
    /// 遊脚振り下げ高さ
    const Vector& getDownSwing() const { return downSwing; }

    /// 復帰時間
    double getReturnTime() const { return returnTime; }
    /// 振り上げ時間
    double getUpTime() const { return upTime; }
    /// 振り下げ時間
    double getDownTime() const { return downTime; }


    /**
     *		軌道パラメータの設定
     *		goalPointおよび goalTime も自動更新される
     */
    void setLegTrajectory(
                    const Vector& start,
                    const Vector& upPhase,
                    const Vector& returnPhase,
                    const Vector& downPhase
    );

    void setLegTrajectoryTime(double start, double upPhase,
                              double returnPhase, double downPhase);

    void initializeLegTrajectory();

    /**
     *		軌道を得る
     *		time: 軌道中の時間（startTime < splitTime < goalTime）
     */
    Vector getPosition(double splitTime);

private:
    // コピーコンストラクタと代入演算子のためのヘルプ関数
    void copy(const LegTrajectory& trajectory);

    /// 遊脚ストローク
    Vector returnStroke;
    /// 遊脚振り上げ高さ
    Vector upSwing;
    /// 遊脚振り下げ高さ
    Vector downSwing;

    Vector Swing;

    /// 復帰時間
    double returnTime{ 0.0 };
    /// 振り上げ時間
    double upTime{ 0.0 };
    /// 振り下げ時間
    double downTime{ 0.0 };

};

}  // namespace designlab_robot_gui::plan


#endif  // DESIGNLABROBOTGUI_PLAN_TRAJECTORY_LEG_TRAJECTORY_H_

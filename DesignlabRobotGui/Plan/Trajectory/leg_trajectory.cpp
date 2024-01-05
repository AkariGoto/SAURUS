
#include "Plan/Trajectory/leg_trajectory.h"

#include "Utility/EngConstant.h"
#include "Math//MathConstant.h"


namespace designlab_robot_gui::plan
{

LegTrajectory::LegTrajectory()
{
    initializeLegTrajectory();
}

LegTrajectory::LegTrajectory(const LegTrajectory& trajectory)
{
    initializeLegTrajectory();
    copy(trajectory);
}


LegTrajectory& LegTrajectory::operator=(const LegTrajectory& trajectory)
{
    // 自己代入を防ぐ
    if (&trajectory != this)
    {
        copy(trajectory);
    }

    return *this;
}

bool LegTrajectory::operator==(const LegTrajectory& trajectory) const
{
    if (returnStroke != trajectory.returnStroke) { return false; }
    if (upSwing != trajectory.upSwing) { return false; }
    if (downSwing != trajectory.downSwing) { return false; }

    if (returnTime != trajectory.returnTime) { return false; }
    if (upTime != trajectory.upTime) { return false; }
    if (downTime != trajectory.downTime) { return false; }

    return true;
}

void LegTrajectory::initializeLegTrajectory()
{
    // 遊脚ストローク
    returnStroke.setSize(Const::THREE_DIMENSION);
    // 遊脚振り上げ高さ
    upSwing.setSize(Const::THREE_DIMENSION);
    // 遊脚振り下げ高さ
    downSwing.setSize(Const::THREE_DIMENSION);

    Swing.setSize(Const::THREE_DIMENSION);
}

void LegTrajectory::setLegTrajectory(const Vector& start,
                                     const Vector& upPhase,
                                     const Vector& returnPhase,
                                     const Vector& downPhase)
{
    // メンバ変数の更新
    startPosition = start;

    upSwing = upPhase;
    returnStroke = returnPhase;
    downSwing = downPhase;

    // ゴール位置を更新する
    goalPosition = startPosition + upSwing + returnStroke + downSwing;
}

void LegTrajectory::setLegTrajectoryTime(double start, double upPhase,
                                         double returnPhase, double downPhase)
{
    // メンバ変数の更新
    startTime = start;

    upTime = upPhase;
    returnTime = returnPhase;
    downTime = downPhase;

    // ゴール時間を更新する
    goalTime = startTime + upTime + returnTime + downTime;
}

Math::Vector LegTrajectory::getPosition(double splitTime)
{
    // 取り出す脚先軌道
    Vector trajectory(Const::THREE_DIMENSION);

    // 合成サイクロイド軌道
    // 離脱,接地時の速度が連続. 遊脚時間の場合分けが2パターンで済む.
    // [引用] 榊原義宏, 菅和俊, 細田祐司, 服部誠, 藤江正克, 
    // "4脚歩行機構のための低衝撃足先軌", 日本ロボット学会誌 8巻6号, pp22 - 31, 1990

    // 遊脚の中間時間
    double MiddleTime = (startTime + goalTime) / 2.00;

    // 遊脚時間
    double SwingTime = goalTime - startTime;

    if (splitTime <= startTime)
    {
        // 遊脚開始前

        trajectory = startPosition;
    }
    else if ((startTime < splitTime) && (splitTime <= MiddleTime))
    {
        // up相

        trajectory = startPosition
            + ((splitTime - startTime) / SwingTime - sin(4 * Const::PI * (splitTime - startTime) / SwingTime) / Const::PI / 4) * upSwing * 2
            + ((splitTime - startTime) / SwingTime - sin(2 * Const::PI * (splitTime - startTime) / SwingTime) / Const::PI / 2) * returnStroke;
    }
    else if ((MiddleTime < splitTime) && (splitTime < goalTime))
    {
        // down相	*downSwingの中身は負

        trajectory = startPosition + upSwing
            + ((splitTime - MiddleTime) / SwingTime - sin(4 * Const::PI * (splitTime - MiddleTime) / SwingTime) / Const::PI / 4) * downSwing * 2
            + ((splitTime - startTime) / SwingTime - sin(2 * Const::PI * (splitTime - startTime) / SwingTime) / Const::PI / 2) * returnStroke;
    }
    else if (goalTime <= splitTime)
    {
        // 遊脚終了

        trajectory = goalPosition;
    }

    return trajectory;
}

void LegTrajectory::copy(const LegTrajectory& trajectory)
{
    // 遊脚ストローク
    returnStroke = trajectory.returnStroke;
    // 遊脚振り上げ高さ
    upSwing = trajectory.upSwing;
    // 遊脚振り下げ高さ
    downSwing = trajectory.downSwing;

    // 復帰時間
    returnTime = trajectory.returnTime;
    // 振り上げ時間
    upTime = trajectory.upTime;
    // 振り下げ時間
    downTime = trajectory.downTime;
}

}  // namespace designlab_robot_gui::plan

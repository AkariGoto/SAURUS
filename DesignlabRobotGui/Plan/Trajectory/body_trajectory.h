
#ifndef DESIGNLABROBOTGUI_PLAN_TRAJECTORY_BODY_TRAJECTORY_H_
#define DESIGNLABROBOTGUI_PLAN_TRAJECTORY_BODY_TRAJECTORY_H_

#include "Plan/Trajectory/trajectory.h"
#include "Math/Matrix/Matrix.h"


namespace designlab_robot_gui::plan
{

class BodyTrajectory final : public Trajectory
{
    using Vector = math::Vector;

public:
    BodyTrajectory();
    BodyTrajectory(const BodyTrajectory& trajectory);

    ~BodyTrajectory() = default;

    BodyTrajectory& operator=(const BodyTrajectory& trajectory);
    bool operator==(const BodyTrajectory& trajectory) const;
    bool operator!=(const BodyTrajectory& trajectory) const
    {
        return !(*this == trajectory);
    }

    const Vector& getMoveStride() const { return moveStride; }
    const double getMoveTime() const { return moveTime; }

    void setMoveTime(const double time) { moveTime = time; }
    void setBodyTrajectory(const Vector& stride);
    void setBodyTrajectoryTime(double time);

    void initializeBodyTrajectory();

    Vector getPosition(double splitTime);

private:
    void copy(const BodyTrajectory& trajectory);

    Vector moveStride;  //!< 遊脚ストローク

    double moveTime;  //!< 復帰時間
};

}  // namespace designlab_robot_gui::plan


#endif  // DESIGNLABROBOTGUI_PLAN_TRAJECTORY_BODY_TRAJECTORY_H_

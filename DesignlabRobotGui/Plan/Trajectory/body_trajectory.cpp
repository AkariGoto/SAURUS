
#include "Plan/Trajectory/body_trajectory.h"

#include "Math/math_constant.h"


namespace designlab_robot_gui::plan
{

BodyTrajectory::BodyTrajectory()
{
    initializeBodyTrajectory();
}

BodyTrajectory::BodyTrajectory(const BodyTrajectory& trajectory)
{
    initializeBodyTrajectory();
    copy(trajectory);
}

BodyTrajectory& BodyTrajectory::operator=(const BodyTrajectory& trajectory)
{
    // 自己代入防止
    if (&trajectory != this)
    {
        copy(trajectory);
    }

    return *this;
}

bool BodyTrajectory::operator==(const BodyTrajectory& trajectory) const
{
    if (moveStride != trajectory.moveStride) { return false; }
    if (moveTime != trajectory.moveTime) { return false; }

    return true;
}

void BodyTrajectory::initializeBodyTrajectory()
{
    moveStride.setSize(math::THREE_DIMENSION);
    moveTime = 0.0;
}

void BodyTrajectory::setBodyTrajectory(const Vector& stride)
{
    moveStride = stride;
    goalPosition = startPosition + stride;
}

void BodyTrajectory::setBodyTrajectoryTime(const double time)
{
    moveTime = time;
    goalTime = startTime + time;
}

math::Vector BodyTrajectory::getPosition(const double splitTime)
{
    // 取り出す重心軌道
    Vector trajectory(math::THREE_DIMENSION);

    return trajectory;
}

void BodyTrajectory::copy(const BodyTrajectory& trajectory)
{
    moveStride = trajectory.moveStride;
    moveTime = trajectory.moveTime;

    return;
}

}  // namespace designlab_robot_gui::plan

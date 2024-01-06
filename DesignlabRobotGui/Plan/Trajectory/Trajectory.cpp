
#include "Plan/Trajectory/trajectory.h"

#include "Math/math_constant.h"


namespace designlab_robot_gui::plan
{

Trajectory::Trajectory(const Trajectory& trajectory)
{
    initializeTrajectory();
    copy(trajectory);
}

Trajectory& Trajectory::operator=(const Trajectory& trajectory)
{
    // 自己代入を防ぐ
    if (&trajectory != this)
    {
        copy(trajectory);
    }

    return *this;
}

bool Trajectory::operator ==(const Trajectory& trajectory) const
{
    if (startPosition != trajectory.startPosition) { return false; }
    if (goalPosition != trajectory.goalPosition) { return false; }
    if (startTime != trajectory.startTime) { return false; }
    if (startTime != trajectory.goalTime) { return false; }

    return true;
}

void Trajectory::initializeTrajectory()
{
    // スタート地点
    startPosition.setSize(math::THREE_DIMENSION);
    // ゴール地点
    goalPosition.setSize(math::THREE_DIMENSION);
}

void Trajectory::setDistance(const Vector& start, const Vector& goal)
{
    startPosition = start;
    goalPosition = goal;
}

void Trajectory::setTime(double start, double goal)
{
    startTime = start;
    goalTime = goal;
}

void Trajectory::shiftStartPosition(const Vector& start)
{
    Vector shift(math::THREE_DIMENSION);
    shift = goalPosition - startPosition;

    startPosition = start;
    goalPosition = start + shift;
}

void Trajectory::shiftStartTime(const double start)
{
    const double shift = goalTime - startTime;

    startTime = start;
    goalTime = start + shift;
}

math::Vector Trajectory::getPosition(const double splitTime)
{
    // 取り出す脚先軌道
    Vector trajectory(math::THREE_DIMENSION);

    if (splitTime <= startTime)
    {
        trajectory = startPosition;
    }
    else if (startTime < splitTime && splitTime <= goalTime)
    {
        trajectory = (goalPosition - startPosition) * ((splitTime - startTime) /
                                                       (goalTime - startTime));
    }
    else if (goalTime < splitTime)
    {
        trajectory = goalPosition;
    }

    return trajectory;
}

void Trajectory::copy(const Trajectory& trajectory)
{
    startPosition = trajectory.startPosition;
    goalPosition = trajectory.goalPosition;

    startTime = trajectory.startTime;
    goalTime = trajectory.goalTime;
}


}  // namespace designlab_robot_gui::plan

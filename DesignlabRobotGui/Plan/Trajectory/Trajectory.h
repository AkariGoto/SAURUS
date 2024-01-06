
#ifndef DESIGNLABROBOTGUI_PLAN_TRAJECTORY_TRAJECTORY_H_
#define DESIGNLABROBOTGUI_PLAN_TRAJECTORY_TRAJECTORY_H_

#include "Math/Matrix/Matrix.h"


namespace designlab_robot_gui::plan
{

class Trajectory
{
    using Vector = math::Vector;
    using Matrix = math::Matrix;

public:
    inline Trajectory() { initializeTrajectory(); }
    Trajectory(const Trajectory& trajectory);

    virtual ~Trajectory() = default;

    virtual Trajectory& operator=(const Trajectory& trajectory);
    bool operator==(const Trajectory& trajectory) const;
    inline bool operator!=(const Trajectory& trajectory) const
    {
        return !(*this == trajectory);
    }


    // 距離に関係するもの
    const Vector& getStartPosition() const { return startPosition; }
    Vector& getStartPosition() { return startPosition; }
    const Vector& getGoalPosition() const { return goalPosition; }
    Vector& getGoalPosition() { return goalPosition; }

    // 移動距離
    const Vector getDistance() const { return (goalPosition - startPosition); }
    Vector getDistance() { return (goalPosition - startPosition); }

    // 時間に関係するもの
    const double getStartTime() const { return startTime; }
    double getStartTime() { return startTime; }
    const double getGoalTime() const { return goalTime; }
    double getGoalTime() { return goalTime; }

    /// 総軌道時間
    double getTime() const { return (goalTime - startTime); }

    void setDistance(const Vector& start, const Vector& goal);
    void setTime(double start, double goal);

    void initializeTrajectory();

    void shiftStartPosition(const Vector& start);
    void shiftStartTime(double start);
    virtual Vector getPosition(double splitTime);

protected:
    Vector startPosition;  //!< スタート位置
    Vector goalPosition;  //!< ゴール位置


    double startTime{ 0.0 };  //!< スタート時間
    double goalTime{ 0.0 };  //!< ゴール時間

private:
    /// コピーコンストラクタと代入演算子のためのヘルプ関数
    void copy(const Trajectory& trajectory);

};

}  // namespace designlab_robot_gui::plan


#endif  // DESIGNLABROBOTGUI_PLAN_TRAJECTORY_TRAJECTORY_H_

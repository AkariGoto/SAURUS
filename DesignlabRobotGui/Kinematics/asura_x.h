
#ifndef DESIGNLABROBOTGUI_KINEMATICS_ASURAX_H_
#define DESIGNLABROBOTGUI_KINEMATICS_ASURAX_H_

#include "Kinematics/walking_robot.h"


namespace designlab_robot_gui::asura
{

class AsuraX final : public WalkingRobot
{
public:
    /// デフォルトコンストラクタ
    AsuraX() = default;

    /// デストラクタ
    ~AsuraX() = default;
};

}  // namespace designlab_robot_gui::asura


#endif  // DESIGNLABROBOTGUI_KINEMATICS_ASURAX_H_

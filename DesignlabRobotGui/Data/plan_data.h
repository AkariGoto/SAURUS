
#ifndef DESIGNLABROBOTGUI_DATA_PLANDATA_H_
#define DESIGNLABROBOTGUI_DATA_PLANDATA_H_

#include <algorithm>

#include "Kinematics/AsuraParameter.h"
#include "Math/Matrix/Matrix.h"
#include "Plan/PlanParameter.h"


namespace designlab_robot_gui::data
{

class PlanData final
{
public:
    PlanData();                            //!< コンストラクタ
    PlanData(const PlanData& PlanData);    //!< コピーコンストラクタ

    ~PlanData() = default;         //!< デストラクタ


    PlanData& operator=(const PlanData& planData);  //!< 代入演算子

    bool operator==(const PlanData& planData);  //!< 等価演算子
    inline bool operator!=(const PlanData& planData) { return !(*this == planData); }  //!< 非等価演算子


    //! 経過時間を取得する．
    //! @return 経過時間
    constexpr double getElapsedTime() const
    {
        return elapsedTime;
    }

    //! 経過時間を設定する．
    //! @param[in] time 経過時間
    inline void setElapsedTime(const double time)
    {
        elapsedTime = time; return;
    }

private:
    // コピーコンストラクタと代入演算子のヘルプ関数
    void copy(const PlanData& planData);

    double elapsedTime;  //!< 経過時間
    double crabAngle;    //!< 側行角
};

}  // namespace designlab_robot_gui::data


#endif  // DESIGNLABROBOTGUI_DATA_PLANDATA_H_


#ifndef DESIGNLABROBOTGUI_DATA_PLANDATA_H_
#define DESIGNLABROBOTGUI_DATA_PLANDATA_H_


namespace designlab_robot_gui::data
{

//! @struct PlanData
//! @brief 経過時間を記録するための構造体．
//! @details
//! final をつけることで，この構造体を継承することを禁止する．
struct PlanData final
{
    double elapsed_time_{ 0.0 };  //!< 経過時間．{} の中の値で初期化する．
};

}  // namespace designlab_robot_gui::data


#endif  // DESIGNLABROBOTGUI_DATA_PLANDATA_H_

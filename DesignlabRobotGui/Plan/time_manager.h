
#ifndef DESIGNLABROBOTGUI_PLAN_TIME_MANAGER_H_
#define DESIGNLABROBOTGUI_PLAN_TIME_MANAGER_H_


namespace designlab_robot_gui::plan
{

//! @brief 時間管理クラス．
//! シングルトンクラス(インスタンスは1つしか生成できない)．
class TimeManager final
{
public:
    TimeManager() = default;  //!< コンストラクタは削除する
    TimeManager(const TimeManager& timeManager) = delete;  //!< コピーコンストラクタは削除する
    TimeManager& operator=(const TimeManager& timeManager) = delete;  //!< コピー代入演算子は削除する
    TimeManager(TimeManager&& timeManager) = delete;  //!< ムーブコンストラクタは削除する

    //! @brief 実時間を設定する．
    //! @param[in] time 実時間 [msec]．0以上の値を設定すること．
    static void setRealTime(double time);


    //! @brief 実時間を取得する．
    //! @return 実時間 [msec]
    static constexpr double getRealTime() { return realTime; }

private:
    static double realTime;  //!< 実時間 [msec]
};

}  // namespace designlab_robot_gui::plan


#endif  // DESIGNLABROBOTGUI_PLAN_TIME_MANAGER_H_
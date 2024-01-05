
#ifndef DESIGNLABROBOTGUI_MOTION_PLAN_THREAD_H_
#define DESIGNLABROBOTGUI_MOTION_PLAN_THREAD_H_

#include <afxmt.h>

#include "Data/asura_data.h"
#include "Data/plan_data.h"
#include "Data/data_handler.h"
#include "Kinematics/asura_x.h"
#include "Plan/plan_parameter.h"
#include "Plan/planner_manager.h"
#include "Plan/tripod_gait_planner.h"
#include "System/Thread/timed_motion_procedure.h"
#include "System/Timer/multi_media_timer.h"
#include "System/win_user_message.h"
#include "UDP/asura_udp_thread.h"


namespace designlab_robot_gui::system
{

class MotionPlanThread : public CWinThread
{
    static asura::AsuraX asuraX;

    /// キネマティクスデータ
    static data::AsuraData asuraXData;
    /// プランデータ
    static data::PlanData planData;
    /// データハンドラオブジェクト
    static data::DataHandler dataHandler;

    //! 動作計画オブジェクト
    static plan::PlannerManager		plannerManager;

    static plan::TripodGaitPlanner	tripodGait;

    TimedMotionProcedure* pTimedMotionProcedure;
    MultiMediaTimer* multi_media_timer_ptr;
    static plan::TimeManager		timeManager;

    udp::AsuraUdpThread* pUDPThread;

    /// スレッドが有効かどうか
    BOOL isAlive;

    /// 通信が有効かどうか
    bool isCommAlive;

    DECLARE_DYNCREATE(MotionPlanThread)

protected:
    /// デフォルトコンストラクタ
    MotionPlanThread();           // 動的生成で使用される protected コンストラクタ
    /// デストラクタ
    virtual ~MotionPlanThread();

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();


    /**
     *	説明
     *		初期化
     */
    void initializeMotionPlanThread();

    /**
     *	説明
     *		終了処理
     */
    void finalizeMotionPlanThread();

    /**
     *	説明
     *		スレッドが有効かどうか
     */
    BOOL isRunning() const { return isAlive; }

    void acquireAsuraUDPThread(udp::AsuraUdpThread* pDlg);

    DECLARE_MESSAGE_MAP()


protected:

    // カスタムメッセージマップ開始

    // スレッド終了
    afx_msg void OnEndThread(WPARAM wParam, LPARAM lParam);
    /// データをメインスレッドに送信
    afx_msg void OnSendViewData(WPARAM wParam, LPARAM lParam);
    /// 歩容のセットアップ
    afx_msg void OnSetupMotion(WPARAM wParam, LPARAM lParam);
    /// 歩容スタート
    afx_msg void OnStartMotion(WPARAM wParam, LPARAM lParam);
    /// 歩容ストップ
    afx_msg void OnStopMotion(WPARAM wParam, LPARAM lParam);
    /// 歩容生成
    afx_msg void OnGenerateMotion(WPARAM wParam, LPARAM lParam);

    // 通信関係

    //! データ送信開始
    afx_msg void OnStartDataSending(WPARAM wParam, LPARAM lParam);
    //! データ送信停止
    afx_msg void OnStopDataSending(WPARAM wParam, LPARAM lParam);


};

} // namespace designlab_robot_gui::system


#endif //DESIGNLABROBOTGUI_MOTION_PLAN_THREAD_H_

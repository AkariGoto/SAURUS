
#ifndef DESIGNLABROBOTGUI_MOTION_PLAN_THREAD_H_
#define DESIGNLABROBOTGUI_MOTION_PLAN_THREAD_H_

#include <afxmt.h>

#include "Data/asura_data.h"
#include "Data/plan_data.h"
#include "Data/data_handler.h"
#include "Kinematics/asura_x.h"
#include "Plan/plan_parameter.h"
#include "Plan/PlannerManager.h"
#include "Plan/TripodGaitPlanner.h"


#include "..\Timer\MultiMediaTimer.h"
#include "TimedMotionProcedure.h"
#include "..\WinUserMessage.h"
#include "..\..\UDP/AsuraUDPThread.h"



class AsuraUDPThread;


class MotionPlanThread : public CWinThread
{
    using AsuraData = designlab_robot_gui::data::AsuraData;
    using PlanData = designlab_robot_gui::data::PlanData;
    using DataHandler = designlab_robot_gui::data::DataHandler;

    DECLARE_DYNCREATE(MotionPlanThread)

    /**
     *		Asuraのキネマティクスオブジェクト
     */
    static Asura::AsuraX asuraX;

    /**
     *		データオブジェクト
     */
     /// キネマティクスデータ
    static AsuraData asuraXData;
    /// プランデータ
    static PlanData planData;
    /// データハンドラオブジェクト
    static DataHandler dataHandler;

    /**
     *		動作計画オブジェクト
     */
    static Plan::PlannerManager		plannerManager;

    static Plan::TripodGaitPlanner	tripodGait;  //20200820


    System::TimedMotionProcedure* pTimedMotionProcedure;
    System::MultiMediaTimer* pMultiMediaTimer;
    static Plan::TimeManager		timeManager;


    AsuraUDPThread* pUDPThread;



    /// スレッドが有効かどうか
    BOOL isAlive;

    /// 通信が有効かどうか
    bool isCommAlive;

    //DWORD URGThreadID;  20200820



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

    void MotionPlanThread::acquireAsuraUDPThread(AsuraUDPThread* pDlg);

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

    /// 通信関係
    /// データ送信開始
    afx_msg void OnStartDataSending(WPARAM wParam, LPARAM lParam);
    /// データ送信停止
    afx_msg void OnStopDataSending(WPARAM wParam, LPARAM lParam);


};


#endif //DESIGNLABROBOTGUI_MOTION_PLAN_THREAD_H_

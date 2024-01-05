
#include "../../pch.h"

#include "System/Thread/MotionPlanThread.h"

#include "ASURA2GUI.h"
#include "Utility/EngConstant.h"


namespace designlab_robot_gui::system
{
asura::AsuraX MotionPlanThread::asuraX;

data::AsuraData MotionPlanThread::asuraXData;
data::PlanData MotionPlanThread::planData;
data::DataHandler MotionPlanThread::dataHandler(&asuraX, &asuraXData,
                                                &tripodGait, &planData);

plan::TimeManager MotionPlanThread::timeManager;

plan::PlannerManager MotionPlanThread::plannerManager;
plan::TripodGaitPlanner MotionPlanThread::tripodGait(&MotionPlanThread::asuraX,
                                                     &MotionPlanThread::timeManager);


/**
 *	------------------------------------------------------------
 *		MotionPlanThreadクラスのメンバ関数定義
 *	------------------------------------------------------------
 */
IMPLEMENT_DYNCREATE(MotionPlanThread, CWinThread)

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
    MotionPlanThread::MotionPlanThread() :pMultiMediaTimer(), pTimedMotionProcedure(), pUDPThread()
{
    /// 生存フラグの初期化
    isAlive = FALSE;

    /// 通信フラグ
    isCommAlive = false;

}

MotionPlanThread::~MotionPlanThread()
{
    /// 生存フラグの終了処理
    isAlive = FALSE;
}

BOOL MotionPlanThread::InitInstance()
{
    // TODO:  スレッドごとの初期化をここで実行します。
    initializeMotionPlanThread();

    return TRUE;
}

int MotionPlanThread::ExitInstance()
{
    // TODO:  スレッドごとの後処理をここで実行します。
    finalizeMotionPlanThread();

    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(MotionPlanThread, CWinThread)

    ON_THREAD_MESSAGE(WM_PLAN_END_THREAD, OnEndThread)					/// スレッド終了
    ON_THREAD_MESSAGE(WM_PLAN_REQUEST_VIEW_DATA, OnSendViewData)					/// 表示データ送信
    ON_THREAD_MESSAGE(WM_PLAN_SETUP_MOTION, OnSetupMotion)					/// プランのセットアップ
    ON_THREAD_MESSAGE(WM_PLAN_START_MOTION, OnStartMotion)					/// プラン開始
    ON_THREAD_MESSAGE(WM_PLAN_STOP_MOTION, OnStopMotion)					/// プラン停止
    ON_THREAD_MESSAGE(WM_PLAN_GENERATE_MOTION, OnGenerateMotion)				/// プラン生成

    ON_THREAD_MESSAGE(WM_PLAN_COMM_START, OnStartDataSending)				/// データ送信開始
    ON_THREAD_MESSAGE(WM_PLAN_COMM_STOP, OnStopDataSending)				/// データ送信停止

END_MESSAGE_MAP()


void MotionPlanThread::OnEndThread(WPARAM wParam, LPARAM lParam)
{
    /// スレッド終了処理
    ExitInstance();

    /// システムに終了を要求(WM_DESTORYに対応)
    PostQuitMessage(0);
}

void MotionPlanThread::OnSendViewData(WPARAM wParam, LPARAM lParam)
{
    // ViewへAsuraXの表示データを送信
    ::PostMessage((HWND)wParam, WM_PLAN_SENDING_VIEW_DATA, (WPARAM)&asuraXData, (LPARAM)&planData);
}

void MotionPlanThread::OnSetupMotion(WPARAM wParam, LPARAM lParam)
{
    plan::Strategy strategy = (plan::Strategy)lParam;

    switch (strategy)
    {
        case plan::Strategy::TRIPOD:
        {
            plannerManager.switchPlan(&tripodGait, plan::Strategy::TRIPOD);
            break;
        }
        case plan::Strategy::TRIPOD_1_CYCLE:
        {
            plannerManager.switchPlan(&tripodGait, plan::Strategy::TRIPOD_1_CYCLE);
            break;
        }
        default:
            break;
    }

    plannerManager.setup();
}

void MotionPlanThread::OnStartMotion(WPARAM wParam, LPARAM lParam)
{
    plannerManager.startMotion();
}

void MotionPlanThread::OnStopMotion(WPARAM wParam, LPARAM lParam)
{
    plannerManager.stopMotion();
}

void MotionPlanThread::OnGenerateMotion(WPARAM wParam, LPARAM lParam)
{
    /// 精確な現在時刻を取得
    timeManager.setRealTime(pMultiMediaTimer->getPresentTime());

    if (plannerManager.isActive())
    {
        /// 基準歩容のスナップショットデータ作成
        plannerManager.createSnapshot();
    }

    /// データの保存
    dataHandler.handleAsuraData();
    dataHandler.handlePlanData();

    /// <UDPでボールねじストローク（ワイヤ変位）指令値送信>

    if (isCommAlive)
        pUDPThread->PostThreadMessage(WM_PLAN_SENDING_COMM_DATA, (WPARAM)(&asuraXData), (LPARAM)(&planData));


    return;
}

/**
 *	----------------------------------------
 *	通信関連
 *	----------------------------------------
 */
 /// データ送信開始
void MotionPlanThread::OnStartDataSending(WPARAM wParam, LPARAM lParam)
{
    isCommAlive = true;
}

/// データ送信停止
void MotionPlanThread::OnStopDataSending(WPARAM wParam, LPARAM lParam)
{
    isCommAlive = false;
}

/**
 *	説明
 *		初期化
 */
void MotionPlanThread::initializeMotionPlanThread(void)
{

    /**
     *	説明
     *		タイマコールバック作成
     */
     /// オブジェクト作成
    pTimedMotionProcedure = new System::TimedMotionProcedure();
    /// タイマコールバックに本スレッドのIDを登録
    pTimedMotionProcedure->setThreadID(m_nThreadID);	//// m_nThreadは基底クラスのCWinThreadのメンバ


    /**
     *	タイマ本体作成
     */
    pMultiMediaTimer = new System::MultiMediaTimer(*pTimedMotionProcedure);
    pMultiMediaTimer->setTimer(20, 5);

    /// スレッドをアクティブに
    isAlive = TRUE;
}

/**
 *	説明
 *		終了処理
 */
void MotionPlanThread::finalizeMotionPlanThread(void)
{
    /**
     *	タイマ終了処理
     */
     /// タイマが停止していなかったら
    if (pMultiMediaTimer != NULL)
    {
        pMultiMediaTimer->killTimer();

        /// タイマオブジェクト破棄
        delete pMultiMediaTimer;
        pMultiMediaTimer = NULL;
    }

    if (pTimedMotionProcedure != NULL)
    {
        /// タイマコールバック破棄
        delete pTimedMotionProcedure;
        pTimedMotionProcedure = NULL;
    }

    return;
}

//UDP通信ダイアログへのポインタをセット
void MotionPlanThread::acquireAsuraUDPThread(designlab_robot_gui::udp::AsuraUdpThread* pDlg)
{
    pUDPThread = pDlg;
    return;
}

}

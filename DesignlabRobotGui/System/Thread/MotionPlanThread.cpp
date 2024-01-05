/**
 *  ファイル名
 *		MotionPlanThread.cpp
 *  説明
 *		歩容計画を行うスレッド生成クラスの宣言クラス
 *		 UIスレッドとして作成するためCWinThreadから派生
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/26(SAT)
 */

 // MotionPlanThread.cpp : 実装ファイル
 //

 //  20200820  trotGaitをtripodGaitに修正・AxisControlダイアログ関連コメントアウト
 //  20201005  1周期で終了

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
  //#include "..\..\stdafx.h"
#include "..\..\pch.h"

#include "..\..\ASURA2GUI.h"
#include "MotionPlanThread.h"

//#include "..\..\Dialog\AxisControlDialog.h"
//#include "..\..\Dialog\SerialPortDialog.h"
#include "Utility/EngConstant.h"


using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;
using namespace System;

// MotionPlanThread

/**
 *	----------------------------------------------------------------------
 *		MotionPlanThreadクラス
 *	----------------------------------------------------------------------
 */

 /**
  *	------------------------------------------------------------
  *		MotionPlanThreadクラスのstatic変数の初期化
  *	------------------------------------------------------------
  */
AsuraX MotionPlanThread::asuraX;

designlab_robot_gui::data::AsuraData MotionPlanThread::asuraXData;
designlab_robot_gui::data::PlanData MotionPlanThread::planData;
designlab_robot_gui::data::DataHandler MotionPlanThread::dataHandler(&asuraX, &asuraXData, &tripodGait, &planData);  //20200820

TimeManager MotionPlanThread::timeManager;

PlannerManager MotionPlanThread::plannerManager;
//AxisControlPlanner MotionPlanThread::axisControl(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);

//CrawlGaitPlanner MotionPlanThread::crawlGait(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
TripodGaitPlanner MotionPlanThread::tripodGait(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);  //  20200820
//TrackDrivePlanner MotionPlanThread::trackDrive(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
//ModeChangePlanner MotionPlanThread::modeChange(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
//HybridMotionPlanner MotionPlanThread::hybridMotion(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
//URGMotionPlanner MotionPlanThread::URGMotion(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);		//追加

//double MotionPlanThread::axisCtrlData[AC_DLG_AXIS_NUM] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};  20200820


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

    //ON_THREAD_MESSAGE(WM_PLAN_AC_LEG_NO,			OnAxisCtrlDlgLegNo)				/// 脚番号選択
    //ON_THREAD_MESSAGE(WM_PLAN_AC_CTRL_MODE,			OnAxisCtrlDlgCtrlMode)			/// 制御モード選択
    //ON_THREAD_MESSAGE(WM_PLAN_AC_SENDING_SLDR_DATA,	OnAxisCtrlDlgRecieveSldrData)	/// データ受信
    //ON_THREAD_MESSAGE(WM_PLAN_AC_REQUEST_DATA,		OnAxisCtrlDlgSendData)			/// データ送信

    //ON_THREAD_MESSAGE(WM_PLAN_URGTHREAD_SET,		OnURGThreadSet)					/// URGThreadアドレスセット

END_MESSAGE_MAP()

// MotionPlanThread メッセージ ハンドラ
/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *		MotionPlanThread メッセージハンドラ 関数
 *	------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	Doc，Viewクラス関連
  *	----------------------------------------
  */
  /**
   *	説明
   *		スレッドの終了処理
   */
void MotionPlanThread::OnEndThread(WPARAM wParam, LPARAM lParam)
{
    /// スレッド終了処理
    ExitInstance();

    /// システムに終了を要求(WM_DESTORYに対応)
    PostQuitMessage(0);
}

/**
 *	説明
 *		メインスレッドへのデータ送信
 */
void MotionPlanThread::OnSendViewData(WPARAM wParam, LPARAM lParam)
{
    /// ViewへAsuraXの表示データを送信
    ::PostMessage((HWND)wParam, WM_PLAN_SENDING_VIEW_DATA, (WPARAM)&asuraXData, (LPARAM)&planData);
}

/**
 *	説明
 *		歩容セットアップ
 */
void MotionPlanThread::OnSetupMotion(WPARAM wParam, LPARAM lParam)
{
    Plan::Strategy strategy = (Plan::Strategy)lParam;

    switch (strategy)
    {
        case Strategy::TRIPOD:
        {
            plannerManager.switchPlan(&tripodGait, Plan::Strategy::TRIPOD);
            break;
        }
        case Strategy::TRIPOD_1_CYCLE:
        {
            plannerManager.switchPlan(&tripodGait, Plan::Strategy::TRIPOD_1_CYCLE);
            break;
        }
        default:
            break;
    }

    plannerManager.setup();
}

/**
 *	説明
 *		歩容開始
 */
void MotionPlanThread::OnStartMotion(WPARAM wParam, LPARAM lParam)
{
    plannerManager.startMotion();
}

/**
 *	説明
 *		歩容停止
 */
void MotionPlanThread::OnStopMotion(WPARAM wParam, LPARAM lParam)
{
    plannerManager.stopMotion();
}

/**
 *	説明
 *		歩容生成
 *		タイマによって呼ばれる中身
 */
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
    pTimedMotionProcedure = new TimedMotionProcedure();
    /// タイマコールバックに本スレッドのIDを登録
    pTimedMotionProcedure->setThreadID(m_nThreadID);	//// m_nThreadは基底クラスのCWinThreadのメンバ


    /**
     *	タイマ本体作成
     */
    pMultiMediaTimer = new MultiMediaTimer(*pTimedMotionProcedure);
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
void MotionPlanThread::acquireAsuraUDPThread(AsuraUDPThread* pDlg)
{
    pUDPThread = pDlg;
    return;
}
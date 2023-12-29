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
using namespace Data;
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

AsuraData MotionPlanThread::asuraXData;
PlanData MotionPlanThread::planData;
//DataHandler MotionPlanThread::dataHandler(&asuraX, &asuraXData, &hybridMotion, &planData);
DataHandler MotionPlanThread::dataHandler(&asuraX, &asuraXData, &tripodGait, &planData);  //20200820

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
        /*case CRAWL:

          plannerManager.switchPlan(&crawlGait, Plan::CRAWL);
          break;*/

          /*case TROT:
            plannerManager.switchPlan(&tripodGait, Plan::TROT);  //  20200820
            break;*/

        case TRIPOD:
            plannerManager.switchPlan(&tripodGait, Plan::TRIPOD);  //  20200820
            break;

            //20201005
        case TRIPOD_1_CYCLE:
            plannerManager.switchPlan(&tripodGait, Plan::TRIPOD_1_CYCLE);  //  20200820
            break;

            /*case TRACK_DRIVE:
              plannerManager.switchPlan(&trackDrive, Plan::TRACK_DRIVE);
              break;*/

              /*case MODE_CHANGE:
                plannerManager.switchPlan(&modeChange, Plan::MODE_CHANGE);
                break;*/

                /*case AXIS_CONTROL:
                  plannerManager.switchPlan(&axisControl, Plan::AXIS_CONTROL);
                  break;*/

                  /*	case HYBRID_MOTION:
                      plannerManager.switchPlan(&hybridMotion, Plan::HYBRID_MOTION);
                      break;*/

                      /*case URG_MOTION:
                        plannerManager.switchPlan(&URGMotion, Plan::URG_MOTION);		//追加
                        break;*/

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

///**
// *	----------------------------------------
// *	CAxisControlDialog関連
// *	----------------------------------------
// */
///// 脚番号設定
//void MotionPlanThread::OnAxisCtrlDlgLegNo(WPARAM wParam, LPARAM lParam)
//{
//	/// 脚番号を設定
//	int legNo = (int)wParam;
//	axisControl.setCtrlTargetLegNo( legNo );
//
//	/// 現在値をPlannerに保持
//	axisControl.setJointAngle(legNo, asuraXData.getLegJointAngle( legNo ) );
//	axisControl.setFootPosition(legNo, asuraXData.getLegFootPosition( legNo ) );
//
//	return;
//}
//
///// 制御モード設定
//void MotionPlanThread::OnAxisCtrlDlgCtrlMode(WPARAM wParam, LPARAM lParam)
//{
//	/// ダイアログの制御モード番号により制御モードを決定
//	switch ( (int)wParam )
//	{
//		case 0:
//			axisControl.setControlMode( AxisControlPlanner::CONTROL_OFF );
//			break;
//
//		case 1:
//			axisControl.setControlMode( AxisControlPlanner::POSITION );
//			break;
//
//		case 2:
//			axisControl.setControlMode( AxisControlPlanner::VELOCITY );
//			break;
//
//		default:
//			break;
//	}
//}
//
///// スライダー値受信
//void MotionPlanThread::OnAxisCtrlDlgRecieveSldrData(WPARAM wParam, LPARAM lParam)
//{
//	/// 制御モードの取得
//	AxisControlPlanner::ControlMode mode = axisControl.getControlMode();
//	/// 制御脚の番号取得 
//	int no = axisControl.getCtrlTargetLegNo();
//	/// 指令値を取得
//	double data = (double)lParam;
//
//	/// 制御オブジェクトの現在値を取得
//	Vector joint(LEG_JOINT_NUM);
//	Vector foot(THREE_DIMENSION);
//
//	if ( mode == AxisControlPlanner::POSITION )
//	{
//		joint = asuraXData.getLegJointAngle(no);
//		foot = asuraXData.getLegFootPosition(no);
//
//		/// スライダー番号により挙動と値を決定
//		switch ( (int)wParam )
//		{
//			case 1:
//				axisControl.setKinematicBehavior( AxisControlPlanner::DIRECT );
//				joint(1) = data*DEG2RAD;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 2:
//				axisControl.setKinematicBehavior( AxisControlPlanner::DIRECT );
//				joint(2) = data*DEG2RAD;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 3:
//				axisControl.setKinematicBehavior( AxisControlPlanner::DIRECT );
//				joint(3) = data*DEG2RAD;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 4:
//				axisControl.setKinematicBehavior( AxisControlPlanner::INVERSE );
//				foot(1) = data;
//				axisControl.setFootPosition(no, foot);
//				break;
//			case 5:
//				axisControl.setKinematicBehavior( AxisControlPlanner::INVERSE );
//				foot(2) = data;
//				axisControl.setFootPosition(no, foot);
//				break;
//			case 6:
//				axisControl.setKinematicBehavior( AxisControlPlanner::INVERSE );
//				foot(3) = data;
//				axisControl.setFootPosition(no, foot);
//				break;
//			default:
//				break;
//		}
//	}
//	else if ( mode == AxisControlPlanner::VELOCITY )
//	{
//		/// スライダー番号により挙動と値を決定
//		switch ( (int)wParam )
//		{
//			case 1:
//				joint(1) = data;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 2:
//				joint(2) = data;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 3:
//				joint(3) = data;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 4:
//				foot(1) = data;
//				axisControl.setFootPosition(no, joint);
//				break;
//			case 5:
//				foot(2) = data;
//				axisControl.setFootPosition(no, joint);
//				break;
//			case 6:
//				foot(3) = data;
//				axisControl.setFootPosition(no, joint);
//				break;
//			default:
//				break;
//		}
//	}
//
//	return;
//
//}
//
///// AsuraXのデータをACダイアログに送信
//void MotionPlanThread::OnAxisCtrlDlgSendData(WPARAM wParam, LPARAM lParam)
//{
//	PostMessage( (HWND)wParam, WM_PLAN_SENDING_AC_DATA, (WPARAM)&asuraXData, NULL);
//	
//
//	/// AxisControlPlannerの指令値も更新
//	int legNo = axisControl.getCtrlTargetLegNo();
//	if ( legNo != 0 )
//	{
//		axisControl.setJointAngle( legNo, asuraXData.getLegJointAngle(legNo) );
//		axisControl.setFootPosition( legNo, asuraXData.getLegFootPosition(legNo) );
//	}
//
//	return;
//}
//
////URGThreadアドレスセット
//void MotionPlanThread::OnURGThreadSet(WPARAM wParam, LPARAM lParam)
//{
//	URGThreadID=(DWORD)wParam;
//
//	pTimedMotionProcedure->URGsetThreadID(URGThreadID);	//// m_nThreadは基底クラスのCWinThreadのメンバ		URG用
//
//	return;
//}

/**
 *	------------------------------------------------------------
 *		自作関数
 *	------------------------------------------------------------
 */
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

///**
// *	説明
// *		操作するダイアログへのポインタをセット
// */
///// 軸制御ダイアログ
//void MotionPlanThread::acquireAxisControlDialog(CAxisControlDialog* pDlg)
//{
//	pAxisControlDialog = pDlg;
//	return;
//}
//
///// シリアル通信ダイアログ
//void MotionPlanThread::acquireSerialPortDialog(CSerialPortDialog* pDlg)
//{
//	pSerialPortDialog = pDlg;
//	return;
//}

//UDP通信ダイアログへのポインタをセット
void MotionPlanThread::acquireAsuraUDPThread(AsuraUDPThread* pDlg)
{
    pUDPThread = pDlg;
    return;
}
/**
 *  ファイル名
 *		MotionPlanThread.h
 *  説明
 *		歩容計画を行うスレッド生成クラスの宣言クラス
 *		 UIスレッドとして作成するためCWinThreadから派生
 *  日付
 *		作成日: 2014/04/28(MON)		更新日:
 */

//  20200820  インクルードするクラス名をTrotGait.hからTripodGait.hに修正・URG・AxisControlダイアログ関連コメントアウト

#pragma once

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <afxmt.h>

#include "..\..\Kinematics\AsuraX.h"
#include "..\..\Data\AsuraData.h"
#include "..\..\Data\PlanData.h"
#include "..\..\Data\DataHandler.h"

#include "..\..\Plan\PlanParameter.h"
#include "..\..\Plan\PlannerManager.h"
//#include "..\..\Plan\AxisControlPlanner.h"
//
//#include "..\..\Plan\CrawlGaitPlanner.h"
//#include "..\..\Plan\TrotGaitPlanner.h"
#include "..\..\Plan\TripodGaitPlanner.h"  //  20200820
//#include "..\..\Plan\TrackDrivePlanner.h"
//#include "..\..\Plan\ModeChangePlanner.h"
//#include "..\..\Plan\HybridMotionPlanner.h"
//#include "..\..\Plan\URGMotionPlanner.h"		//追加

#include "..\Timer\MultiMediaTimer.h"
#include "TimedMotionProcedure.h"
#include "..\WinUserMessage.h"
//#include "ASURA2GUI/UDP/AsuraUDPThread.h"
#include "..\..\UDP/AsuraUDPThread.h"


/**
 *	----------------------------------------------------------------------
 *		追加のクラスの前方宣言
 *	----------------------------------------------------------------------
 */
//class CAxisControlDialog;  20200820
//class CSerialPortDialog;
class AsuraUDPThread;

/**
 *	----------------------------------------------------------------------
 *		MotionPlanThreadクラス
 *	----------------------------------------------------------------------
 */
class MotionPlanThread : public CWinThread
{
	DECLARE_DYNCREATE(MotionPlanThread)

/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */

/**
 *		Asuraのキネマティクスオブジェクト
 */
	static Asura::AsuraX asuraX;

/**
 *		データオブジェクト
 */
	/// キネマティクスデータ
	static Data::AsuraData asuraXData;
	/// プランデータ
	static Data::PlanData planData;
	/// データハンドラオブジェクト
	static Data::DataHandler dataHandler;

/**
 *		動作計画オブジェクト
 */
	static Plan::PlannerManager		plannerManager;
	//static Plan::AxisControlPlanner	axisControl;

	//static Plan::CrawlGaitPlanner	crawlGait;
	static Plan::TripodGaitPlanner	tripodGait;  //20200820
	//static Plan::TrackDrivePlanner	trackDrive;
	//static Plan::ModeChangePlanner	modeChange;
	//static Plan::HybridMotionPlanner	hybridMotion;
	//static Plan::URGMotionPlanner	URGMotion;		//追加

/**
 *		タイマオブジェクト
 */
	System::TimedMotionProcedure*	pTimedMotionProcedure;
	System::MultiMediaTimer*		pMultiMediaTimer;
	static Plan::TimeManager		timeManager;

/**
 *		ダイアログ関連
 */
	/// ポインタ
	//CAxisControlDialog* pAxisControlDialog;  20200820
	//CSerialPortDialog*	pSerialPortDialog;
	AsuraUDPThread* pUDPThread;
	/// CAxisControlDialog
	//static double axisCtrlData[Plan::AC_DLG_AXIS_NUM];  20200820

/**
 *		フラグ
 */
	/// スレッドが有効かどうか
	BOOL isAlive;

	/// 通信が有効かどうか
	bool isCommAlive;

	//DWORD URGThreadID;  20200820


/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
protected:

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
	/// デフォルトコンストラクタ
	MotionPlanThread();           // 動的生成で使用される protected コンストラクタ
	/// デストラクタ
	virtual ~MotionPlanThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

/**
 *	------------------------------------------------------------
 *		自作関数
 *	------------------------------------------------------------
 */
	/**
	 *	説明
	 *		初期化
	 */
	void initializeMotionPlanThread(void);

	/**
	 *	説明
	 *		終了処理
	 */
	void finalizeMotionPlanThread(void);

	/**
	 *	説明
	 *		スレッドが有効かどうか
	 */
	BOOL isRunning(void) const{return isAlive;}

	/**
	 *	説明
	 *		操作するダイアログへのポインタをセット
	 */
	/// 軸制御ダイアログ
	//void acquireAxisControlDialog(CAxisControlDialog* pDlg);
	/// シリアル通信ダイアログ
	//void acquireSerialPortDialog(CSerialPortDialog* pDlg);

	void MotionPlanThread::acquireAsuraUDPThread(AsuraUDPThread* pDlg);

DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *	------------------------------------------------------------
 */
protected:
/**
 *		カスタムメッセージマップ開始
 */
	/// スレッド終了
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

	/// CAxisControlDialog関連
	/// 脚番号設定
	//afx_msg void OnAxisCtrlDlgLegNo(WPARAM wParam, LPARAM lParam);
	/// 制御モード設定
	//afx_msg void OnAxisCtrlDlgCtrlMode(WPARAM wParam, LPARAM lParam);
	/// スライダー値受信
	//afx_msg void OnAxisCtrlDlgRecieveSldrData(WPARAM wParam, LPARAM lParam);
	/// ダイアログにデータ送信
	//afx_msg void OnAxisCtrlDlgSendData(WPARAM wParam, LPARAM lParam);

	/// 通信関係
	/// データ送信開始
	afx_msg void OnStartDataSending(WPARAM wParam, LPARAM lParam);
	/// データ送信停止
	afx_msg void OnStopDataSending(WPARAM wParam, LPARAM lParam);

	/// URGThreadアドレスセット
	//afx_msg void OnURGThreadSet(WPARAM wParam, LPARAM lParam);

/**
 *		カスタムメッセージマップ終了
 */
};



/**
 *  ファイル名
 *		MultiMediaTimer.cpp
 *  説明
 *		マルチメディアタイマを使用したタイマ(Windows専用)
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/19(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "MultiMediaTimer.h"
#include <iostream>
using namespace std;

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		MultiMediaTimerクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		PlanDaMultiMediaTimertaクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
MultiMediaTimer::MultiMediaTimer(MultiMediaTimerCallback& timerCallback_) : timerCallback(timerCallback_)
{
	/// タイマ性能に関係
	timerPeriod						= 0;
	timerResolution					= 0;
	timerID							= 0;
	timerFrequency					= 0.0;
	performanceFrequency.HighPart	= 0;
	performanceFrequency.LowPart	= 0;

	/// フラグ
	isTimerEnded					= false;
	isTimerWarned					= false;

	/// 開始時間
	startTime						= 0.0;

	/// 検査用のマーカ
	marker = checkMMTimerMarker;

	/// 1秒あたりのインクリメント値を得る
	::QueryPerformanceFrequency(&performanceFrequency);
	/// double値に変換
	timerFrequency = (double)(performanceFrequency.HighPart)*(double)(0xffffffff) + (double)(performanceFrequency.LowPart);

}

/**
 *		デストラクタ
 */
MultiMediaTimer::~MultiMediaTimer()
{
    /// タイマを破棄
	killTimer();
	
	/// 無効な値にセット
	marker = 0;
}

/**
 *	説明
 *		タイマコールバックオブジェクトのセット
 *		タイマを開始する前に必ず行う
 *	引数
 *		コールバック関数へのポインタ
 */
bool MultiMediaTimer::setMultiMediaTimerCallback(MultiMediaTimerCallback& timerCallback_)
{
	timerCallback = timerCallback_;
	return true;
}

/**
 *	説明
 *		タイマのセット
 *	引数
 *		period: 周期
 *		resolution: タイマの分解能
 */
bool MultiMediaTimer::setTimer(UINT period, UINT resolution)
{
	/**
	 *		引数チェック
	 *			指定したタイマ周期が0以下だったら
	 *			指定したタイマ周期が指定したタイマ分解能より小さかったら
	 */
	if ( (period <= 0) || ( (period < resolution) && resolution != 0) )
	{
		cerr << "Error: [MultiMediaTimer::setTimer] Invalid argument\n" << endl;
		return false;
	}

	/// 現在のタイマをクリア
	killTimer();

	/// 警告の識別子を初期化
	isTimerWarned = false;

	/// 最初に識別子を初期化
	timerID = TIMERR_NOERROR;

	/// 利用できるタイマの分解能
	TIMECAPS tc;

	/// タイマの分解能を調査
	MMRESULT mmresult = ::timeGetDevCaps( &tc, sizeof(TIMECAPS) );

	if ( mmresult != TIMERR_NOERROR || mmresult == TIMERR_STRUCT )
	{
		/// 返り値はTIMERR_STRUCTのはず
		timerID = mmresult;

		return false;
	}

	/// 設定したタイマ周期が調査したシステムの最小のタイマ分解能より小さいか
	/// 設定したタイマ周期が調査したシステムの最大のタイマ分解能より大きいか
	if ( (period < tc.wPeriodMin) || (tc.wPeriodMax < period) )
	{
		cerr << "Error: [MultiMediaTimer::setTimer] Invalid timer period\n" << endl;
		isTimerWarned = true;
	}

	/// 設定したタイマ分解能が調査したシステムの最小のタイマ分解能より小さいか
	/// 設定したタイマ分解能が調査したシステムの最大のタイマ分解能より大きいか
	if ( (resolution < tc.wPeriodMin) || (tc.wPeriodMax < resolution) )
	{
		cerr << "Error: [MultiMediaTimer::setTimer] Invalid timer resolution\n" << endl;
		isTimerWarned = true;
	}

	/// タイマの分解能を利用できる最小の分解能にセット
	timerResolution = min( ( max(tc.wPeriodMin, resolution) ), tc.wPeriodMax );

	/// 最小タイマ分解能を設定
	mmresult = ::timeBeginPeriod(timerResolution);
	/// 設定に失敗したら
	if ( mmresult != TIMERR_NOERROR || mmresult == TIMERR_NOCANDO )
	{

		timerResolution	= 0;
		timerID				= mmresult;

		return false;
	}

	/// タイマの周期を利用できる最小周期にセット
	timerPeriod = min( ( max(tc.wPeriodMin, period) ), tc.wPeriodMax);

	/// 指定したデータを使用してタイマイベントを開始
	mmresult = ::timeSetEvent(	timerPeriod,							/// タイマ周期
								timerResolution,						/// タイマ分解能
								multiMediaTimerProcedure,				/// コールバック関数のアドレス
								(DWORD_PTR)this,						/// コールバックデータ
								TIME_PERIODIC | TIME_CALLBACK_FUNCTION	/// タイマの種類
							);

	/// タイマイベントの設定に成功したら
	if ( mmresult != 0 )
	{
		timerCallback.onStart();
		timerID = mmresult;
	}
	/// 失敗判定
	else
	{
		/// タイマ分解能をリセット
		resetPeriodAndResolution();
	
		timerID = TIMERR_NOCANDO;

		return false;
	}

	/// 終了フラグをOFF
	isTimerEnded = false;

	/// 開始時刻を取得
	startTime = getPresentTime();

	return isActive();
}

/**
 *	説明
 *		タイマの破棄
 */
bool MultiMediaTimer::killTimer()
{
	/// クリティカルセクションによる部分制御ロック
	CriticalSectionLock lock(criticalSection);

	if ( isActive() )
	{
		UINT timerID_ = timerID;
		timerID = 0;	/// 強制的に停止

		/// 開始したタイマイベントを破棄
		::timeKillEvent(timerID_);

		/// タイマのコールバックインターフェイスオブジェクトの終了処理
		timerCallback.onStop();

		/// フラグをクリア
		isTimerEnded = false;
	}
	else
		return false;

	/// 周期と分解能をリセット
	resetPeriodAndResolution();

	return true;
}

/**
 *	説明
 *		タイマの周期と分解能のリセット
 */
void MultiMediaTimer::resetPeriodAndResolution(void)
{
	if ( timerResolution != 0 )
	{
		/// 最小タイマ分解能をクリア
		/// setTimerのtimeBeginPeriod 関数の呼び出しとtimeEndPeriod 関数の呼び出しを一致
		::timeEndPeriod(timerResolution);
	}

	timerResolution=0;
	timerPeriod=0;

	return;
}

/**
 *	説明
 *		現在のCPU時刻の取得
 */
double MultiMediaTimer::getPresentTime(void)
{
	LARGE_INTEGER currentTime;

	/// システム起動からの経過時間を取得
	::QueryPerformanceCounter(&currentTime);

	/// double値に変換
	double tics = (double)(currentTime.HighPart)*(double)(0xffffffff) + (double)(currentTime.LowPart);

	return ( (tics / timerFrequency) );
}

/**
 *	------------------------------------------------------------
 *		MultiMediaTimerクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/**
 *	引数
 *		タイマプロシージャのコールバック関数(staticなのはコールバック関数のため)
 *	説明
 *		uID			: タイマID
 *		uMsg		: 不使用
 *		dwUser		: ユーザ定義(大抵thisポインタを入れる)
 *		dw1			: 不使用
 *		dw2			: 不使用
 */
void CALLBACK MultiMediaTimer::multiMediaTimerProcedure(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	MultiMediaTimer* timerPointer = (MultiMediaTimer*)dwUser;

	bool isValidTimer = ( ( timerPointer != NULL ) && ( timerPointer->checkMarker() ) );

	/// 有効なマルチメディアタイマのオブジェクトでない限り呼ばない
	if ( isValidTimer )		/// 有効だったら
		timerPointer->run();
}

/**
 *	説明
 *		タイマのコールバック関数で呼び出す処理
 */
void MultiMediaTimer::run(void)
{
	/// この1回前の呼び出しで終わらせるように指示されたら
	if ( isTimerEnded )
	{
		killTimer();
	}

	/**
	 *		この関数を呼ぶ前にisActive()がtrueかどうか確認
	 *		オーバーライドされたOnStart()が1周期より多くかかったらfalseになる可能性がある
	 *		OnStart()は最初にこの関数が呼ばれる前に完了している必要がある
	 */
	if ( !isTimerEnded && isActive() )
	{
		/// onMultiMediaTimerが成功したらisTimerEndedはfalseのはず
		isTimerEnded = !timerCallback.onMultiMediaTimer();
	}

	return;
}

}	/// end of namespace System
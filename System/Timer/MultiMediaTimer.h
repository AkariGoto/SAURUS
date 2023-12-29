/**
 *  ファイル名
 *		MultiMediaTimer.h
 *  説明
 *		マルチメディアタイマを使用したタイマ(Windows専用)
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/19(SAT)
 */

#ifndef __MultiMediaTimer_h__
#define __MultiMediaTimer_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>

#include "..\CriticalSection.h"

/**
 *		winmm.libを使用
 *		リンカへ通知
 */
#pragma comment (lib, "winmm.lib")

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		定数、マクロ宣言
 *	----------------------------------------------------------------------
 */

/**
 *		呼び出すオブジェクトが有効かどうかチェックするマーカ
 *		0xF5F5F5F5はillegalなアドレスの値
 *		冗長チェック用
 */
const   DWORD   checkMMTimerMarker = 0xF5F5F5F5;


/**
 *	----------------------------------------------------------------------
 *		MultiMediaTimerCallbackクラス
 *			コールバック関数のインターフェイス
 *			繰り返し処理をTimerクラスから離して
 *			run()からの呼び出しがマルチメディタイマ
 *			クラスの関数の突発的な呼び出しを防ぐ
 *	----------------------------------------------------------------------
 */
/**
 *		タイマーをセットすると一定の間隔で
 *		このコールバック関数が実行される.
 *
 *		つまり1回前のコールバックが終了なくても
 *		次のコールが呼び出されるので注意
 */
class MultiMediaTimerCallback
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
	/// デフォルトコンストラクタ
	MultiMediaTimerCallback(){}
	/// デストラクタ
	virtual ~MultiMediaTimerCallback(){}

/**
 *	----------------------------------------
 *	タイマで実際に処理される
 *	----------------------------------------
 */
	/// ルーチン開始時に呼び出す処理
	virtual void	onStart(void){}
	/// ルーチン終了時に呼び出す処理
	virtual void	onStop(void){}
	/**
	 *		オーバーライド必須のタイマルーチン
	 *		終了時にはfalseを返す
	 */
	virtual bool	onMultiMediaTimer(void)=0;//false;

};	/// end of class MultiMediaTimerCallback

/**
 *		//////////////////////////////////////////////////
 *				MultiMediaTimerクラス
 *		//////////////////////////////////////////////////
 */
/**
 *	----------------------------------------------------------------------
 *		MultiMediaTimerクラス
 *	----------------------------------------------------------------------
 */
/**
 *		使い方
 *			MultiMediaTimerCallbackクラスを派生させて, 
 *			onMultiMediaTimer()をオーバーライドする
 *
 *			setTimer()が呼び出しが成功すれば, period毎に
 *			MultiMediaTimerCallbackクラスの	onMultiMediaTimerが呼ばれる
 */
class MultiMediaTimer
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
private:

/**
 *	----------------------------------------
 *	タイマのための各種変数
 *	----------------------------------------
 */
/**
 *	オブジェクト変数
 *		timerCallback				: コールバック関数内で処理される関数
 *		criticalSection				: クリティカルセクションオブジェクト
 */
	MultiMediaTimerCallback&		timerCallback;
	CriticalSection						criticalSection;

/**
 *	タイマの性能に関係する変数
 *		period						: タイマイベント周期[msec]
 *		resolution					: タイマイベントの分解能[msec]
 *		timerID						: タイマイベントの識別子
 *		frequency					: performanceFrequencyの単位変換値[sec]
 *		performanceFrequency	: システム起動からの経過時間[msec]
 */
	UINT			timerPeriod;
	UINT			timerResolution;
	MMRESULT		timerID;
	double			timerFrequency;
	LARGE_INTEGER	performanceFrequency;


/**
 *	フラグ変数
 *		isTimerEnded				: タイマが終わっているかどうか
 *		isTimerWarned				: 警告を受けたかどうか
 */
	bool	isTimerEnded;
	bool	isTimerWarned;

/**
 *	マルチメディアタイマの開始時間
 */
	double startTime;

/**
 *	検査用のマーカ
 */
	DWORD	marker;

protected:

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
	/// デフォルトコンストラクタ
	MultiMediaTimer(MultiMediaTimerCallback& timerCallback_);
	/// デストラクタ
	virtual ~MultiMediaTimer();

/**
 *	説明
 *		アクセス関数
 */
	UINT	getPeriod(void)	 const{ return timerPeriod; }
	UINT	getResolution(void) const{ return timerResolution; }
	double	getStartTime(void) const{ return startTime; }

/**
 *	説明
 *		タイマーが動いているかどうか
 */
	bool isActive(void) const{ return (timerID != 0); }

/**
 *	説明
 *		タイマコールバックオブジェクトのセット
 *		タイマを開始する前に必ず行う
 *	引数
 *		コールバック関数へのポインタ
 */
	bool setMultiMediaTimerCallback(MultiMediaTimerCallback& timerCallback_);

/**
 *	説明
 *		タイマのセット
 *	引数
 *		period: 周期
 *		resolution: タイマの分解能
 */
	bool setTimer(UINT period, UINT resolution = 1);

/**
 *	説明
 *		タイマの破棄
 */
	bool killTimer(void);

/**
 *	説明
 *		タイマの周期と分解能のリセット
 */
	void resetPeriodAndResolution(void);

/**
 *	説明
 *		現在のCPU時刻の取得
 */
	double getPresentTime(void);

private:
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
	static void CALLBACK multiMediaTimerProcedure(
									UINT uID, 
									UINT uMsg,
									DWORD_PTR dwUser, 
									DWORD_PTR dw1, 
									DWORD_PTR dw2
									);

/**
 *	説明
 *		タイマのコールバック関数で呼び出す処理
 */
	void	run(void);

/**
 *	説明
 *		冗長検査用
 */
	bool	checkMarker(void) const{return ( marker == checkMMTimerMarker );}

protected:
};	/// end of class MultiMediaTimer

}

#endif // __MMTimer_h__
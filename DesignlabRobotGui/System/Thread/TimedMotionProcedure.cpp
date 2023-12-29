/**
 *  File: TimedMotionProcedure.cpp
 *
 *  Description: タイマで周期的に呼ばれる歩容計画を行うための手順
 *				 MultiMediaTimerCallbackクラスから派生
 *
 *  Created: 2007/05/12(Sat)
 *  Updated: 2007/05/191(Sat)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

 //  20200820  URG関連コメントアウト

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "TimedMotionProcedure.h"
#include "..\WinUserMessage.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		TimedMotionProcedureクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		TimedMotionProcedureクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// コンストラクタ
TimedMotionProcedure::TimedMotionProcedure()
{
	targetThreadID = 0;
	//URGtargetThreadID = 0;  20200820
}

/// デストラクタ
TimedMotionProcedure::~TimedMotionProcedure()
{
}

/**
 *	説明
 *		呼び出し先のスレッドのIDを設定
 */
void TimedMotionProcedure::setThreadID(DWORD id)
{
	if ( id != 0 )
		targetThreadID = id;

	return;
}
/**
 *	説明
 *		呼び出し先のスレッドのIDを設定		//URG用
 */
/*  20200820
void TimedMotionProcedure::URGsetThreadID(DWORD id)
{
	if ( id != 0 )
		URGtargetThreadID = id;

	return;
}
*/

/**
 *	説明
 *		タイマコールバック開始時に呼び出される
 */
void TimedMotionProcedure::onStart(void)
{
	return;
}

/**
 *	説明
 *		タイマコールバック終了時に呼び出される
 */
void TimedMotionProcedure::onStop(void)
{
	return;
}

/**
 *	説明
 *		オーバーライド必須のタイマルーチン
 *		終了時にはfalseを返す
 */
bool TimedMotionProcedure::onMultiMediaTimer(void)
{
	///200msをカウントするフラグ
	//static int URG_200ms_Flag=0;  20200820

	if ( targetThreadID != 0 )
	{
		/// 指定したスレッドへメッセージ通知
		::PostThreadMessage(targetThreadID, WM_PLAN_GENERATE_MOTION, NULL, NULL);
	}

	///URGによる測距を行う
	/*  20200820
	if ( URGtargetThreadID != 0 )
	{
		if(URG_200ms_Flag == 10)
		{
			::PostThreadMessage(URGtargetThreadID, WM_URG_GENERATE_MOTION, NULL, NULL);
			URG_200ms_Flag=0;
		}
		URG_200ms_Flag=URG_200ms_Flag+1;
	}
	*/

	return true;
}

}	/// end of namespace System;
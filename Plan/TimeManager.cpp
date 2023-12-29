/**
 *  ファイル名
 *		TimeManager.cpp
 *  説明
 *		計画のための時間管理をするクラス
 *  日付
 *		作成日: 2007/09/01(SAT)		更新日: 2008/10/04(SAT)
 */

#include "TimeManager.h"

using namespace std;

namespace Plan
{
/**
 *	----------------------------------------------------------------------
 *		TimeManagerクラス
 *	----------------------------------------------------------------------
 */
/**
 *	------------------------------------------------------------
 *		TimeManagerクラスのstatic変数の初期化
 *	------------------------------------------------------------
 */
double TimeManager::realTime = 0.0;

/**
 *	------------------------------------------------------------
 *		TimeManagerクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::setRealTime(double time)
{
	/// 引数チェック
	if ( time<=0 )
	{
		cerr << "Error: [TimeManager::SetRealTime] Invalid argument\n" << endl;
		return;
	}

	realTime = time;
	return;

}

}	/// end of namespace Plan
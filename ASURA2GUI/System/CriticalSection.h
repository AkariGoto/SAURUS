/**
 *  ファイル名
 *		CriticalSection.h
 *  説明
 *		クリティカルセクションを使った同期処理(Windows専用)
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/19(SAT)
 */

#ifndef __CriticalSection_h__
#define __CriticalSection_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <cassert>
#include <iostream>
#include "LockObject.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		CriticalSectionクラス
 *		（Windows Critical Sectionのwrapperクラス）
 *	----------------------------------------------------------------------
 */
class CriticalSection : public LockObject
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
protected:
/**
 *	クリティカルセクション構造体
 */
	CRITICAL_SECTION criticalSection;

/**
 *	ロックを何回行ったかのカウンタ
 */
	int lockCount;

/**
 *	ロックするスレッドID
 */
	unsigned int threadID;

/**
 *		無効なスレッドIDの規定値
 */
	static const unsigned int invalidThreadID = 0xffffffff;

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
	CriticalSection() : lockCount(0), threadID(invalidThreadID)
    {
		/// クリティカルセクションオブジェクトの初期化
		::InitializeCriticalSection( &criticalSection );
	}

    ~CriticalSection()    
    {
		assert(threadID == invalidThreadID);
		assert(lockCount == 0);
		/// クリティカルセクションオブジェクトの放棄(後処理)
		::DeleteCriticalSection( &criticalSection );
	}

/**
 *	説明
 *		ロック
 */
	virtual void lock(void)
	{
		::EnterCriticalSection(&criticalSection);
		lockCount++;
		threadID = ::GetCurrentThreadId();
	}

/**
 *	説明
 *		アンロック
 */
	virtual void unlock(void)
	{
		assert(lockCount > 0);
		lockCount--;
		
		if ( lockCount == 0 )
			threadID = invalidThreadID;

		::LeaveCriticalSection(&criticalSection);
	}

/**
 *	説明
 *		現行スレッドによりロックされているか
 *	説明
 *		true: ロックされている
 */
	virtual bool isLockedByCurrentThread() const
	{
		return ::GetCurrentThreadId() == threadID;
	}

};	/// end of class CriticalSection

/**
 *	----------------------------------------------------------------------
 *		CriticalSectionLockクラス
 *	----------------------------------------------------------------------
 */
class CriticalSectionLock
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
/**
 *	CriticalSectionクラスのオブジェクト
 */
    CriticalSection&		criticalSection;

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
	/**
	 *	デフォルトコンストラクタ
	 *	生成時にブロックする
	 */
	CriticalSectionLock(CriticalSection& cs)
	: criticalSection( cs )
	{
		criticalSection.lock();
	}

	/**
	 *	デストラクタ
	 *	ブロック解除
	 */
    ~CriticalSectionLock()
	{
		criticalSection.unlock();
	}
};	/// end of class CriticalSectionLock

}	/// end of namespace System;

#endif // __CriticalSection_h__
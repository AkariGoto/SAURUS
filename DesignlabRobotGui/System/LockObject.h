/**
 *  ファイル名
 *		LockObject.h
 *  説明
 *		ロックオブジェクトを提供するクラス(Windows専用)
 *  日付
 *		作成日: 2007/05/12(SAT)		更新日: 2007/05/19(SAT)
 */

#ifndef __LockObject_h__
#define __LockObject_h__

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		LockObjectクラス
 *	----------------------------------------------------------------------
 */
class LockObject
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
	LockObject() {};

	/// デストラクタ
	~LockObject() {};    

	/**
	 *	説明
	 *		ロック
	 */
	virtual void lock(void) = 0;

	/**
	 *	説明
	 *		ロック解除
	 */
	virtual void unlock(void) = 0;

	/**
	 *	説明
	 *		現行スレッドによりロックされているか
	 *	返却値
	 *		true: ロックされている
	 */
	virtual bool isLockedByCurrentThread() const = 0;

private:
	/// コピーコンストラクタの禁止
	LockObject(const LockObject& copy);

	/// 代入演算子の禁止
	LockObject& operator =(const LockObject& copy);

};	/// end of class LockObject

}	/// end of namespace System

#endif // __LockObject_h__
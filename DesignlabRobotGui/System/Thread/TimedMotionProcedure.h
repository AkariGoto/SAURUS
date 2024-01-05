
#ifndef __TimedMotionProcedure_h__
#define __TimedMotionProcedure_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "System/Timer/MultiMediaTimer.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		TimedMotionProcedureクラス
 *	----------------------------------------------------------------------
 */
class TimedMotionProcedure : public MultiMediaTimerCallback
{
    /**
     *	------------------------------------------------------------
     *		メンバ変数
     *	------------------------------------------------------------
     */
private:
    /**
     *	キューを入れるスレッドID
     */
    DWORD targetThreadID;
    //DWORD URGtargetThreadID;		//URG用  20200820

  /**
   *	TimedMotionProcedureオブジェクトを呼び出すタイマへのポインタ
   */
    MultiMediaTimer* timerPointer;

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
     /// コンストラクタ
    TimedMotionProcedure();
    /// デストラクタ
    virtual ~TimedMotionProcedure();

    /**
     *	----------------------------------------
     *	タイマオブジェクトの取得・解放
     *	----------------------------------------
     */
     /// 取得
    void acquireTimer(MultiMediaTimer* timer);
    /// 解放
    void releaseTimer(void);

    /**
     *	----------------------------------------
     *	スレッドIDの取得・解放
     *	----------------------------------------
     */
     /// 取得
    void	setThreadID(DWORD id);
    //void	URGsetThreadID(DWORD id);  20200820
    DWORD	getThreadID(void) const { return (targetThreadID); }

    /**
     *		MultiMediaTimerCallbackクラスの仮想関数
     *		タイマで実際に処理される
     */
     /**
      *	----------------------------------------
      *	MultiMediaTimerCallbackクラスのオーバーライド関数
      *	タイマで実際に処理される
      *	----------------------------------------
      */
      /// ルーチン開始時に呼び出す処理
    virtual void	onStart(void);
    /// ルーチン終了時に呼び出す処理
    virtual void	onStop(void);
    /**
     *	説明
     *		オーバーライド必須のタイマルーチン
     *		終了時にはfalseを返す
     */
    virtual bool	onMultiMediaTimer(void);


};	/// end of class TimedMotionProcedure

}	/// end of namespace System;

#endif /// __TimedMotionProcedure_h__
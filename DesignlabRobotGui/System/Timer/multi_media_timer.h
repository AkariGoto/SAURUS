
#ifndef DESIGNLABROBOTGUI_SYSTEM_TIMER_MULTI_MEDIA_TIMER_H_
#define DESIGNLABROBOTGUI_SYSTEM_TIMER_MULTI_MEDIA_TIMER_H_

#include <Windows.h>
#include <MMSystem.h>

#include "System/critical_section.h"


// winmm.libを使用することをリンカへ通知．

#pragma comment (lib, "winmm.lib")


namespace designlab_robot_gui::system
{

//! 呼び出すオブジェクトが有効かどうかチェックするマーカ．
//! 0xF5F5F5F5 は illegal なアドレスの値．
//! 冗長チェック用．
constexpr DWORD kCheckMMTimerMarker = 0xF5F5F5F5;



//! @class MultiMediaTimerCallback
//! @brief タイマコールバック関数のインターフェイス．
//! @details
//! インターフェースは純粋仮想関数のみを持ち，メンバ変数を持たない．
//! 純粋仮想関数は実装する必要がなく，継承先で実装する．
//! そのため，このクラスのインスタンスを生成することはできず，必ず継承して使用する．
//!
//! 繰り返し処理を MultiMediaTimer クラスから離して，
//! run() からの呼び出しがマルチメディタイマクラスの関数の突発的な呼び出しを防ぐ．
//! タイマーをセットすると一定の間隔でこのコールバック関数が実行される．
//! つまり1回前のコールバックが終了しなくても次のコールが呼び出されるので注意．
class MultiMediaTimerCallback
{
public:
    //!< デフォルトコンストラクタは default (何もしない)．
    MultiMediaTimerCallback() = default;

    //!< デストラクタは default (何もしない)．
    virtual ~MultiMediaTimerCallback() = default;


    //! @brief ルーチン開始時に呼び出す処理．
    virtual void	OnStart() = 0;

    //! @brief ルーチン終了時に呼び出す処理．
    virtual void	OnStop() = 0;

    //! @brief タイマルーチン．
    //! @retval true 継続する．
    //! @retval false 終了する．
    virtual bool	OnMultiMediaTimer() = 0;
};


/**
 *		使い方
 *			MultiMediaTimerCallbackクラスを派生させて,
 *			OnMultiMediaTimer()をオーバーライドする
 *
 *			setTimer()が呼び出しが成功すれば, period毎に
 *			MultiMediaTimerCallbackクラスの	onMultiMediaTimerが呼ばれる
 */
class MultiMediaTimer
{
public:
    MultiMediaTimer(MultiMediaTimerCallback& timerCallback_);

    virtual ~MultiMediaTimer();


    UINT	getPeriod()	 const { return timerPeriod; }
    UINT	getResolution() const { return timerResolution; }
    double	getStartTime() const { return startTime; }

    /**
     *	説明
     *		タイマーが動いているかどうか
     */
    bool isActive() const { return (timerID != 0); }

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
    bool killTimer();

    /**
     *	説明
     *		タイマの周期と分解能のリセット
     */
    void resetPeriodAndResolution();

    /**
     *	説明
     *		現在のCPU時刻の取得
     */
    double getPresentTime();

private:
    //! @brief タイマプロシージャのコールバック関数．
    //! (staticなのはコールバック関数のため)
    //! @param[in] uID タイマID．
    //! @param[in] uMsg 不使用．
    //! @param[in] dwUser ユーザ定義(大抵 this ポインタを入れる)．
    //! @param[in] dw1 不使用．
    //! @param[in] dw2 不使用．
    static void CALLBACK multiMediaTimerProcedure(UINT uID, UINT uMsg,
                                                  DWORD_PTR dwUser, DWORD_PTR dw1,
                                                  DWORD_PTR dw2);

    //! @brief タイマのコールバック関数で呼び出す処理．
    void	run();

    //! @brief 冗長検査用．
    bool	checkMarker() const
    {
        return (marker == kCheckMMTimerMarker);
    }


    MultiMediaTimerCallback& timerCallback;     //!< コールバック関数内で処理される関数．
    CriticalSection criticalSection;            //!< クリティカルセクションオブジェクト．

    UINT timerPeriod;       //!< タイマイベント周期 [msec]
    UINT timerResolution;   //!< タイマイベントの分解能 [msec]
    MMRESULT timerID;       //!< タイマイベントの識別子
    double timerFrequency;  //!< performanceFrequency の単位変換値 [sec]
    LARGE_INTEGER performanceFrequency;  //!< システム起動からの経過時間 [msec]

    bool isTimerEnded;  //!< タイマが終わっているかどうか
    bool isTimerWarned; //!< 警告を受けたかどうか

    double startTime;  //!< タイマの開始時間

    DWORD	marker;  //!< 冗長検査用のマーカ．
};

}  // namespace designlab_robot_gui::system


#endif // DESIGNLABROBOTGUI_SYSTEM_TIMER_MULTI_MEDIA_TIMER_H_

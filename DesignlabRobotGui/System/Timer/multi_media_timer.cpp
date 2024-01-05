
#include "pch.h"

#include "System/object_locker.h"
#include "System/Timer/multi_media_timer.h"


namespace designlab_robot_gui::system
{

MultiMediaTimer::MultiMediaTimer(MultiMediaTimerCallback& timerCallback_) :
    timerCallback(timerCallback_)
{
    /// タイマ性能に関係
    timerPeriod = 0;
    timerResolution = 0;
    timerID = 0;
    timerFrequency = 0.0;
    performanceFrequency.HighPart = 0;
    performanceFrequency.LowPart = 0;

    /// フラグ
    isTimerEnded = false;
    isTimerWarned = false;

    /// 開始時間
    startTime = 0.0;

    /// 検査用のマーカ
    marker = kCheckMMTimerMarker;

    /// 1秒あたりのインクリメント値を得る
    ::QueryPerformanceFrequency(&performanceFrequency);
    /// double値に変換
    timerFrequency = (double)(performanceFrequency.HighPart) * (double)(0xffffffff) + (double)(performanceFrequency.LowPart);

}

MultiMediaTimer::~MultiMediaTimer()
{
    /// タイマを破棄
    killTimer();

    /// 無効な値にセット
    marker = 0;
}

bool MultiMediaTimer::setMultiMediaTimerCallback(MultiMediaTimerCallback& timerCallback_)
{
    timerCallback = timerCallback_;
    return true;
}

bool MultiMediaTimer::setTimer(UINT period, UINT resolution)
{
    // 指定したタイマ周期が0以下だったら，
    // または，指定したタイマ周期が指定したタイマ分解能より小さかったら
    if ((period <= 0) || ((period < resolution) && resolution != 0))
    {
        // エラーは cout ではなく cerr に出力すること．
        std::cerr << "Error: [MultiMediaTimer::setTimer] Invalid argument" << std::endl;

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
    MMRESULT mm_result = ::timeGetDevCaps(&tc, sizeof(TIMECAPS));

    if (mm_result != TIMERR_NOERROR || mm_result == TIMERR_STRUCT)
    {
        /// 返り値はTIMERR_STRUCTのはず
        timerID = mm_result;

        return false;
    }

    // 設定したタイマ周期が調査したシステムの最小のタイマ分解能より小さいか
    // 設定したタイマ周期が調査したシステムの最大のタイマ分解能より大きいか
    if ((period < tc.wPeriodMin) || (tc.wPeriodMax < period))
    {
        std::cerr << "Error: [MultiMediaTimer::setTimer] Invalid timer period\n" << std::endl;
        isTimerWarned = true;
    }

    // 設定したタイマ分解能が調査したシステムの最小のタイマ分解能より小さいか
    // 設定したタイマ分解能が調査したシステムの最大のタイマ分解能より大きいか
    if ((resolution < tc.wPeriodMin) || (tc.wPeriodMax < resolution))
    {
        std::cerr << "Error: [MultiMediaTimer::setTimer] Invalid timer resolution\n" << std::endl;
        isTimerWarned = true;
    }

    /// タイマの分解能を利用できる最小の分解能にセット
    timerResolution = min((max(tc.wPeriodMin, resolution)), tc.wPeriodMax);

    /// 最小タイマ分解能を設定
    mm_result = ::timeBeginPeriod(timerResolution);
    /// 設定に失敗したら
    if (mm_result != TIMERR_NOERROR || mm_result == TIMERR_NOCANDO)
    {
        timerResolution = 0;
        timerID = mm_result;

        return false;
    }

    // タイマの周期を利用できる最小周期にセット
    timerPeriod = min((max(tc.wPeriodMin, period)), tc.wPeriodMax);

    // 指定したデータを使用してタイマイベントを開始
    mm_result = ::timeSetEvent(timerPeriod, timerResolution, multiMediaTimerProcedure,
                              (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

    if (mm_result != 0)
    {
        // タイマイベントの設定に成功したら

        timerCallback.OnStart();
        timerID = mm_result;
    }
    else
    {
        // 失敗判定

        // タイマ分解能をリセット
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

bool MultiMediaTimer::killTimer()
{
    /// クリティカルセクションによる部分制御ロック
    designlab_robot_gui::system::ObjectLocker Lock(&criticalSection);

    if (isActive())
    {
        UINT timerID_ = timerID;
        timerID = 0;	/// 強制的に停止

        /// 開始したタイマイベントを破棄
        ::timeKillEvent(timerID_);

        /// タイマのコールバックインターフェイスオブジェクトの終了処理
        timerCallback.OnStop();

        /// フラグをクリア
        isTimerEnded = false;
    }
    else
    {
        return false;
    }

    /// 周期と分解能をリセット
    resetPeriodAndResolution();

    return true;
}

void MultiMediaTimer::resetPeriodAndResolution()
{
    if (timerResolution != 0)
    {
        // 最小タイマ分解能をクリア
        // setTimer の timeBeginPeriod 関数の呼び出しと timeEndPeriod 関数の呼び出しを一致
        ::timeEndPeriod(timerResolution);
    }

    timerResolution = 0;
    timerPeriod = 0;

    return;
}

double MultiMediaTimer::getPresentTime()
{
    LARGE_INTEGER currentTime;

    /// システム起動からの経過時間を取得
    ::QueryPerformanceCounter(&currentTime);

    /// double値に変換
    double tics = (double)(currentTime.HighPart) * (double)(0xffffffff) + (double)(currentTime.LowPart);

    return ((tics / timerFrequency));
}

void CALLBACK MultiMediaTimer::multiMediaTimerProcedure(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    MultiMediaTimer* timerPointer = (MultiMediaTimer*)dwUser;

    bool isValidTimer = ((timerPointer != NULL) && (timerPointer->checkMarker()));

    /// 有効なマルチメディアタイマのオブジェクトでない限り呼ばない
    if (isValidTimer)		/// 有効だったら
        timerPointer->run();
}

void MultiMediaTimer::run()
{
    /// この1回前の呼び出しで終わらせるように指示されたら
    if (isTimerEnded)
    {
        killTimer();
    }

    /**
     *		この関数を呼ぶ前に isActive() が trueかどうか確認
     *		オーバーライドされた OnStart() が1周期より多くかかったら false になる可能性がある
     *		OnStart() は最初にこの関数が呼ばれる前に完了している必要がある
     */
    if (!isTimerEnded && isActive())
    {
        /// onMultiMediaTimer が成功したら isTimerEnded は false のはず
        isTimerEnded = !timerCallback.OnMultiMediaTimer();
    }

    return;
}

}  // namespace designlab_robot_gui::system

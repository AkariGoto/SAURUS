
#ifndef DESIGNLABROBOTGUI_SYSTEM_WIN_USER_MESSAGE_H_
#define DESIGNLABROBOTGUI_SYSTEM_WIN_USER_MESSAGE_H_


// マクロによって展開される値には () をつけること．
// つけないと，マクロの展開順序によって意図しない結果になる．
// 例えば，#define THREE 1 + 2 としておいて，THREE * 2 と書いた場合，
// 6 になると思っているかもしれないが，実際には 1 + 2 * 2 となり，5 になってしまう．


// シリアル通信

#define WM_COMM_BREAK_DETECTED      (WM_APP + 101)  //!< 入力において中断点が検知できたメッセージ．
#define WM_COMM_CTS_DETECTED        (WM_APP + 102)  //!< CTS (clear-to-send) 信号による状態変化のメッセージ．
#define WM_COMM_DSR_DETECTED        (WM_APP + 103)  //!< DSR (data-set-ready) 信号による状態変化のメッセージ．
#define WM_COMM_ERR_DETECTED        (WM_APP + 104)  //!< LI (Line-status) エラー. LIエラーは CE_FRAME , CE_OVERRUN , CE_RXPARITY ．
#define WM_COMM_RING_DETECTED       (WM_APP + 105)  //!< RI (ring indicator) が検知されたメッセージ.
#define WM_COMM_RLSD_DETECTED       (WM_APP + 106)  //!< RLSD (receive-line-signal-detect) 信号による状態変化のメッセージ.
#define WM_COMM_RXCHAR              (WM_APP + 107)  //!< 文字 (character) が入力バッファに受信された．
#define WM_COMM_RXFLAG_DETECTED     (WM_APP + 108)  //!< イベント (character) が入力バッファに受信された．
#define WM_COMM_TXEMPTY_DETECTED    (WM_APP + 109)  //!< 出力バッファの最後の文字が送られた．


// スレッドへのメッセージ．
// ドキュメント・ビュークラスから， MotionPlan スレッドへのメッセージ．

#define WM_PLAN_END_THREAD          (WM_APP + 110)  //!< スレッド終了．
#define WM_PLAN_REQUEST_VIEW_DATA   (WM_APP + 111)  //!< Viewデータからのデータ要求．
#define WM_PLAN_SETUP_MOTION        (WM_APP + 112)  //!< Docクラスからの動作準備．
#define WM_PLAN_START_MOTION        (WM_APP + 113)  //!< Viewクラスからの動作開始．
#define WM_PLAN_STOP_MOTION         (WM_APP + 114)  //!< Viewクラスからの動作停止．
#define WM_PLAN_GENERATE_MOTION     (WM_APP + 115)  //!< タイマコールバッククラスからの動作生成．
#define WM_PLAN_COMM_START          (WM_APP + 116)  //!< 通信開始．
#define WM_PLAN_COMM_STOP           (WM_APP + 117)  //!< 通信停止．


// スレッドからのメッセージ．
// MotionPlanスレッドからのメッセージ．

#define WM_PLAN_SENDING_VIEW_DATA   (WM_APP + 122)  //!< Viewクラスへのデータ送信．
// #define WM_PLAN_SENDING_AC_DATA  (WM_APP + 123)  //!< AxisControlダイアログへのデータ送信．
#define WM_PLAN_SENDING_COMM_DATA   (WM_APP + 124)  //!< シリアル通信ダイアログへデータ送信．


// ダイアログへのメッセージ．
// シリアル通信ダイアログ．
// ビュークラスからのメッセージ．

#define WM_DLG_COMM_START_LISTEN    (WM_APP + 125)  //!< シリアル通信開始．
#define WM_DLG_COMM_STOP_LISTEN     (WM_APP + 126)  //!< シリアル通信停止．
#define WM_DLG_COMM_DISPLAY         (WM_APP + 127)  //!< 表示 / 非表示のメッセージ．
#define WM_DLG_COMM_START_LOGGING   (WM_APP + 128)  //!< ログ開始のメッセージ．
#define WM_DLG_COMM_STOP_LOGGING    (WM_APP + 129)  //!< ログ停止のメッセージ．
#define WM_DLG_COMM_UPDATE          (WM_APP + 130)  //!< 画面更新要求のメッセージ．
#define WM_DLG_UDPTHREAD_END        (WM_APP + 131)  //!< AsuraUdpThread の破棄メッセージ．


// ドキュメント・ビュークラスから MotionPlan スレッドへのメッセージ．

#define WM_PLAN_URGTHREAD_SET       (WM_APP + 137)  //!< URGTHREAD のアドレスセット．

// ドキュメント・ビュークラスから DataString スレッドへのメッセージ．

#define WM_DATASRTING_END_THREAD        (WM_APP + 138)  //!< スレッド終了．
#define WM_DATASRTING_SETUP_MOTION      (WM_APP + 139)  //!< Docクラスからの動作準備．
#define WM_DATASRTING_START_MOTION      (WM_APP + 140)  //!< Viewクラスからの動作開始．
#define WM_DATASRTING_STOP_MOTION       (WM_APP + 141)  //!< Viewクラスからの動作停止．
#define WM_DATASRTING_GENERATE_MOTION   (WM_APP + 142)  //!< URGスレッドからの動作生成．


#endif  // DESIGNLABROBOTGUI_SYSTEM_WIN_USER_MESSAGE_H_

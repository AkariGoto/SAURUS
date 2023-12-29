/**
 *  ファイル名
 *		WinUserMessage.h
 *  説明
 *		ユーザ定義のWindowsメッセージ）
 *  日付
 *		作成日: 2007/04/01(SAT)		更新日: 2008/07/26(SAT)
 */

//  20200820  URG・AxisControlダイアログ関連コメントアウト

/**
 *		通信関係
 */
/// シリアル通信
#define	WM_COMM_BREAK_DETECTED			WM_APP+101		/// 入力において中断点が検知できたメッセージ
#define	WM_COMM_CTS_DETECTED			WM_APP+102		/// CTS (clear-to-send) 信号による状態変化のメッセージ 
#define	WM_COMM_DSR_DETECTED			WM_APP+103		/// DSR (data-set-ready) 信号による状態変化のメッセージ 
#define	WM_COMM_ERR_DETECTED			WM_APP+104		/// LI (Line-status) エラー. LIエラーはCE_FRAME, CE_OVERRUN, CE_RXPARITY. 
#define	WM_COMM_RING_DETECTED			WM_APP+105		/// RI (ring indicator) が検知されたメッセージ. 
#define	WM_COMM_RLSD_DETECTED			WM_APP+106		/// RLSD (receive-line-signal-detect) 信号による状態変化のメッセージ. 
#define	WM_COMM_RXCHAR					WM_APP+107		/// 文字 (character) が入力バッファに受信された 
#define	WM_COMM_RXFLAG_DETECTED			WM_APP+108		/// イベント (character) が入力バッファに受信された  
#define	WM_COMM_TXEMPTY_DETECTED		WM_APP+109		/// 出力バッファの最後の文字が送られた.


/**
 *		スレッドへのメッセージ
 */
/// ドキュメント・ビュークラスからMotionPlanスレッドへのメッセージ
#define	WM_PLAN_END_THREAD				WM_APP+110		/// スレッド終了
#define	WM_PLAN_REQUEST_VIEW_DATA		WM_APP+111		/// Viewデータからのデータ要求
#define	WM_PLAN_SETUP_MOTION			WM_APP+112		/// Docクラスからの動作準備
#define	WM_PLAN_START_MOTION			WM_APP+113		/// Viewクラスからの動作開始
#define	WM_PLAN_STOP_MOTION				WM_APP+114		/// Viewクラスからの動作停止
#define	WM_PLAN_GENERATE_MOTION			WM_APP+115		/// タイマコールバッククラスからの動作生成
#define	WM_PLAN_COMM_START				WM_APP+116		/// 通信開始
#define	WM_PLAN_COMM_STOP				WM_APP+117		/// 通信停止

/// AxisControlダイアログからMotionPlanスレッドへのメッセージ
/*  20200820
#define	WM_PLAN_AC_LEG_NO				WM_APP+118		/// 脚番号の通知
#define	WM_PLAN_AC_CTRL_MODE			WM_APP+119		/// 制御モードの通知
#define	WM_PLAN_AC_SENDING_SLDR_DATA	WM_APP+120		/// スライダデータの送信
#define	WM_PLAN_AC_REQUEST_DATA			WM_APP+121		/// データを要求
*/

/*
 *		スレッドからのメッセージ
 */
/// MotionPlanスレッドからのメッセージ
#define	WM_PLAN_SENDING_VIEW_DATA		WM_APP+122		/// Viewクラスへのデータ送信
//#define	WM_PLAN_SENDING_AC_DATA			WM_APP+123		/// AxisControlダイアログへのデータ送信  20200820
#define	WM_PLAN_SENDING_COMM_DATA		WM_APP+124		/// シリアル通信ダイアログへデータ送信

/**
 *		ダイアログへのメッセージ
 */
/// シリアル通信ダイアログ
/// ビュークラスからのメッセージ
#define		WM_DLG_COMM_START_LISTEN	WM_APP+125		/// シリアル通信開始
#define		WM_DLG_COMM_STOP_LISTEN		WM_APP+126		/// シリアル通信停止
#define		WM_DLG_COMM_DISPLAY			WM_APP+127		/// 表示/非表示のメッセージ
#define		WM_DLG_COMM_START_LOGGING	WM_APP+128		/// ログ開始のメッセージ
#define		WM_DLG_COMM_STOP_LOGGING	WM_APP+129		/// ログ停止のメッセージ
#define		WM_DLG_COMM_UPDATE			WM_APP+130		/// 画面更新要求のメッセージ
#define		WM_DLG_UDPTHREAD_END		WM_APP+131		///	AsuraUDPThreadの破棄メッセージ

/// AxisControlダイアログへのメッセージ
//#define		WM_DLG_AC_DISPLAY			WM_APP+131		/// 表示/非表示のメッセージ  20200820


/// ドキュメント・ビュークラスからURGスレッドへのメッセージ
/*  20200820
#define	WM_URG_END_THREAD				WM_APP+132		/// スレッド終了
#define	WM_URG_SETUP_MOTION				WM_APP+133		/// Docクラスからの動作準備
#define	WM_URG_START_MOTION				WM_APP+134		/// Viewクラスからの動作開始
#define	WM_URG_STOP_MOTION				WM_APP+135		/// Viewクラスからの動作停止
#define	WM_URG_GENERATE_MOTION			WM_APP+136		/// タイマコールバッククラスからの動作生成
*/

/// ドキュメント・ビュークラスからMotionPlanスレッドへのメッセージ
#define	WM_PLAN_URGTHREAD_SET			WM_APP+137		/// URGTHREADのアドレスセット

/// ドキュメント・ビュークラスからDataSrtingスレッドへのメッセージ
#define	WM_DATASRTING_END_THREAD		WM_APP+138		/// スレッド終了
#define	WM_DATASRTING_SETUP_MOTION		WM_APP+139		/// Docクラスからの動作準備
#define	WM_DATASRTING_START_MOTION		WM_APP+140		/// Viewクラスからの動作開始
#define	WM_DATASRTING_STOP_MOTION		WM_APP+141		/// Viewクラスからの動作停止
#define	WM_DATASRTING_GENERATE_MOTION	WM_APP+142		/// URGスレッドからの動作生成


/*  20200820
/// ドキュメント・ビュークラスからURGスレッドへのメッセージ
#define	WM_URG_DATASRTINGTHREAD_SET		WM_APP+143		/// DataSrtingTHREADのアドレスセット

/// ドキュメント・ビュークラスからURGDelineateスレッドへのメッセージ
#define	WM_URGDELINEATE_END_THREAD		WM_APP+144		/// スレッド終了
#define	WM_URGDELINEATE_SETUP_MOTION	WM_APP+145		/// Docクラスからの動作準備
#define	WM_URGDELINEATE_START_MOTION	WM_APP+146		/// Viewクラスからの動作開始
#define	WM_URGDELINEATE_STOP_MOTION		WM_APP+147		/// Viewクラスからの動作停止
#define	WM_URGDELINEATE_GENERATE_MOTION	WM_APP+148		/// URGスレッドからの動作生成

/// ドキュメント・ビュークラスからURGスレッドへのメッセージ
#define	WM_URG_URGDELINEATETHREAD_SET	WM_APP+149		/// URGDelineateTHREADのアドレスセット

/// ドキュメント・ビュークラスからURG_graphicへのメッセージ
#define	WM_URG_URG_GRAPHIC				WM_APP+150		/// URG_graphicのメッセージ
*/
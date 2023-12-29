/**
 *  ファイル名
 *		SerialPortDialog.h
 *  説明
 *		シリアル通信のログ表示のためのダイアログ
 *  日付
 *		作成日: 2007/06/11(Mon)		更新日: 2007/11/10(Sat)
 */

#pragma once


// CSerialPortDialog ダイアログ

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\resource.h"

#include "..\Comm\SerialPort.h"
#include "..\Comm\SciPacket.h"
//追加ヘッダーファイル*//
#include "..\UdpSocket.h"


#include "..\Data\AsuraData.h"
#include "..\Data\PlanData.h"

#include "..\System\ViewSetting.h"
#include "..\File\FileWriter.h"
#include "..\Utility\RingBuffer.h"


/**
 *	----------------------------------------------------------------------
 *		CSerialPortDialogクラス
 *	----------------------------------------------------------------------
 */
class CSerialPortDialog : public CDialog
{
	DECLARE_DYNAMIC(CSerialPortDialog) //new処理をする関数の宣言
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
	static const int MAX_BUFFER_SIZE = 2048;

private:
/**
 *	親ウィンドウ
 */
	CWnd* pParentWindow;

/**
 *	Viewのスタイル
 */
	ViewStyle viewStyle;

/**
 *	通信ポート
 */
	Comm::SerialPort* pCommPort;
	
	UDP::UdpSocket* pUdpPort ;

/**
 *	メッセージを送るスレッドのID
 */
	DWORD motionPlanThreadID;

/**
 *	通信関係
 */
	/// 通信用のAsuraData
	Data::AsuraData commAsuraData;
	Data::PlanData commPlanData;

	Data::AsuraData *Asuratest;

	/**
	 *	送信バッファ
	 */
	char longPacket[Comm::SCI_PACKET_SIZE_LONG];//unsigned char
	unsigned char shortPacket[Comm::SCI_MAX_JOINT_NUM][Comm::SCI_PACKET_SIZE_SHORT];

	/**
	 *	受信用バッファ
	 */
	RingBuffer rxBuffer;

	/**
	 *	パケットタイプ
	 */
	Comm::SciPacketType sciPacketType;

/**
 *	ログ用のファイルライタオブジェクト
 */
	System::FileWriter* pSendLogWriter;

/**
 *	タイマ
 */
	/// タイマ識別子
	UINT_PTR	timerID;

/**
 *	ログ用文字列
 */
	/// ショートパケット送信用
	CString shortPacketString[Comm::SCI_MAX_JOINT_NUM];
	/// ロングパケット送信用
	CString longPacketString;

	/// 受信用
	CString rxDataStr;

/**
 *	色
 */
	/// エディットテキスト色
	COLORREF editColor;
	/// スタティックテキスト色
	COLORREF staticColor;

	/// エディットボックス背景色
	COLORREF editBkColor;
	/// スタティックテキスト背景色
	COLORREF staticBkColor;

/**
 *	ブラシ
 */
	/// ダイアログ背景
	HBRUSH backColorBrush;
	/// エディット背景
	HBRUSH editBkColorBrush;
	/// テキスト背景
	HBRUSH staticBkColorBrush;

/**
 *	フラグ
 */
	/// ブラシ
	bool isBrushCreated;
	/// データ表示
	bool isSendingDataDisplayed;
	bool isRecievingDataDisplayed;

	/// ログを行うか
	bool isLogging;

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
	CSerialPortDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSerialPortDialog();

// ダイアログ データ
	enum { IDD = IDD_DLG_SERIAL_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *	------------------------------------------------------------
 */
public:
	/// Windows管理
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	/// カスタムメッセージ
	/// ダイアログ表示/非表示切替
	afx_msg LRESULT OnSwitchDisplayMode(WPARAM wParam, LPARAM lParam);

	/// 通信開始
	afx_msg LRESULT OnStartPortListening(WPARAM wParam, LPARAM lParam);
	/// 通信停止
	afx_msg LRESULT OnStopPortListening(WPARAM wParam, LPARAM lParam);

	/// 指令値をロボットへ送信
	afx_msg LRESULT OnSendCommandData(WPARAM wParam, LPARAM lParam);

	/// シリアル通信受信
	afx_msg LRESULT OnReceiveData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHandleReceiveMessage(WPARAM wParam, LPARAM lParam);

	/// ログ開始
	afx_msg LRESULT OnStartLogging(WPARAM wParam, LPARAM lParam);
	/// ログ停止
	afx_msg LRESULT OnStopLogging(WPARAM wParam, LPARAM lParam);

	/// 画面更新
	afx_msg LRESULT OnRedrawDialog(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

protected:
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();

/**
 *	------------------------------------------------------------
 *		自作関数
 *	------------------------------------------------------------
 */
public:
	/**
	 *	説明
	 *		メッセージを送るスレッドIDの設定
	 *	引数
	 *		id: スレッドID
	 */
	void setMotionPlanThreadID(DWORD id);

	/**
	 *	説明
	 *		メッセージを送るスレッドIDの取得
	 */
	DWORD getMotionPlanThreadID(void) const{return motionPlanThreadID;}

	/**
	 *	説明
	 *		ダイアログの背景などを設定
	 *	引数
	 *		style: 背景のスタイル
	 */
	void setDialogStyle(ViewStyle style);

	/**
	 *	説明
	 *		送信パケットタイプの設定
	 *	引数
	 *		type: パケットタイプ
	 */
	void setSciPacketType(Comm::SciPacketType type){sciPacketType = type; return;}

	/**
	 *	説明
	 *		送信パケットタイプの取得
	 */
	Comm::SciPacketType getSciPacketType(void){return sciPacketType;}

/**
 *	ログ表示のための関数
 */
	void displayLogData(UINT itemID, LPCTSTR lpFmt, ...);

/**
 *	説明
 *		シリアル通信用パケットデータの作成
 *	引数
 *		type: パケットタイプ
 *		packet: 送信バッファ
 *		adress: 送信先関節番号（単関節命令の場合のみ）
 *		command: 指令種類
 */
	bool buildPacket(Comm::SciPacketType type, char* packet, int address, int command);//unsigned char* packet
};


/**
 *  ファイル名
 *		SerialPort.h
 *  説明
 *		シリアルポートをチェックしデータを送受信する
 *		ポートに変化が起こったらユーザに通知する
 *		送受信用のスレッドも別途作成する
 *  日付
 *		作成日: 2007/04/01(Sat)		更新日: 2007/11/03(Sat)
 */

#ifndef __SerialPort_h__
#define __SerialPort_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */

#include <afx.h>
#include <windows.h>
#include <afxwin.h>

#include "SciRingBuffer.h"
#include "..\System\WinUserMessage.h"
#include "..\System\CriticalSection.h"

namespace Comm
{

/**
 *	----------------------------------------------------------------------
 *		SerialPortクラス
 *	----------------------------------------------------------------------
 */
class SerialPort
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
protected:
/*
*	ネットワーク設定
*/	//yoshidaさんが加えたとこ
 /*
 *	ポート番号
 */
	u_short local_portNo	;			/// 自分のポート番号
	u_short remort_portNo;				/// 相手のポート番号
 /*
 *	IPアドレス
 */
	char local_IPAdress[13]	;			/// 自分のIPアドレス
	char remort_IPAdress[13];			/// 送信先IPアドレス
/**
 *	winsock関連
 */
	WSAData				wsaData; ///	WSAStartupの状態確認の構造体
	int					WSAHandle;
	SOCKET				sockHandle;
	struct sockaddr_in	addr; /// sendto関数(winsockの送信関数)へ渡すポート,IPアドレスの構造体

/****************ここから下がもとからの記述************************/
/**
 *	COMポート関連
 */
	/// COMポートの親ウィンドウへのハンドル
	HWND				commOwnerWindowHandle;
	/// COMポートハンドル
	HANDLE				commHandle;
	/// 通信イベント
	DWORD				commEvent;
	/// ポート番号
	UINT				commPortNumber;
	/// 通信のタイムアウト設定を扱う構造体
	COMMTIMEOUTS	commTimeoutsStruct;
	/// Device-Control Block構造体（通信条件の設定）
	DCB					commDCBStruct;

/**
 *	構造体
 */
	/// overlapped構造体
	OVERLAPPED		readOverLappedStruct;
	OVERLAPPED		writeOverLappedStruct;

/**
 *	送受信用スレッド関連
 */
	/// スレッドポインタ
	CWinThread* readThreadPointer;
	CWinThread* writeThreadPointer;

	/// スレッド生存判定のフラグ
	BOOL isReadThreadAlive;
	BOOL isWriteThreadAlive;

	/// 同期オブジェクト
	System::CriticalSection	criticalSection;

	/// スレッド終了ハンドル
	HANDLE		readThreadTerminateHandle;
	HANDLE		writeThreadTerminateHandle;

/**
 *	イベント配列
 */	
	/// 各イベント毎に用意したイベントハンドル
	/// WaitForMultipleObjects関数のために準備
	HANDLE		readEventHandles[2];
	HANDLE		writeEventHandles[2];

/**
 *	バッファ
 */
	/// バッファサイズ
	static const int MAX_BUFFER_SIZE = 1024;

	/**
	 *	一時バッファ
	 */
	/// 受信バッファ
	unsigned char* readBuffer;   //訂正前：unsigned char* readBuffer
	/// 送信バッファ
	char* writeBuffer;
	//unsigned char* writeBuffer;

	/// 受信記録バッファ
	SciRingBuffer receiveBuffer;

	/// 送受信サイズ
	DWORD bytesToRead;
	DWORD bytesToWrite;

	/// 受信バッファサイズ
	DWORD		readBufferSize;
	/// 送信バッファサイズ
	DWORD		writeBufferSize;

/**
 *		フラグ
 */
	bool isListeningStopped;
	bool isCommOpen;

private:

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
	explicit SerialPort();
	/// デストラクタ
	virtual ~SerialPort();

/**
 *	説明
 *		シリアルポートを開く
 *	引数
 *		hOwnerWnd: COMポートの親ウィンドウ
 *		portNum: COMポート番号
 *		baudRate: ボーレート
 *		parity: パリティ
 *		dataBits: データビット
 *		stopBits: ストップビット
 *		commEvents: ユーザが設定する通信イベント
 *		readBufferSize_: 受信バッファのサイズ
 *		writeBufferSize_: 送信バッファのサイズ
 */
	BOOL openSerialPort(	HWND hOwnerWnd,								/// COMポートの親ウィンドウ
							UINT portNum = 1,							/// COMポート番号
							UINT baudRate = 115200,						/// ボーレート
							char parity = 'N',							/// パリティ
							UINT dataBits = 8,							/// データビット 
							UINT stopBits = 1,							/// ストップビット
							DWORD commEvents = EV_RXCHAR | EV_CTS,		/// ユーザが設定する通信イベント
							UINT readBufferSize_ = MAX_BUFFER_SIZE,		/// 受信バッファのサイズ
							UINT writeBufferSize_ = MAX_BUFFER_SIZE		/// 送信バッファのサイズ
						);

/**
 *	説明
 *		シリアルポートを閉じる
 */
	void closeSerialPort(void);

/**
 *	説明
 *		DCBの設定（データ制御ブロック）
 *	引数
 *		deviceCtrlStr: デバイス制御情報を表す文字列へのポインタ
 */
	BOOL initializeCommDCB(LPTSTR deviceCtrlStr);

/**
 *	説明
 *		COMMTIMEOUTSの設定
 */
	BOOL initializeCommTimeouts(void);

/**
 *	----------------------------------------	
 *	アクセス関数
 *	----------------------------------------
 */
	/// COMポートハンドル
	HANDLE getCommPortHandle(void) const{return commHandle;}
	/// 通信イベント
	DWORD getCommPortEvent(void) const{return commEvent;}
	/// ポート番号
	UINT getCommPortNumber(void) const{return commPortNumber;}
	/// 通信のタイムアウト設定を扱う構造体
	COMMTIMEOUTS getCommTimeoutsStruct(void) const{return commTimeoutsStruct;}
	/// Device-Control Block構造体（通信条件の設定）
	DCB getCommDCBStruct(void) const{return commDCBStruct;}

	/// 送信/受信バッファサイズ
	DWORD getReadBufferSize(void) const{return readBufferSize;}
	DWORD getWriteBufferSize(void) const{return writeBufferSize;}

	/// リングバッファ本体
	SciRingBuffer& getReceiveBuffer(void) {return receiveBuffer;}

/**
 *	COMポートの監視関数（スレッド開始・停止・終了）
 */
	/// 監視開始
	BOOL startListening(void); //ここからスレッド開始してる
	/// 監視再開
	BOOL restartListening(void);
	/// 監視停止
	BOOL stopListening(void);

/**
 *	説明
 *		COMポートへデータを送信
 *	引数
 *		sendData: 書き込みデータへのポインタ
 *		bytesToSend: 書き込みサイズ
 */
	void	sendData( char* sendData, DWORD bytesToSend);//unsigned

protected:
/**	
 *	説明
 *		通信エラーの取得
 *	引数
 *		書き込み先へのポインタ
 */
	void outputProcessErrorMessage(char* errorText);

/**
 *	----------------------------------------
 *	送受信に用いるスレッド関数
 *	----------------------------------------
 */
	/// 受信スレッド本体
	static UINT readEventListenThread(LPVOID pParam);//
	/// 送信スレッド本体
	static UINT writeEventListenThread(LPVOID pParam);//

	/// データ受信関数
	static void	readData(SerialPort* port, COMSTAT& comStat);//
	/// データ送信関数
	static void	writeData(SerialPort* port, COMSTAT& comStat);//

private:
	/// コピーコンストラクタ無効
	SerialPort(const SerialPort& serial);

	/// 代入演算子無効
	SerialPort& operator=(const SerialPort& serial)=delete;
};	/// end of class SerialPort

}	/// end of namespace Comm

#endif	/// __SerialPort_h__
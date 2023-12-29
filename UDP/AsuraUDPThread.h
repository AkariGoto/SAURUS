#pragma once

#include "..\resource.h"

#include "..\Data\AsuraData.h"
#include "..\Data\PlanData.h"

#include "..\System\ViewSetting.h"
#include "..\File\FileWriter.h"
#include "..\Utility\RingBuffer.h"

//2020/08/21追加
#include "Udp.h"
#include "UDPPacket.h"

// WinSock 20210622追加
#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//SerialPort.hの
#include <afx.h>
#include <windows.h>
#include <afxwin.h>

#include "UdpRingBuffer.h"
#include "..\System\WinUserMessage.h"
#include "..\System\CriticalSection.h"
#include "..\System\Timer\MultiMediaTimer.h"
#include "..\System\Thread\TimedMotionProcedure.h"
#include "..\System\Thread\MotionPlanThread.h"

//クラスの前方宣言
class UDPSocket;

using namespace UDP;



// AsuraUDPTherad

class AsuraUDPThread : public CWinThread
{

	DECLARE_DYNCREATE(AsuraUDPThread)
	
/**
 *		タイマオブジェクト
 */
	System::TimedMotionProcedure*	 pTimedUDPProcedure;
	System::MultiMediaTimer*		 pMultiMediaTimer;
	static Plan::TimeManager		 timeManager;

/**
 *		フラグ
 */
 /// スレッドが有効かどうか
	BOOL isAlive;

	/// 通信が有効かどうか
	bool isCommAlive;

	//bool isListeningStopped;
	


protected:
	AsuraUDPThread();           // 動的生成で使用される protected コンストラクター
	virtual ~AsuraUDPThread();

public:
	virtual BOOL InitInstance();	//初期化
	virtual int ExitInstance();


	/**
	 *	説明
	 *		開始処理
	 */
	void initializeAsuraUDPThread(void);

	/**
	 *	説明
	 *		終了処理
	 */
	void finalizeAsuraUDPThread(void);

	/*
	*
	* パケットデータ作成
	*
	*/

	char buildPacket(UDP::SciPacketType type, int address, int command);

	/**
	 *	説明
	 *		スレッドが有効かどうか
	 */
	BOOL isRunning(void) const { return isAlive; }

	DECLARE_MESSAGE_MAP()

protected:
	

	/// スレッド終了
	afx_msg void OnEndThread(WPARAM wParam, LPARAM lParam);
	/// 通信開始
	afx_msg void OnStartPortListening(WPARAM wParam, LPARAM lParam);
	/// 通信停止
	afx_msg void OnStopPortListening(WPARAM wParam, LPARAM lParam);

	/// 指令値をロボットへ送信
	afx_msg void OnSendCommandData(WPARAM wParam, LPARAM lParam);

	/*未実装20210622
	/// ログ開始
	afx_msg void OnStartLogging(WPARAM wParam, LPARAM lParam);
	/// ログ停止
	afx_msg void OnStopLogging(WPARAM wParam, LPARAM lParam);
	*/





/*
 *	ポート番号
 */
	u_short s_portNo=10004;			/// 自分のポート番号
	u_short c_portNo=10003;				/// 相手のポート番号
 /*
 *	IPアドレス
 */
	//char server_IPAdress[14]="192.168.0.154";			/// 自分のIPアドレス
	const char server_IPAdress[14] = "192.168.0.157";			/// 自分のIPアドレス
	const char client_IPAdress[14] = "192.168.0.169";			/// 送信先IPアドレス


	SOCKET sock;
	struct sockaddr_in server_addr, client_addr;
	int port;
	const char* addr;

/*********************  構造体  ***************************/

	/// overlapped構造体
	//OVERLAPPED		readOverLappedStruct;
	//OVERLAPPED		writeOverLappedStruct;


/**
 *	イベント配列
 */
 /// 各イベント毎に用意したイベントハンドル
 /// WaitForMultipleObjects関数のために準備
	//HANDLE		readEventHandles[2];
	//HANDLE		writeEventHandles[2];

	/// スレッド終了ハンドル
	//HANDLE		readThreadTerminateHandle;
//HANDLE		writeThreadTerminateHandle;


/**
	 *	バッファ
	 */
	 /// バッファサイズ
	static const int MAX_BUFFER_SIZE = 2048;

	/**
	 *	一時バッファ
	 */
	 /// 受信バッファ
	//unsigned char* readBuffer;
	/// 送信バッファ
	unsigned char* writeBuffer{};

	/// 受信記録バッファ
	//unsigned char* SciRingBufferreceiveBuffer;

	/// 送受信サイズ
	//DWORD bytesToRead{};
	DWORD bytesToWrite{};

	/// 受信バッファサイズ
	//DWORD		readBufferSize;
	/// 送信バッファサイズ
	DWORD writeBufferSize{};
	UINT writeBufferSize_{};
	int bytesToSend{};




private:

	Data::AsuraData viewAsuraXData;
	Data::PlanData viewPlanData;

	


	/**
	 *	パケットタイプ
	 */
	UDP::SciPacketType sciPacketType;

	/**
	 *	送信バッファ
	 */
	//char longPacket[UDP::SCI_PACKET_SIZE_LONG];//unsigned char
	char longPacket[32];
	unsigned char shortPacket[UDP::SCI_MAX_JOINT_NUM][UDP::SCI_PACKET_SIZE_SHORT];

	char onelegPacket[UDP::SCI_PACKET_SIZE_ONELEG];//追加


	//bool buildPacket(UDP::SciPacketType type, char* packet, int address, int command);

	//UDPSocket* pUdpPort;

	//void writeData(AsuraUDPThread* port, COMSTAT& comStat);



	/// 同期オブジェクト
	//System::CriticalSection	criticalSection;

	//UDPSocket* pUDPSocket;

};



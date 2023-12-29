/**
 *  ファイル名
 *		Socket.h
 *  説明
 *		UDP通信に関するクラス
 *  日付
 *		作成日: 2019/11/18(Sat)		更新日: 2007/11/03(Sat)
 */


#include <stdio.h>
#include <stdlib.h>
//Win32API
//#include<windows.h>
#include <afx.h>
#include <afxwin.h>
#include "UdpRingBuffer.h"
#include "..\System\WinUserMessage.h"
#include "..\System\CriticalSection.h"

//Winsock関連
#include<winSock2.h>

namespace UDP
{
/**
 *	----------------------------------------------------------------------
 *		UdpSocketクラス
 *	----------------------------------------------------------------------
 */
class UdpSocket {
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */

private:
    //SOCKET sock ; // ソケット
	UdpSocket* udpsocket;
    //struct sockaddr_in addr ;
	// sockaddr_in server_addr, client_addr;   // 送受信アドレス

protected :

	/******************  ネットワーク設定(yoshidaさんが加えたとこ)*****************/
 
               	       /*ポート番号*/
	u_short local_portNo	;			 /// 自分のポート番号
	u_short remort_portNo;				 /// 相手のポート番号

	                   /*IPアドレス*/
	char local_IPAdress[13]	;			 /// 自分のIPアドレス
	char remort_IPAdress[13];			 /// 送信先IPアドレス

	                  /*winsock関連*/
	WSAData				wsaData;         ///WSAStartupの状態確認の構造体
	int					WSAHandle;
	SOCKET				sockHandle;      ///ソケット作成
	struct sockaddr_in	addr;           ///IPアドレスの構造体


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

/*********************  構造体  ***************************/

	/// overlapped構造体
	OVERLAPPED		readOverLappedStruct;
	OVERLAPPED		writeOverLappedStruct;

/*****************  送受信用スレッド関連  ******************/

	/// スレッドポインタ
	CWinThread* UDPreadThreadPointer;
	CWinThread* UDPwriteThreadPointer;

	/// スレッド生存判定のフラグ
	BOOL UDPisReadThreadAlive;
	BOOL UDPisWriteThreadAlive;

	/// 同期オブジェクト
	System::CriticalSection	criticalSection;

	/// スレッド終了ハンドル
	HANDLE		UDPreadThreadTerminateHandle;
	HANDLE		UDPwriteThreadTerminateHandle;

/****************  イベント配列  *************************/	

	/// 各イベント毎に用意したイベントハンドル
	/// WaitForMultipleObjects関数のために準備
	HANDLE		readEventHandles[2];
	HANDLE	    writeEventHandles[2];




/*********************  バッファ  ************************/
	/// バッファサイズ
	static const int MAX_BUFFER_SIZE = 1024;


/*******************  一時バッファ **************************/

	/// 受信バッファ
	unsigned char* readBuffer; //unsigned char* UDPreadBuffer;  
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
	bool UDPisListeningStopped;
	bool UDPisCommOpen;




public:
/**************************************************************************/
                  //コンストラクタ＆デストラクタ
/**************************************************************************/
	explicit UdpSocket();                  
	virtual ~UdpSocket();                  

/************************  UDPを開く  ******************************/
	BOOL  openUDP(	HWND hOwnerWnd,								/// COMポートの親ウィンドウ
							UINT portNum = 1,							/// COMポート番号
							UINT baudRate = 115200,						/// ボーレート
							char parity = 'N',							/// パリティ
							UINT dataBits = 8,							/// データビット 
							UINT stopBits = 1,							/// ストップビット
							DWORD commEvents = EV_RXCHAR | EV_CTS,		/// ユーザが設定する通信イベント
							UINT readBufferSize_ = MAX_BUFFER_SIZE,		/// 受信バッファのサイズ
							UINT writeBufferSize_ = MAX_BUFFER_SIZE		/// 送信バッファのサイズ
						);   // 初期化           （失敗:0 成功:1）


/**************************  UDPを閉じる  *******************************/
    void closeUDP(void);
	    

	//送信データをここに入れる
	char buf[2048] ;  //とりま保留


/**************************************************************************/
                  //アクセス関数
/**************************************************************************/
	
	/// 送信/受信バッファサイズ
	DWORD UDPgetReadBufferSize(void) const{return readBufferSize;}
	DWORD UDPgetWriteBufferSize(void) const{return writeBufferSize;}



	/**
 *	説明
 *		COMポートへデータを送信
 *	引数
 *		sendData: 書き込みデータへのポインタ
 *		bytesToSend: 書き込みサイズ
 */
	void	sendData( char* sendData, DWORD bytesToSend);//unsigned

    void receiveData( char* receiveData, DWORD bytesToReceive);



/******************  COMポートの監視関数（スレッド開始・停止・終了）***************************/

	/// 監視開始
	BOOL UDPstartListening(void); //ここからスレッド開始してる
	/// 監視再開
	BOOL UDPrestartListening(void);
	/// 監視停止
	BOOL UDPstopListening(void);



protected:
	/**	
 *	説明
 *		通信エラーの取得
 *	引数
 *		書き込み先へのポインタ
 */
	void outputProcessErrorMessage(char* errorText);



	

/*	----------------------------------------
 	送受信に用いるスレッド関数
 *	----------------------------------------*/
	/// 受信スレッド本体
	static UINT UDPreadEventListenThread(LPVOID pParam);//
	/// 送信スレッド本体
	static UINT UDPwriteEventListenThread(LPVOID pParam);//

	/// データ受信関数
	static void	UDPreadData(UdpSocket* udpsocket, COMSTAT& comStat);   //引数はSocketクラスのオブジェクト, この中でUDP受信操作を行う
	/// データ送信関数
	static void	UDPwriteData(UdpSocket* udpsocket, COMSTAT& comStat);  //引数はSocketクラスのオブジェクト, この中でUDP送信操作を行う


private:
	/// コピーコンストラクタ無効
	UdpSocket(const UdpSocket& serial);

	/// 代入演算子無効
	UdpSocket& operator=(const UdpSocket& serial);

}; //end of class Socket

}//end of namespace UDP


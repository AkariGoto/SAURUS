
/**
 *  ファイル名
 *		UdpSocket.cpp
 *  説明
 *		UDP通信に関するクラス
 *  日付
 *		作成日: 2019/11/19	更新日: 2019/11/19
 */

#include"UdpSocket.h"

//
#include <cassert>
#include <process.h>
#include "Comm\SerialPort.h"
#include "..\System\DebugOutput.h"

//追加
#include <winsock2.h>



namespace UDP
{

//コンストラクタ
UdpSocket::UdpSocket()
{
	/**
 *	ポート番号
 */
	/// 自分のポート番号
	local_portNo		= 10004;
	/// 相手のポート番号
	remort_portNo		= 10003;
/**
 *	IPアドレス
 */
	/// 自分のIPアドレス
	sprintf(local_IPAdress,"192.168.0.18");
	/// 相手のIPアドレス
	sprintf(remort_IPAdress,"192.168.0.169");	//送信先IPアドレス


/**
 *	WASData,Soketハンドル初期化
 *
 *	WASStartup	失敗:  
 *				成功:  0	
 *
 *	soket		失敗: SOCKET_ERROR(-1)
 *				成功: 負でない(含0)ソケット記述子
 */
	WSAHandle	= 1;
	sockHandle	= INVALID_SOCKET ;

/**
 *	オーナーウィンドウ初期化
 */
	commOwnerWindowHandle = NULL;

/**
 *	COMポートハンドルの初期化
 */
	commHandle = NULL;

/** 
 *	OVERLAPPED構造体のメンバ変数を0にする
 */
	readOverLappedStruct.Offset = 0;
	writeOverLappedStruct.Offset = 0;
	readOverLappedStruct.OffsetHigh = 0;
	writeOverLappedStruct.OffsetHigh = 0;

	/// イベントハンドルの初期化
	readOverLappedStruct.hEvent		= NULL;
	writeOverLappedStruct.hEvent	= NULL;

	/// バッファの初期化
	/// 受信用
	readBuffer = NULL;
	/// 送信用
	writeBuffer = NULL;

	/// 送受信サイズ
	bytesToRead = 0;
	bytesToWrite = 0;

/**
 *	スレッド関連の初期化
 */
	/// ハンドル
	UDPreadThreadTerminateHandle = NULL;
	UDPwriteThreadTerminateHandle = NULL;

	/// スレッドフラグ
	UDPisReadThreadAlive = FALSE;
	UDPisWriteThreadAlive = FALSE;

/**
 *	フラグの初期化
 */
	UDPisListeningStopped = true;
	UDPisCommOpen = false;








}

//デストラクタ
UdpSocket::~UdpSocket(){
	//デストラクタ＝＞Socketクラスを破棄するときに呼ぶ
}

//-----------------------------------------------------------------------------------------
//初期化(ソケット作ったりいろいろ)
//-----------------------------------------------------------------------------------------
int UdpSocket::openUDP(	HWND hOwnerWnd,				/// 受信ポートの親ウィンドウハンドル
						UINT portNo,				/// ポート番号
						UINT baudRate,				/// ボーレート
						char parity,				/// パリティ
						UINT dataBits,				/// データビット
						UINT stopBits,				/// ストップビット
						DWORD commEvent_,			/// イベント
						UINT readBufferSize_,		/// 受信バッファサイズ
						UINT writeBufferSize_		/// 送信バッファサイズ
									)
{
	//ソケット通信winsockの立ち上げ
	///winsock初期化
	WSAHandle = WSAStartup(MAKEWORD(2,0), &wsaData);///Ver.2.0

	///エラー処理　(初期化が失敗するとwinsockの機能が使えないので要確認)
	///WSAStartup :成功した場合,戻り値0	
	assert(WSAHandle == 0);
	if (WSAHandle == 0){
		///WSASETUP成功
		//DEBUG_TRACE(TEXT("WSASETUP\n"));
	}
	else if (WSAHandle != 0) {
		switch (WSAHandle) {
			///ネットワークサブシステムがネットワークへの接続を準備できていない
			case WSASYSNOTREADY:
				DEBUG_TRACE(TEXT("WSASYSNOTREADY\n"));
			break;
			///要求されたwinsockのバージョンがサポートされていない
			case WSAVERNOTSUPPORTED:
				DEBUG_TRACE(TEXT("WSAVERNOTSUPPORTED\n"));
			break;
			///ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理している
			case WSAEINPROGRESS:
				DEBUG_TRACE(TEXT("WSAEINPROGRESS\n"));
			break;
			///winsockが処理できる最大プロセス数に達した
			case WSAEPROCLIM:
				DEBUG_TRACE(TEXT("WSAEPROCLIM\n"));
			break;
			///	第二引数であるlpWSAData は有効なポインタではない
			case WSAEFAULT:
				DEBUG_TRACE(TEXT("WSAEFAULT\n"));
			break;
			assert(WSAHandle =! 0);
		}
		return FALSE;
	}

///ソケット作成
	sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
	//AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？
	
	
	///エラー処理
	///失敗したらsocketは-1を返す
	if(sockHandle =! -1){
		#define SOCK_HANDLE_VALUE ((HANDLE)(2))
		commHandle = SOCK_HANDLE_VALUE;///<---ハンドルの値がわからない
	}                                   //ワンちゃんsockHandleかも？？
	///エラー
	else if (sockHandle == -1){//INVALID_SOCKET
		///エラー値を表示 (VC++エラールックアップにエラー値を入力して確認してください)
		DEBUG_TRACE(TEXT("socket error : %d\n", WSAGetLastError()));
		assert(sockHandle=! -1);
        return FALSE;
	}











	/**
 *	バッファ関係
 */
	/// バッファ領域の確保
	if (readBufferSize != NULL) delete [] readBuffer;
	if (readBufferSize_ > MAX_BUFFER_SIZE) readBufferSize_ = MAX_BUFFER_SIZE;
	readBuffer = new unsigned char[readBufferSize_];

	if (writeBuffer != NULL) delete [] writeBuffer;
	if (writeBufferSize_ > MAX_BUFFER_SIZE) writeBufferSize_ = MAX_BUFFER_SIZE;
	writeBuffer = new char[writeBufferSize_];
	//writeBuffer = new unsigned char[writeBufferSize_];

	/// リングバッファの設定
	receiveBuffer.initialize(11);

	/// バッファサイズの設定
	readBufferSize = readBufferSize_;
	writeBufferSize = writeBufferSize_;

	/// バッファの初期化
	ZeroMemory(readBuffer, readBufferSize);
	ZeroMemory(writeBuffer, writeBufferSize);









}






/***********************************************************************
                 //  スレッド開始・停止・終了
*************************************************************************/

//モニタリング開始
BOOL UdpSocket::UDPstartListening(void)
{
///////////////////  UDP受信スレッド開始  //////////////////////////
	UDPreadThreadPointer = AfxBeginThread(	UDPreadEventListenThread,    //制御関数
										(LPVOID)this,             //制御関数に渡すパラメータ
										THREAD_PRIORITY_NORMAL,   //スレッドの優先順位
										0,                        //作成直後にスレッドを開始
										CREATE_SUSPENDED
										);
	UDPreadThreadPointer->m_bAutoDelete=FALSE;
	UDPreadThreadPointer->ResumeThread();


//////////////////  UDP送信スレッド開始  ///////////////////////////
	UDPwriteThreadPointer = AfxBeginThread(	UDPwriteEventListenThread,
											(LPVOID)this,
											THREAD_PRIORITY_NORMAL,
											0,
											CREATE_SUSPENDED
										);

	UDPwriteThreadPointer->m_bAutoDelete=FALSE;
	UDPwriteThreadPointer->ResumeThread();

	DEBUG_TRACE( TEXT("Thread started\n") );

	/// フラグの反転
	UDPisListeningStopped = false;

	return TRUE;	
}

/// モニタリング再開
BOOL UdpSocket::UDPrestartListening(void)
{
	//なんかかく
	if (!UDPisListeningStopped)
		return FALSE;

	/// スレッド再開
	UDPreadThreadPointer->ResumeThread();
	UDPwriteThreadPointer->ResumeThread();

	DEBUG_TRACE( TEXT("Thread resumed\n") );

	/// フラグの決定
	UDPisListeningStopped = false;

	return TRUE;	
}

/// モニタリング停止
BOOL UdpSocket::UDPstopListening(void)
{
	//なんかかく
	if (UDPisListeningStopped)
		return FALSE;

	/// スレッド停止
	UDPreadThreadPointer->SuspendThread();
	UDPwriteThreadPointer->SuspendThread();

	DEBUG_TRACE( TEXT("Thread suspended\n") );

	/// フラグの決定
	UDPisListeningStopped = true;

	return TRUE;
}






/**
 *	説明
 *		COMポートへデータを送信
 *	引数
 *		sendData: 書き込みデータへのポインタ
 *		bytesToSend: 書き込みサイズ
 */
void UdpSocket::sendData( char* sendData, DWORD bytesToSend)//unsigned char
{
	//sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
	/// COMポートが開かれているかのチェック
	//assert(commHandle != 0);
	
	/// 初期化
	memset(writeBuffer, 0, sizeof(writeBuffer));

	/// データコピー
	int i;
	for (i=0; i<(int)bytesToSend; i++)
	{
		///Winsock2のsendtoはchar型配列なので注意
		writeBuffer[i] = sendData[i];//(char)
	}

	/// 送信バイト数代入
	bytesToWrite = bytesToSend;

	/// 書き込みスレッドのための送信イベントセット
	SetEvent(writeOverLappedStruct.hEvent);

	return;
}


void UdpSocket::receiveData(char* receiveData, DWORD bytesToReceive)
{

}




//////////////////////  通信エラーの取得  ////////////////////////////////////////////
void UdpSocket::outputProcessErrorMessage(char* errorText)
{

}



///////////////////  UDP受信に用いるスレッド関数  ////////////////////////////////////
UINT UdpSocket::UDPreadEventListenThread(LPVOID pUserData)
{
	/// 引数のVOIDポインタをSerialPortクラスのポインタへキャスト
	UdpSocket* udpsocket = (UdpSocket*) pUserData;

	/// スレッドが走っていることを通知するため，状態変数をTRUEにする
	udpsocket->UDPisReadThreadAlive = TRUE;	
		
	/// ローカル変数の宣言
	DWORD		eventMask = 0;
	DWORD		readThreadEvents = 0;
	DWORD		readEventMask = 0;
	DWORD		commErrors = 0;
	BOOL		isEventOccurred = TRUE;
	COMSTAT		comStat;

	/// ポートが開かれているかチェックして，起動時に通信バッファをクリア
	if (udpsocket->commHandle)
		PurgeComm(udpsocket->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	//
	 //	無限ループ
	 //	このループはスレッドが走っている限り継続
	 //
	while (1) 
	{ 
		//
		 //	WaitCommEvent関数の呼び出し
		 //		ただし，通信ポートを非同期で開いたためすぐに終了する
		 //
		 //		この呼び出しにより，overLappedStructのメンバであるhEvent（readEventHandles[1]）を
		 //		読み込み可能なbyteデータがなければリセット状態に，読み込み可能なデータがあればセット状態にする
		 //
		 //		このイベントハンドルがリセット状態であると，通信ポートにデータが来た時にセット状態になる
		 //		なお，この時はOverLapped構造体に手動リセットイベントのハンドルを指定しておく必要がある
		 //
		isEventOccurred = WaitCommEvent(udpsocket->commHandle, &eventMask, &udpsocket->readOverLappedStruct);

		/// 以下で受信があったかどうかの確認
		if ( !isEventOccurred )	/// 受信イベントがなかったら
		{
			 //
			 //	WaitCommEvent関数がFALSEを返した場合，プロセスによりエラーが決定される
			 //	以下でのそのエラーの特定を行う
			 //
			switch (commErrors = GetLastError()) 
			{ 
				case ERROR_IO_PENDING: 	
				{ 
					/// このエラーはポートにデータがない場合の通常の戻り値
					/// 特に何もせずに続ける
					break;
				}
				case 87:
				{
					/// Windows NT系の場合，何かの理由で返却されることがある
					/// しかし特に問題ないので継続可
					break;
				}
				default:
				{
					/// 他の全てのエラー：深刻なエラーが起きたことを示す
					/// したがってこのエラー処理をする
					udpsocket->outputProcessErrorMessage( "WaitCommEvent()" );
					break;
				}
			}	/// switch ( commErrors = GetLastError() ) 
		}	/// end if isEventOccurred

		/**
		 *	受信待機関数
		 *	この関数により1つのイベントが発生するまでスレッドが通常ブロックされる
		 */
		/**
		 *	DWORD WaitForMultipleObjects(
		 *				DWORD  nCount,				/// オブジェクトの数
		 *				CONST HANDLE *pHandles,		/// ハンドルの配列
		 *				BOOL   bWaitAll,			/// 待機イベントのシグナル状態を1つかすべて待つかのフラグ
		 *				DWORD  dwMilliseconds		/// タイムアウト時間
		 *				);
		 */
		//readThreadEvents = WaitForMultipleObjects(2, port->readEventHandles, FALSE, INFINITE);

		/**
		 *	WaitCommEvent関数によりイベントを検知した場合
		 *	実際にバッファにデータがあるかチェックする
		 */
		/**
		 *	ClearCommError()
		 *		エラーをクリアすることが目的であるので，本来の目的とは異なる．
		 *		しかし，この関数により返される構造体COMSTATに，
		 *		受信バッファ中のデータ数をあらわすメンバcbInQue があるので, 
		 *		この関数を呼び出し，受信データ数を取得する
		 */
		isEventOccurred = ClearCommError(udpsocket->commHandle, &commErrors, &comStat);

		switch (readThreadEvents)
		{
			case WAIT_OBJECT_0:	/// スレッドシャットダウンイベント
			{
				/// このイベントは最優先であり最初に提供される
			 	udpsocket->UDPisReadThreadAlive = FALSE;
				
				/// スレッド破棄
				AfxEndThread(100);
				
				break;
			}
		
			case WAIT_OBJECT_0 + 1:	/// 読み込みイベント
			{
				/// 受信バッファを確認して，データがなければ無限ループの最初に戻る
				if ( comStat.cbInQue == 0 )
					continue;

				/// COMポートのイベントを取得する
				GetCommMask(udpsocket->commHandle, &readEventMask);

				//
				 //	受信以外のイベントが起こっていたらまず処理する
				 //
				/// CTS（送信可）信号の状態が変化
				if ( readEventMask & EV_CTS && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_CTS_DETECTED, 
										(WPARAM) 0, 
										(LPARAM) port->commPortNumber);

				/// 入力上でブレークを検出
				if ( readEventMask & EV_BREAK && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_BREAK_DETECTED, 
										(WPARAM) 2, 
										(LPARAM) port->commPortNumber);

				/// 回線状態エラーが発生（CE_FRAME、CE_OVERRUN、CE_RXPARITY）
				if ( readEventMask & EV_ERR && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_ERR_DETECTED, 
										(WPARAM) 3, 
										(LPARAM) port->commPortNumber);

				/// 呼び出し信号を検出
				if ( readEventMask & EV_RING && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_RING_DETECTED, 
										(WPARAM) 4, 
										(LPARAM) port->commPortNumber);

				/// COMポートから文字列データを受信
				if ( readEventMask & EV_RXCHAR )
				{
					UDPreadData(udpsocket, comStat);//ここをいじって出力
				}

				break;
			}
			default:
				break;
		} /// end of switch

		/// 受信バッファのデータ読み込みが終わった時点でイベントリセット
		ResetEvent(port->readOverLappedStruct.hEvent);

	} /// 無限ループ
	
	return 0;
}

///////////  UDP受信スレッドで用いる受信バッファからの読み出し関数  ///////////////////
void UdpSocket::UDPreadData(UdpSocket* udpsocket, COMSTAT& comsStat)
{
	BOOL	UDPisReadingToTry		= TRUE;		/// 読み込みを行うかどうか
	BOOL	UDPisReadingCompleted	= TRUE;		/// 読み込みが完了したかどうか
	BOOL	UDPisReadingSucceeded	= TRUE;		/// 読み込みが成功したかどうか
	DWORD	UDPerrorCode				= 0;		/// エラー
	DWORD	UDPbytesRead				= 0;		/// 読み込んだバイト数
	int i=0;										/// カウンタ		

	///ソケット作成
	//udpsocket->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);

	udpsocket->addr.sin_family	= AF_INET;									///アドレスファミリ
	udpsocket->addr.sin_port	= htons(udpsocket->remort_portNo);//htons(10003);//					///相手のポート番号
	udpsocket->addr.sin_addr.S_un.S_addr = inet_addr(udpsocket->remort_IPAdress);//inet_addr("192.168.0.169");//	///相手のIPアドレス


	//ポート
	//bindしてない場合はrecvfromを使う
	bind(udpsocket->sockHandle, (struct sockaddr *)&(udpsocket->addr), sizeof(udpsocket->addr));

	///ソケットのデータを受信
	UDPisReadingSucceeded =recvfrom( udpsocket->sockHandle, 
									udpsocket->readBuffer,//Rxbuffer,
									udpsocket->bytesToRead,//length2,
									0, 
									(struct sockaddr *)&udpsocket->addr, 
									sizeof(udpsocket->addr));



}





///////////////////  UDP送信に用いるスレッド関数  /////////////////////////////////////
UINT UdpSocket::UDPwriteEventListenThread(LPVOID pParam)
{
	/// 引数のVOIDポインタをSerialPortクラスのポインタへキャスト
	UdpSocket* udpsocket = (UdpSocket*) pParam ;

	/// スレッドが走っていることを通知するため，状態変数をTRUEにする
	udpsocket->UDPisWriteThreadAlive = TRUE ;

	/// ローカル変数の宣言
//	DWORD		eventMask = 0;
//	DWORD		writeEventMask = 0;
	DWORD		writeThreadEvents = 0;
//	DWORD		writeEvents = 0;
//	DWORD		commErrors = 0;
//	BOOL		isEventOccurred = TRUE;
	COMSTAT		comStat;

	/// ポートが開かれているかチェックして，起動時に通信バッファをクリア
	if (udpsocket->commHandle)
		PurgeComm(udpsocket->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/**
	 *	 無限ループ
	 *	このループはスレッドが走っている限り継続
	 */
	while (1) 
	{
		/// WaitForMultipleObjects()でイベント待ち
		writeThreadEvents = WaitForMultipleObjects(2, udpsocket->writeEventHandles, FALSE, INFINITE);

		switch (writeThreadEvents)
		{
			case WAIT_OBJECT_0:	/// スレッドシャットダウンイベント
			{
				/// このイベントは最優先であり最初に提供される
			 	udpsocket->UDPisWriteThreadAlive = FALSE;

				/// スレッド破棄
				AfxEndThread(200);
				break;
			}
			case WAIT_OBJECT_0 + 1:	/// 書き込みイベント
			{
				/// ポートへデータ書き込み
				UDPwriteData(udpsocket, comStat);

				break;
			}
			default:
				break;
		}	/// end of switch (writeThreadEvents)

	}	/// 無限ループ

	return 0;
}


///////////  UDP送信スレッドで用いる送信バッファからの書き込み関数  ///////////////////
void UdpSocket::UDPwriteData(UdpSocket* udpsocket, COMSTAT& comStat)
{
	/// ローカル変数の宣言
	BOOL	UDPisWritingToTry		= TRUE;		/// 書き込みを行うかどうか
	BOOL	UDPisWritingCompleted	= TRUE;		/// 書き込みが完了したかどうか
	BOOL	UDPisWritingSucceeded	= TRUE;		/// 書き込みが成功したかどうか
	DWORD	UDPerrorCode			= 0;		/// エラーコード
	DWORD	UDPbytesWritten		    = 0;
	char	UDPErrMassage[32]		= {0};		///エラーコード表示用

	///ソケット作成
	udpsocket->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);

	assert(udpsocket->WSAHandle =! 0);

	/// 待機イベントをリセット
	ResetEvent(udpsocket->writeOverLappedStruct.hEvent);

	udpsocket->criticalSection.lock();

	/**
	 *	ClearCommError()によりCOMSTAT構造体を更新し他のエラーを消去
	 */
	UDPisWritingSucceeded = ClearCommError(udpsocket->commHandle, &UDPerrorCode, &comStat);

    udpsocket->addr.sin_family	= AF_INET;									///アドレスファミリ
	udpsocket->addr.sin_port		= htons(udpsocket->remort_portNo);//htons(10003);//					///相手のポート番号
	udpsocket->addr.sin_addr.S_un.S_addr = inet_addr(udpsocket->remort_IPAdress);//inet_addr("192.168.0.169");//	///相手のIPアドレス

	///ソケットのデータを送信
	UDPisWritingSucceeded =sendto( udpsocket->sockHandle, 
									udpsocket->writeBuffer,//Txbuffer,
									udpsocket->bytesToWrite,//length2,
									0, 
									(struct sockaddr *)&udpsocket->addr, 
									sizeof(udpsocket->addr));






}

























//-----------------------------------------------------------------------------------------//
//ソケット閉じる
//
//----------------------------------------------------------------------------------------//

/*void UdpSocket::close(void)
{
	///ソケットを閉じる
	closesocket(sockHandle);
	///winsock終了
	WSACleanup();

    if (sock != INVALID_SOCKET) {
        closesocket(sockHandle);
        sock = INVALID_SOCKET;
    }
}*/
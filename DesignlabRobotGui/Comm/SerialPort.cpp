/**
 *  ファイル名
 *		SerialPort.cpp
 *  説明
 *		シリアルポートをチェックしデータを送受信する
 *		ポートに変化が起こったらユーザに通知する
 *		送受信用のスレッドも別途作成する
 *  日付
 *		作成日: 2007/04/01(Sat)		更新日: 2007/11/03(Sat)
 */

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
#include <cassert>
#include <process.h>
  //#include "ASURA2GUI/pch.h"
#include "..\pch.h"
#include "SerialPort.h"
#include "..\System\DebugOutput.h"
#include <winsock2.h>//追加

namespace Comm
{
/**
 *	----------------------------------------------------------------------
 *		SerialPortクラス
 *	----------------------------------------------------------------------
 */

 /**
  *	------------------------------------------------------------
  *		SerialPortクラスのメンバ関数定義
  *	------------------------------------------------------------
  */
  /**
   *	コンストラクタ
   */
SerialPort::SerialPort()
{
    /**
     *	ポート番号
     */
     /// 自分のポート番号
    local_portNo = 10004;
    /// 相手のポート番号
    remort_portNo = 10003;
    /**
     *	IPアドレス
     */
     /// 自分のIPアドレス
     //sprintf(local_IPAdress,"192.168.0.154");
    sprintf(local_IPAdress, "192.168.0.157");
    /// 相手のIPアドレス
    sprintf(remort_IPAdress, "192.168.0.169");	//送信先IPアドレス


    /**
     *	WASData,Soketハンドル初期化
     *
     *	WASStartup	失敗:
     *				成功:  0
     *
     *	soket		失敗: SOCKET_ERROR(-1)
     *				成功: 負でない(含0)ソケット記述子
     */
    WSAHandle = 1;
    sockHandle = INVALID_SOCKET;

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
    readOverLappedStruct.hEvent = NULL;
    writeOverLappedStruct.hEvent = NULL;

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
    readThreadTerminateHandle = NULL;
    writeThreadTerminateHandle = NULL;

    /// スレッドフラグ
    isReadThreadAlive = FALSE;
    isWriteThreadAlive = FALSE;

    /**
     *	フラグの初期化
     */
    isListeningStopped = true;
    isCommOpen = false;
}

/**
 *	デストラクタ
 */
SerialPort::~SerialPort()
{
    closeSerialPort();
}

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
BOOL SerialPort::openSerialPort(HWND hOwnerWnd,				/// 受信ポートの親ウィンドウハンドル
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
    /**
     *	引数チェック
     */
     //DEBUG_ASSERT(portNo > 0 && portNo < 6);
   //	DEBUG_ASSERT(commOwnerWindowHandle != NULL);

   /**
    *	スレッド確認
    */
    /// 既にスレッドが走っていたら消去
    if (isWriteThreadAlive)
    {
        do
        {
            SetEvent(writeThreadTerminateHandle);//イベントオブジェクトをシグナル状態にする
        } while (isWriteThreadAlive);

        DEBUG_TRACE(TEXT("Thread ended\n"));
    }

    if (isReadThreadAlive)
    {
        do
        {
            SetEvent(readThreadTerminateHandle);
        } while (isReadThreadAlive);

        DEBUG_TRACE(TEXT("Thread ended\n"));
    }

    /**
     *	イベント作成
     */
     ///OVERLAPPED構造体の送受信イベント
    if (readOverLappedStruct.hEvent != NULL)
        ResetEvent(readOverLappedStruct.hEvent);
    readOverLappedStruct.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (writeOverLappedStruct.hEvent != NULL)
        ResetEvent(writeOverLappedStruct.hEvent);
    writeOverLappedStruct.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    /// スレッド破棄イベント
    if (readThreadTerminateHandle != NULL)
        ResetEvent(readThreadTerminateHandle);
    readThreadTerminateHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (writeThreadTerminateHandle != NULL)
        ResetEvent(writeThreadTerminateHandle);
    writeThreadTerminateHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

    /// イベント配列の初期化（HANDLE型はvoid*なので初期化はここのみで良い）
    readEventHandles[0] = readThreadTerminateHandle;
    readEventHandles[1] = readOverLappedStruct.hEvent;

    writeEventHandles[0] = writeThreadTerminateHandle;
    writeEventHandles[1] = writeOverLappedStruct.hEvent;

    /**
     *	ポート設定
     */
     /// シリアルポートの所有権を持つ（メッセージを受ける）ウィンドウハンドルをセット
    commOwnerWindowHandle = hOwnerWnd;

    /// ポート番号セット
    commPortNumber = portNo;

    /*************************************************************************************
     *	                       winsock設定
     */
     ///***********************************************************************************

       ///winsock初期化
    WSAHandle = WSAStartup(MAKEWORD(2, 0), &wsaData);///Ver.2.0
    //WSAHandle = WSASYSNOTREADY;//意図的エラー


    ///エラー処理　(初期化が失敗するとwinsockの機能が使えないので要確認)
    ///WSAStartup :成功した場合,戻り値0	
    assert(WSAHandle == 0);
    if (WSAHandle == 0) {
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
                assert(WSAHandle = !0);
        }
        return FALSE;
    }//*/
  /**
   *	バッファ関係
   */
   /// バッファ領域の確保
    if (readBufferSize != NULL) delete[] readBuffer;
    if (readBufferSize_ > MAX_BUFFER_SIZE) readBufferSize_ = MAX_BUFFER_SIZE;
    readBuffer = new unsigned char[readBufferSize_];

    if (writeBuffer != NULL) delete[] writeBuffer;
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

    /**
     *	その他
     */
     /// 通信イベントの設定
    commEvent = commEvent_;

    /// COMポートのための文字列
    LPTSTR portNoStr = (LPTSTR)malloc(256);
    LPTSTR dcbStr = (LPTSTR)malloc(256);

    /**
     *	排他処理開始
     */
    criticalSection.Lock();

    /// ソケットが既に開いていたら，閉じる
    if (commHandle != NULL)
    {
        ///CloseHandle(commHandle);
        //closesocket(sockHandle); ///ソケット閉鎖
        commHandle = NULL;
    }
    ///*


      ///ソケット作成
    sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
    //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？


    ///エラー処理
    ///失敗したらsocketは-1を返す
    if (sockHandle = !- 1) {
#define SOCK_HANDLE_VALUE ((HANDLE)(2))
        commHandle = SOCK_HANDLE_VALUE;///<---ハンドルの値がわからない
    }
    ///エラー
    else if (sockHandle == -1) {//INVALID_SOCKET
        ///エラー値を表示 (VC++エラールックアップにエラー値を入力して確認してください)
        DEBUG_TRACE(TEXT("socket error : %d\n", WSAGetLastError()));
        assert(sockHandle = !- 1);
        return FALSE;
    }
    //*/
      /// COMポートが既に開いていたら，閉じる
    if (commHandle != NULL)
    {
        CloseHandle(commHandle);
        commHandle = NULL;
    }

    /// ポートを開くための文字列の準備
    wsprintf(portNoStr, TEXT("COM%d"), portNo);
    wsprintf(dcbStr, TEXT("baud=%d parity=%c data=%d stop=%d"), baudRate, parity, dataBits, stopBits);

    /// COMポートへのハンドルを取得
  /*	commHandle = CreateFile(	portNoStr,						/// 通信ポート文字列 (COMX)
                  GENERIC_READ | GENERIC_WRITE,	/// 読み込み/書き込みの種類
                  0,								/// 通信ポートなので排他アクセスで開く
                  NULL,							/// 取得したハンドルの子プロセスへの継承を許可するかどうかを決めるSECURITY_ATTRIBUTES構造体へのポインタを指定
                  OPEN_EXISTING,					/// 通信デバイスを開く時はこのフラグ（OPEN_EXISTING）を指定する
                  FILE_FLAG_OVERLAPPED,			/// 非同期入出力（ファイルに対して複数の操作を同時に行うことが可能）
                  0);								/// テンプレートファイルへのGENERIC_READアクセスを持つハンドルを指定（0を指定する）

    /// COMポートが開かれたどうかのチェック
    if (commHandle == INVALID_HANDLE_VALUE)
    {
      /// ポートが見つからなかった時
      free(portNoStr);
      free(dcbStr);

      DEBUG_TRACE(TEXT("Failed to open COM Port Reason: %d\n"), GetLastError());

      return FALSE;
    }
  */
  /// 送受信バッファの容量を設定

/**
 *	COMポートの初期設定
 */
 /*	if (SetupComm(commHandle, readBufferSize, writeBufferSize))
   {
     /// COMMTIMEOUTS構造体の設定
     if (initializeCommTimeouts())
     {
       //
        *	SetCommMask()
        *		COMポートでのデータの着信や回線状態エラーの発生，出力バッファの最後の文字送信などの
        *		複数のイベントをまとめて監視する関数
        *		第1引数: 通信デバイスへのハンドル
        *		第2引数: 監視するイベント
       //
       if (SetCommMask(commHandle, commEvent_))
       {
         /// 現在のCOMポートの通信設定を取得する
         if (GetCommState(commHandle, &commDCBStruct))
         {
           /// DCB構造体の設定
           if (initializeCommDCB(dcbStr))
           {
             /// COMポートの通信設定を行う
             if ( SetCommState(commHandle, &commDCBStruct))
             {
             }
             else	/// COMポートの通信設定失敗
             {
               outputProcessErrorMessage("SetCommState()");
             }
           }
           else	/// DCB構造体の設定失敗
           {
             outputProcessErrorMessage("initializeCommTimeouts");
           }
         }
         else	/// 現在のCOMポートの通信設定の取得失敗
         {
           outputProcessErrorMessage("GetCommState()");
         }
       }
       else	/// SetCommMask()失敗
       {
         outputProcessErrorMessage("SetCommMask()");
       }
     }
     else	/// COMMTIMEOUTS構造体の設定失敗
     {
       outputProcessErrorMessage("initializeCommTimeouts()");
     }
   }
   else
   {
     /// 送受信バッファの設定失敗
     outputProcessErrorMessage("SetupComm()");
   }


   /// 通信ポート設定のための文字列消去
   free( portNoStr );
   free( dcbStr );

   /// 既にあるCOMポートの送受信バッファをクリアする
   PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
 */

 /**
  *	排他処理終了
  */
    criticalSection.Unlock();

    /// フラグの初期化
    isListeningStopped = false;
    isCommOpen = true;

    /// デバッグ用
    DEBUG_TRACE(TEXT("Initialization for communication port %d completed.\nUse Start monitor to communicate.\n"), portNo);

    return TRUE;

}	/// end of openSerialPort()

/**
 *	説明
 *		シリアルポートを閉じる
 *		ソケットを閉じる
 */
void SerialPort::closeSerialPort(void)
{
    // 既に閉じられていたら終了
    if (!isCommOpen)
        return;

    /// スレッドがsuspendされてたら、再開
    if (isListeningStopped)
        restartListening();

    ///ソケットを閉じる
    closesocket(sockHandle);

    ///winsock終了
    WSACleanup();

    /**
     *	スレッド終了
     */
    do
    {
        SetEvent(writeThreadTerminateHandle);
    } while (isWriteThreadAlive);

    /// スレッドの終了待ち
    WaitForSingleObject(writeThreadPointer->m_hThread, INFINITE);


    do
    {
        SetEvent(readThreadTerminateHandle);
    } while (isReadThreadAlive);

    /// スレッドの終了待ち
    WaitForSingleObject(readThreadPointer->m_hThread, INFINITE);

    DEBUG_TRACE(TEXT("Threads ended\n"));

    /// スレッドオブジェクトの消去
    delete writeThreadPointer;
    delete readThreadPointer;

    /**
     *	バッファの後始末
     */
    if (writeBuffer != NULL)
    {
        delete[] writeBuffer;
        writeBuffer = NULL;
    }

    if (readBuffer != NULL)
    {
        delete[] readBuffer;
        readBuffer = NULL;
    }

    /**
     *	ハンドルを閉じる
     */
     /// OVERLAPPED構造体のイベントハンドル
    if (readOverLappedStruct.hEvent != NULL)
    {
        CloseHandle(readOverLappedStruct.hEvent);
        readOverLappedStruct.hEvent = NULL;
    }

    /// OVERLAPPED構造体のイベントハンドル
    if (writeOverLappedStruct.hEvent != NULL)
    {
        CloseHandle(writeOverLappedStruct.hEvent);
        writeOverLappedStruct.hEvent = NULL;
    }

    /// スレッド終了のイベントハンドル
    if (readThreadTerminateHandle != NULL)
    {
        CloseHandle(readThreadTerminateHandle);
        readThreadTerminateHandle = NULL;
    }

    /// スレッド終了のイベントハンドル
    if (writeThreadTerminateHandle != NULL)
    {
        CloseHandle(writeThreadTerminateHandle);
        writeThreadTerminateHandle = NULL;
    }


    /// 未処理終了，バッファのクリア
    if (commHandle)
        PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    /// COMポートハンドル
    if (commHandle != INVALID_HANDLE_VALUE && commHandle != NULL)
    {
        CloseHandle(commHandle);
        commHandle = NULL;
    }

    /// フラグ処理
    isListeningStopped = true;
    isCommOpen = false;
}

/**
 *	説明
 *		DCBの設定
 *	引数
 *		deviceCtrlStr: デバイス制御情報を表す文字列へのポインタ
 */
BOOL SerialPort::initializeCommDCB(LPTSTR deviceCtrlStr)
{
    /**
     *	基本設定は引数であるdeviceCtrlStrによって決定する
     *	BaudRate, Parity, DataBit, StopBit
     */

     /**
    *	フロー制御
    */
    //commDCBStruct.fOutxCtsFlow		= FALSE;				/// CTSフロー制御なし
    //commDCBStruct.fOutxDsrFlow		= FALSE;				/// DSRフロー制御なし
    commDCBStruct.fDtrControl = DTR_CONTROL_ENABLE;	/// DTRライン有効
    commDCBStruct.fRtsControl = RTS_CONTROL_ENABLE;	/// RTSライン有効
    //commDCBStruct.fDsrSensitivity		= FALSE;				/// DSR制御なし 

    /**
     *	XON/XOFF
     */
    commDCBStruct.fOutX = FALSE;				/// XONなし
    commDCBStruct.fInX = FALSE;				/// XOFFなし
    //commDCBStruct.fTXContinueOnXoff	= TRUE;					/// XON送信後も送信を継続
    //commDCBStruct.XonLim				= 2048;					/// XONが送られるまでに格納できる最小バイト数
    //commDCBStruct.XoffLim				= 2048;					/// XOFFが送られるまでに格納できる最小バイト数
    //commDCBStruct.XonChar				= 0x11;					/// XONキャラクタ
    //commDCBStruct.XoffChar			= 0x13;					/// XOFFキャラクタ 

    /**
     *	その他の制御
     */
    commDCBStruct.fBinary = TRUE;					/// バイナリモード
    //commDCBStruct.fNull				= FALSE;				/// NULLバイトは破棄しない
    commDCBStruct.fAbortOnError = FALSE;				/// エラー時は読み書き操作を終了
    //commDCBStruct.fErrorChar			= FALSE;				/// パリティエラー時のキャラクタ置換なし
    //commDCBStruct.ErrorChar			= 0x00;					/// 置換キャラクタ
    //commDCBStruct.EofChar				= 0x03;					/// データ終了キャラクタ
    //commDCBStruct.EvtChar				= 0x0a;					/// イベントキャラクタ 

    /// DCB構造体を設定する
    if (BuildCommDCB(deviceCtrlStr, &commDCBStruct))
    {
        return TRUE;
    }
    else
    {
        outputProcessErrorMessage("BuildCommDCB()");
        return FALSE;
    }


}

/**
 *	説明
 *		COMMTIMEOUTSの設定
 */
BOOL SerialPort::initializeCommTimeouts(void)
{
    /**
     *	タイムアウトを設定し，受信開始から10ms以内に応答が無ければ無応答
     */
     /**
      *	受信タイムアウト
      *		ReadTotalTimeoutMultiplier x 受信バイト数 + ReadTotalTimeoutConstant
      *	送信タイムアウト
      *		WriteTotalTimeoutMultiplier x 送信バイト数 + WriteTotalTimeoutConstant
      */
    memset(&commTimeoutsStruct, 0, sizeof(commTimeoutsStruct));

    commTimeoutsStruct.ReadIntervalTimeout = 100;
    commTimeoutsStruct.ReadTotalTimeoutMultiplier = 0;
    commTimeoutsStruct.ReadTotalTimeoutConstant = 0;

    commTimeoutsStruct.WriteTotalTimeoutMultiplier = 100;
    commTimeoutsStruct.WriteTotalTimeoutConstant = 100;

    if (!SetCommTimeouts(commHandle, &commTimeoutsStruct))
    {
        outputProcessErrorMessage("SetCommTimeouts()");
        return FALSE;
    }

    return TRUE;

}

/**
 *	COMポートの監視関数（スレッド開始・停止・終了）
 */
 /// モニタリング開始
BOOL SerialPort::startListening(void)
{
    /**
     *	CWinThread* AfxBeginThread(	AFX_THREADPROC pfnThreadProc,
       *								LPVOID pParam,
     *								int nPriority = THREAD_PRIORITY_NORMAL,
     *								UINT nStackSize = 0,
     *								DWORD dwCreateFlags = 0,
     *								LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL )
     */

     //////////受信スレッド開始///////////////////////////////////////////////////////////
    readThreadPointer = AfxBeginThread(readEventListenThread,    //制御関数
                      (LPVOID)this,             //制御関数に渡すパラメータ
                      THREAD_PRIORITY_NORMAL,   //スレッドの優先順位
                      0,                        //作成直後にスレッドを開始
                      CREATE_SUSPENDED
    );

    readThreadPointer->m_bAutoDelete = FALSE;
    readThreadPointer->ResumeThread();


    ///////// 送信スレッド開始///////////////////////////////////////////////////////////
    writeThreadPointer = AfxBeginThread(writeEventListenThread,
                        (LPVOID)this,
                        THREAD_PRIORITY_NORMAL,
                        0,
                        CREATE_SUSPENDED
    );

    writeThreadPointer->m_bAutoDelete = FALSE;
    writeThreadPointer->ResumeThread();

    DEBUG_TRACE(TEXT("Thread started\n"));

    /// フラグの反転
    isListeningStopped = false;

    return TRUE;
}

/// モニタリング再開
BOOL SerialPort::restartListening(void)
{
    if (!isListeningStopped)
        return FALSE;

    /// スレッド再開
    readThreadPointer->ResumeThread();
    writeThreadPointer->ResumeThread();

    DEBUG_TRACE(TEXT("Thread resumed\n"));

    /// フラグの決定
    isListeningStopped = false;

    return TRUE;
}

/// モニタリング停止
BOOL SerialPort::stopListening(void)
{
    if (isListeningStopped)
        return FALSE;

    /// スレッド停止
    readThreadPointer->SuspendThread();
    writeThreadPointer->SuspendThread();

    DEBUG_TRACE(TEXT("Thread suspended\n"));

    /// フラグの決定
    isListeningStopped = true;

    return TRUE;
}

/**
 *	説明
 *		COMポートへデータを送信
 *	引数
 *		sendData: 書き込みデータへのポインタ
 *		bytesToSend: 書き込みサイズ
 */
void SerialPort::sendData(char* sendData, DWORD bytesToSend)//unsigned char
{
    //sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
    /// COMポートが開かれているかのチェック
    //assert(commHandle != 0);

    /// 初期化
    memset(writeBuffer, 0, sizeof(writeBuffer));

    /// データコピー
    int i;
    for (i = 0; i < (int)bytesToSend; i++)
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

/**
 *	------------------------------------------------------------
 *		SerialPortのprotectedなメンバ関数
 *	------------------------------------------------------------
 */

 /**
  *	説明
  *		通信エラーの取得
  *	引数
  *		書き込み先へのポインタ
  */
void SerialPort::outputProcessErrorMessage(char* errorText)
{
    TCHAR* message;
    TCHAR error[1024];

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	/// デフォルト言語
            (TCHAR*)&message,
            0,
            NULL
    );

    _stprintf(error,
          TEXT("WARNING:  %S Failed with the following error: \n%s\nPort: %d\n"),
          errorText,
          message,
          commPortNumber
    );

    MessageBox(NULL, error, TEXT("Serial Port Application Error"), MB_ICONSTOP);

    LocalFree(message);
}









/**
 *	受信に用いるスレッド関数
 */
UINT SerialPort::readEventListenThread(LPVOID pUserData)//LPVOID UserData
{
    /*	/// 引数のVOIDポインタをSerialPortクラスのポインタへキャスト
      SerialPort* port = (SerialPort*) pUserData;

      /// スレッドが走っていることを通知するため，状態変数をTRUEにする
      port->isReadThreadAlive = TRUE;

      /// ローカル変数の宣言
      DWORD		eventMask = 0;
      DWORD		readThreadEvents = 0;
      DWORD		readEventMask = 0;
      DWORD		commErrors = 0;
      BOOL		isEventOccurred = TRUE;
      COMSTAT		comStat;

      /// ポートが開かれているかチェックして，起動時に通信バッファをクリア
      if (port->commHandle)
        PurgeComm(port->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

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
        isEventOccurred = WaitCommEvent(port->commHandle, &eventMask, &port->readOverLappedStruct);

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
              port->outputProcessErrorMessage( "WaitCommEvent()" );
              break;
            }
          }	/// switch ( commErrors = GetLastError() )
        }	/// end if isEventOccurred
    */
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
        /*		isEventOccurred = ClearCommError(port->commHandle, &commErrors, &comStat);

            switch (readThreadEvents)
            {
              case WAIT_OBJECT_0:	/// スレッドシャットダウンイベント
              {
                /// このイベントは最優先であり最初に提供される
                port->isReadThreadAlive = FALSE;

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
                GetCommMask(port->commHandle, &readEventMask);

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
                  readData(port, comStat);//ここをいじって出力
                }

                break;
              }
              default:
                break;
            } /// end of switch

            /// 受信バッファのデータ読み込みが終わった時点でイベントリセット
            ResetEvent(port->readOverLappedStruct.hEvent);

          } /// 無限ループ
          */
    return 0;

}

/**
 *	受信スレッドで用いる受信バッファからの読み出し関数
 */
void SerialPort::readData(SerialPort* port, COMSTAT& comStat)
{
    BOOL	isReadingToTry = TRUE;		/// 読み込みを行うかどうか
    BOOL	isReadingCompleted = TRUE;		/// 読み込みが完了したかどうか
    BOOL	isReadingSucceeded = TRUE;		/// 読み込みが成功したかどうか
    DWORD	errorCode = 0;		/// エラー
    DWORD	bytesRead = 0;		/// 読み込んだバイト数
    int i;										/// カウンタ			


    /////////////////////UDP受信////////////////////////////////////
    ///ソケット作成
  /*	port->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);

    assert(port->WSAHandle =! 0);


    port->addr.sin_family	= AF_INET;									///アドレスファミリ
    port->addr.sin_port		= htons(port->remort_portNo);//htons(10003);//					///相手のポート番号
    port->addr.sin_addr.S_un.S_addr = inet_addr(port->remort_IPAdress);//inet_addr("192.168.0.169");//	///相手のIPアドレス


    bind(port->sockHandle, (struct sockaddr *)&(port->addr), sizeof(port->addr));

    while(1){

    ///ソケットのデータを受信
      isReadingSucceeded =recvfrom( port->sockHandle,
                    port->readBuffer,//Rxbuffer,
                    port->bytesToRead,//length2,
                    0,
                    (struct sockaddr *)&port->addr,
                    sizeof(port->addr));

      if(isReadingSucceeded == -1);

    }

    */



    /**
     *	無限ループ開始
     *	読み込むべき全てのデータ処理が終了して初めてループを抜ける
     */
    while (1)
    {

        ZeroMemory(port->readBuffer, MAX_BUFFER_SIZE);//メモリーの中身を0にする

        /**
         *	排他処理開始
         *	COMポートの所有権を取得する
         */
        port->criticalSection.Lock();

        /**
         *	ClearCommError()によりCOMSTAT構造体を更新し他のエラーを消去
         */
        isReadingSucceeded = ClearCommError(port->commHandle, &errorCode, &comStat);
        port->bytesToRead = comStat.cbInQue;

        /// 排他処理終了
        port->criticalSection.Unlock();

        /// 読み込むべきデータあるかどうか確認し，なかったらループから抜ける
        if (port->bytesToRead == 0)
        {
            break;
        }
        else if (port->bytesToRead > 0)
        {
            isReadingToTry = TRUE;

            /// 読み込もうとするバッファサイズが最大値より大きかったら
            if (port->bytesToRead > port->readBufferSize)
                port->bytesToRead = port->readBufferSize;
        }

        /**
         *	排他処理開始
         *	読み込む前にCOMポートの所有権を取得する
         */
        port->criticalSection.Lock();

        /// データ読み込みを行う場合
        if (isReadingToTry)
        {
            /// 変数の初期化
            port->readOverLappedStruct.Offset = 0;
            port->readOverLappedStruct.OffsetHigh = 0;

            //ポートからデータを読み込む
            isReadingSucceeded = ReadFile(
                            port->commHandle,					/// 対象COMポートへのハンドル 
                            port->readBuffer,					/// 受信バッファへのポインタ
                            port->bytesToRead,					/// 読み込もうとするバイト数
                            &bytesRead,							/// 実際に読み込んだバイト数
                            &port->readOverLappedStruct		/// 情報を書き込むOVERLAPPED構造体へのポインタ
            );

            /// 読み込みエラーが出た場合の処理 
            if (!isReadingSucceeded)
            {
                switch (errorCode = GetLastError())
                {
                    case ERROR_IO_PENDING:
                    {
                        /**
                         *	非同期入出力（ReadFile()）がまだ処理中である
                         *	したがって，GetOverlappedResults()で事後処理を行う
                         */
                        isReadingCompleted = FALSE;
                        break;
                    }
                    default:
                    {
                        /// 予期しないエラーなので，このエラー処理
                        port->outputProcessErrorMessage("ReadFile()");
                        break;
                    }
                }	/// end of switch
            }
            else
            {
                /// ReadFile()が終了した場合
                /// この場合GetOverlappedResults()を呼び出す必要は無い
                isReadingCompleted = TRUE;
            }	/// close if ( !isReadingSuccessed )

        }	/// close if ( isReadingToTry )

        /**
         *	ReadFile()が終了しなかったのでGetOverlappedResult()により読み込み動作を完了させる
         */
        if (!isReadingCompleted)
        {
            /**
             *	GetOverlappedResult
             *		第１引数：ファイルハンドル
             *		第２引数：OVERLAPPED構造体のアドレス
             *		第３引数：処理済みのバイト数を受け取る変数のアドレス
             *		第４引数：フラグ
             *
             *	非同期I/Oがまだ完了していなかった場合，第4引数のフラグが
             *	TRUEだと完了するまで関数の内部で待機し，FALSEだと待たずに関数から戻る．
             */
            isReadingSucceeded = GetOverlappedResult(
                                  port->commHandle,				/// COMポートハンドル 
                                  &port->readOverLappedStruct,	/// 情報を書き込むOverlapped構造体
                                  &bytesRead,						/// 読み込んだバイト数
                                  TRUE							/// 待機フラグ
            );

            /// エラーコード処理 
            if (!isReadingSucceeded)
            {
                port->outputProcessErrorMessage("GetOverlappedResults() in ReadFile()");
            }

        }  /// close if ( !isReadingCompleted )

        /// 一時受信バッファから記録用リングバッファに保存
        for (i = 0; i < (int)bytesRead; i++)
            port->receiveBuffer.get(&port->readBuffer[i]);

        /**
         *	排他処理終了
         *	読み込む終わったのでCOMポートの所有権を放棄
         */
        port->criticalSection.Unlock();

        /**
         *	所有権を持ってる親ウィンドウへ受信を通知
         *	通知パラメータ：
         *		wParam：読み込みデータ
         *		lParam：COMポート番号
         */
         /// オーナーウィンドウへポスト
        if (port->commOwnerWindowHandle != NULL)
            ::PostMessage(
                    port->commOwnerWindowHandle,
                    WM_COMM_RXCHAR,
                    (WPARAM)(port->readBuffer),
                    MAKELPARAM((WORD)port->commPortNumber, (WORD)bytesRead)
            );
    }	/// 無限ループの終わり
}


/**
 *	送信に用いるスレッド関数
 */
UINT SerialPort::writeEventListenThread(LPVOID pParam)//制御関数
{
    /// 引数のVOIDポインタをSerialPortクラスのポインタへキャスト
    SerialPort* port = (SerialPort*)pParam;

    /// スレッドが走っていることを通知するため，状態変数をTRUEにする
    port->isWriteThreadAlive = TRUE;

    /// ローカル変数の宣言
  //	DWORD		eventMask = 0;
  //	DWORD		writeEventMask = 0;
    DWORD		writeThreadEvents = 0;
    //	DWORD		writeEvents = 0;
    //	DWORD		commErrors = 0;
    //	BOOL		isEventOccurred = TRUE;
    COMSTAT		comStat;

    /// ポートが開かれているかチェックして，起動時に通信バッファをクリア
    if (port->commHandle)
        PurgeComm(port->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    /**
     *	 無限ループ
     *	このループはスレッドが走っている限り継続
     */
    while (1)
    {
        /// WaitForMultipleObjects()でイベント待ち
        writeThreadEvents = WaitForMultipleObjects(2, port->writeEventHandles, FALSE, INFINITE);

        switch (writeThreadEvents)
        {
            case WAIT_OBJECT_0:	/// スレッドシャットダウンイベント
            {
                /// このイベントは最優先であり最初に提供される
                port->isWriteThreadAlive = FALSE;

                /// スレッド破棄
                AfxEndThread(200);
                break;
            }
            case WAIT_OBJECT_0 + 1:	/// 書き込みイベント
            {
                /// ポートへデータ書き込み
                writeData(port, comStat);

                break;
            }
            default:
                break;
        }	/// end of switch (writeThreadEvents)

    }	/// 無限ループ

    return 0;
}

/**
 *	送信スレッドで用いる送信バッファへの書き込み関数
 */
void SerialPort::writeData(SerialPort* port, COMSTAT& comStat)
{
    /// ローカル変数の宣言
    BOOL	isWritingToTry = TRUE;		/// 書き込みを行うかどうか
    BOOL	isWritingCompleted = TRUE;		/// 書き込みが完了したかどうか
    BOOL	isWritingSucceeded = TRUE;		/// 書き込みが成功したかどうか
    DWORD	errorCode = 0;		/// エラーコード
    DWORD	bytesWritten = 0;
    char	ErrMassage[32] = { 0 };		///エラーコード表示用

    ///ソケット作成
    port->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
    //assert(port->sockHandle =! -1);
    assert(port->WSAHandle = !0);

    //TEST_BUFFER
    char Txbuffer[8] = { 0 };
    ///ゲートウェイID
    Txbuffer[0] = (char)(0x00);
    Txbuffer[1] = (char)(0x0A);
    int length2 = sizeof(Txbuffer);

    /// 待機イベントをリセット
    ResetEvent(port->writeOverLappedStruct.hEvent);

    port->criticalSection.Lock();

    /**
     *	ClearCommError()によりCOMSTAT構造体を更新し他のエラーを消去
     */
    isWritingSucceeded = ClearCommError(port->commHandle, &errorCode, &comStat);

    //SOCKET sock2;
    //struct sockaddr_in addr2;
    //sock2 = socket(AF_INET, SOCK_DGRAM, 0);	//ソケット作成
  /*
    if(sock2 == INVALID_SOCKET){
      sprintf(ErrMassage,"error : %d\n", WSAGetLastError());
      port->outputProcessErrorMessage( ErrMassage );
      assert(sock2 =! INVALID_SOCKET);
    }
    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(10003);	//送信先ポート番号
    addr2.sin_addr.S_un.S_addr = inet_addr("192.168.0.169");	//送信先IPアドレス
  */
  //isWritingSucceeded =sendto(sock2, Txbuffer, length2, 0, (struct sockaddr *)&addr2, sizeof(addr2));


  //closesocket(sock2);

  /// 書き込むべきデータあるかどうか確認し、ない場合だけ書き込む <--シリアル用?
/*	if (comStat.cbOutQue > 0)
  {
    port->criticalSection.Unlock();
    return;
  }
*/		///送信先のネットワーク情報を準備
///ポートIPアドレス番号,IPアドレスの文字ストリングをネットワークバイトオーダーに変換
    port->addr.sin_family = AF_INET;									///アドレスファミリ
    port->addr.sin_port = htons(port->remort_portNo);//htons(10003);//					///相手のポート番号
    port->addr.sin_addr.S_un.S_addr = inet_addr(port->remort_IPAdress);//inet_addr("192.168.0.169");//	///相手のIPアドレス



    ///ソケットのデータを送信
    isWritingSucceeded = sendto(port->sockHandle,
                  port->writeBuffer,//Txbuffer,
                  port->bytesToWrite,//length2,
                  0,
                  (struct sockaddr*)&port->addr,
                  sizeof(port->addr));
    /*
        isWritingSucceeded = sendto(port->sockHandle,							///ソケット記述子
                      port->writeBuffer,					///送信するメッセージが入っているバッファーへのポインター
                      port->bytesToWrite,					///バッファー内のメッセージの長さ
                      0,									///フラグの設定
                      (struct sockaddr *)&port->addr,		///アドレス一式
                      sizeof(port->addr));						///アドレスのサイズ
    */

    /*
      if (isWritingToTry)
      {

        /// 変数の初期化
        port->writeOverLappedStruct.Offset = 0;
        port->writeOverLappedStruct.OffsetHigh = 0;

        isWritingSucceeded = WriteFile(
                        port->commHandle,				/// COMポートへのハンドル
                        port->writeBuffer,				/// 書き込みバッファへのポインタ
                        port->bytesToWrite,				/// 書き込むデータ長さ
                        &bytesWritten,					/// 実際に書き込んだバイト数を記録するポインタ
                        &port->writeOverLappedStruct	/// 書き込み用のOVERLAPPED構造体
                        );

        /// エラー時の処理
        assert(isWritingSucceeded =! -1);

        if (isWritingSucceeded == -1)
        {
          /// エラーコード取得

          errorCode = errno ;///GetLastError();

          switch (errorCode)
          {
            case ERROR_IO_PENDING:
            {
                /// WriteFile()が直ぐに終了しなかった場合の通常の返り値なのでGetOverlappedResults()へ移行
                bytesWritten = 0;

                /// 書き込み終了フラグはOFF
                isWritingCompleted = FALSE;
                break;
            }

            default:
            {
                /// 他の全てのエラー
                sprintf(ErrMassage,"sendto Err code:%4d",errorCode);
                port->outputProcessErrorMessage( ErrMassage );
                port->criticalSection.Unlock();
            }
          }	/// end of switch
        }
        /// 変数の初期化
        port->writeOverLappedStruct.Offset = 0;
        port->writeOverLappedStruct.OffsetHigh = 0;

        ///送信関数
        isWritingSucceeded = WriteFile(
                        port->commHandle,				/// COMポートへのハンドル
                        port->writeBuffer,				/// 書き込みバッファへのポインタ
                        port->bytesToWrite,				/// 書き込むデータ長さ
                        &bytesWritten,					/// 実際に書き込んだバイト数を記録するポインタ
                        &port->writeOverLappedStruct	/// 書き込み用のOVERLAPPED構造体
                        );

        /// エラー時の処理
        if (!isWritingSucceeded)
        {
          /// エラーコード取得
          errorCode = GetLastError();

          switch (errorCode)
          {
            case ERROR_IO_PENDING:
            {
                /// WriteFile()が直ぐに終了しなかった場合の通常の返り値なのでGetOverlappedResults()へ移行
                bytesWritten = 0;

                /// 書き込み終了フラグはOFF
                isWritingCompleted = FALSE;
                break;
            }
            default:
            {
                /// 他の全てのエラー
                port->outputProcessErrorMessage( "WriteFile()" );
                port->criticalSection.Unlock();
            }
          }	/// end of switch
        }
        else
        {
          port->criticalSection.Unlock();
        }	/// end of if (!isWritingSuccessed)

      } // end of if( isWritingToTry )
    */
    /*
      /// 書き込みが一定時間で終了しなかったら
      if (!isWritingCompleted)
      {

         //	GetOverlappedResult
         //		第１引数：ファイルハンドル
         //		第２引数：OVERLAPPED構造体のアドレス
         //		第３引数：処理済みのバイト数を受け取る変数のアドレス
         //		第４引数：フラグ
         //
         //	非同期I/Oがまだ完了していなかった場合，第4引数のフラグが
         //	TRUEだと完了するまで関数の内部で待機し，FALSEだと待たずに関数から戻る．

        isWritingSucceeded = GetOverlappedResult(
                              port->commHandle,				/// COMポートへのハンドル
                              &port->writeOverLappedStruct,	/// OVERLAPPED構造体
                              &bytesWritten,					/// 書き込んだバイト数
                              TRUE							/// 待機フラグ（書き込み終了まで待機する）
                            );

        port->criticalSection.Unlock();

        /// GetOverlappedResult()により書き込み終了
        isWritingCompleted = TRUE;

        /// エラーコード処理
        if (!isWritingSucceeded)
        {
          port->outputProcessErrorMessage( "GetOverlappedResults() in WriteFile()" );
        }

      } // end if ( !isWritingCompleted )

      /// 書き込む予定だったバイト数と実際に書き込んだバイト数のチェック
      if (bytesWritten != port->bytesToWrite)
      {
        DEBUG_TRACE(
              TEXT("WARNING: WriteFile() error.. Bytes Written: %d; Message Length: %d\n"),
              bytesWritten,
              port->bytesToWrite
              );
      }*/
}

}	/// end of namespace Comm
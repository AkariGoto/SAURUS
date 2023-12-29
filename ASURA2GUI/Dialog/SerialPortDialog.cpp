/**
 *  ファイル名
 *		SerialPortDialog.cpp
 *  説明
 *		シリアル通信のログ表示のためのダイアログ
 *  日付
 *		作成日: 2007/06/11(Mon)		更新日: 2007/11/10(Sat)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\stdafx.h"

#include "SerialPortDialog.h"

#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Asura;
using namespace Comm;
using namespace Data;
using namespace System;
using namespace Const;
using namespace Plan;
//追加
using namespace UDP;

unsigned char changeflag=0x0F;//クローラ状態
#define BIT(num)                 ((unsigned int)1 << (num))
char LED=0;
long LEDD;
long OV=0;
char LLED=0;
long LLEEDD=0;
/**
 *	----------------------------------------------------------------------
 *		CSerialPortDialogクラス
 *	----------------------------------------------------------------------
 */

/**
 *		フレームワークが実行時にクラス名やクラス階層でのクラスの位置を知るために
 *		CObject クラスの動的派生オブジェクトに必要な C++ のコードを生成する．
 *
 *		第一引数は派生クラス（このクラス：CSerialPortDialog），第二引数は基本クラス（CDialog）をとる．
 */
IMPLEMENT_DYNAMIC(CSerialPortDialog, CDialog)

BEGIN_MESSAGE_MAP(CSerialPortDialog, CDialog)

	/// Windows予約
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()

	/// カスタムメッセージ
	ON_MESSAGE(WM_DLG_COMM_DISPLAY,			OnSwitchDisplayMode)
	ON_MESSAGE(WM_DLG_COMM_UPDATE,			OnRedrawDialog)
	ON_MESSAGE(WM_DLG_COMM_START_LISTEN,	OnStartPortListening)
	ON_MESSAGE(WM_DLG_COMM_STOP_LISTEN,		OnStopPortListening)
	ON_MESSAGE(WM_PLAN_SENDING_COMM_DATA,	OnSendCommandData)

	/// ログ
	ON_MESSAGE(WM_DLG_COMM_START_LOGGING,	OnStartLogging)
	ON_MESSAGE(WM_DLG_COMM_STOP_LOGGING,	OnStopLogging)

/**
 *		通信イベント
 */
	ON_MESSAGE(WM_COMM_CTS_DETECTED,		OnHandleReceiveMessage)
	ON_MESSAGE(WM_COMM_BREAK_DETECTED,		OnHandleReceiveMessage)
	ON_MESSAGE(WM_COMM_ERR_DETECTED,		OnHandleReceiveMessage)
	ON_MESSAGE(WM_COMM_RING_DETECTED,		OnHandleReceiveMessage)

	/// パケット受信イベント
	ON_MESSAGE(WM_COMM_RXFLAG_DETECTED,		OnHandleReceiveMessage)

	/// 受信イベント
	ON_MESSAGE(WM_COMM_RXCHAR,				OnReceiveData)

END_MESSAGE_MAP()

/**
 *	----------------------------------------------------------------------
 *		CSerialPortDialogクラスのメンバ関数定義
 *	----------------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
 CSerialPortDialog::CSerialPortDialog(CWnd* pParent /*=NULL*/)
{
	/// 親ウィンドウのポインタ取得
	pParentWindow = pParent;

	/// タイマID
	timerID = 0;

	/// カウンタ
	int i, j;
	/// 送信パケットバッファ
	for (i=0; i<SCI_MAX_JOINT_NUM; i++)
	{
		for (j=0; j<SCI_PACKET_SIZE_SHORT; j++)
			ZeroMemory(shortPacket[i], SCI_PACKET_SIZE_SHORT);
	}

	ZeroMemory(longPacket, SCI_PACKET_SIZE_LONG);

	/// 受信バッファの作成
	rxBuffer.setSize(2048);
}

CSerialPortDialog::~CSerialPortDialog()
{

}

void CSerialPortDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *		CSerialPortDialog メッセージ ハンドラ
 *	------------------------------------------------------------
 */
/// Windows管理
HBRUSH CSerialPortDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
    switch  ( nCtlColor )
	{
		case CTLCOLOR_DLG:	/// 背景色の設定
		{
			if (*pWnd == *this)
				hbr = backColorBrush;

			break;
		}	/// case CTLCOLOR_DLG:
		case CTLCOLOR_MSGBOX:
		case CTLCOLOR_EDIT:
		{
			/// 文字色
			pDC->SetTextColor( editColor );
			/// 背景色
			pDC->SetBkColor( editBkColor );
				
			hbr = editBkColorBrush;

			break;
		}
		case CTLCOLOR_STATIC:
		{
			int id = pWnd->GetDlgCtrlID();

			if ( IDC_TEXT_JOINT == id || 
				IDC_TEXT_FOOTHOLD == id || 
				IDC_TEXT_BODY == id ||
				IDC_TEXT_GAIT == id ||
				IDC_TEXT_TIME == id )
			{
				/// 文字色
				pDC->SetTextColor( staticColor );
				/// 背景色
				pDC->SetBkColor( staticBkColor );
				
				hbr = staticBkColorBrush;
			}
			else
			{
				/// 文字色
				pDC->SetTextColor( staticColor );
				/// 背景色
				pDC->SetBkColor( staticBkColor );
				
				hbr = staticBkColorBrush;
			}

			break;
		}	/// end of case CTLCOLOR_STATIC:
		default:
			break;
	}	/// switch  ( nCtlColor )
	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

/// Windowsタイマ
void CSerialPortDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	PostMessage(WM_DLG_COMM_UPDATE);

	CDialog::OnTimer(nIDEvent);
}

/**
 *	カスタムメッセージ
 */
/// ダイアログの表示/非表示の切替
LRESULT CSerialPortDialog::OnSwitchDisplayMode(WPARAM wParam, LPARAM lParam)
{
	if ( (int)wParam )
	{
		ShowWindow(SW_SHOW);
		
		timerID = 1;
		SetTimer(timerID, 100, NULL);

		return 1;
	}
	else
	{

		ShowWindow(SW_HIDE);

		if (timerID == 1)
			KillTimer(timerID);
		
		timerID = 0;

		return 0;
	}

	return 2;
}

/// 画面の更新
LRESULT CSerialPortDialog::OnRedrawDialog(WPARAM wParam, LPARAM lParam)
{
	int i;

	/// パケットデータのログ作成
	if ( isSendingDataDisplayed )
	
	{
		switch (sciPacketType)
		{
			case Comm::SHORT:
			{
				CString txDataStr;

				for (i=0; i<SCI_MAX_JOINT_NUM; i++)
				{
					shortPacketString[i].Format(	TEXT("0x%.2X 0x%.2X 0x%.2X 0x%.2X 0x%.2X 0x%.2X 0x%.2X\r\n"),
														shortPacket[i][0], shortPacket[i][1], shortPacket[i][2],
														shortPacket[i][3], shortPacket[i][4], shortPacket[i][5],
														shortPacket[i][6]);
					txDataStr += shortPacketString[i];
	
					if ( isLogging )
						pSendLogWriter->writeFormattedString( shortPacketString[i] );
					
				}
				/// 送信用のログ表示
				displayLogData(IDC_SENDING_EDIT, txDataStr);
			}
			break;

			case Comm::LONG:
			{
				CString txDataStr[6];

				txDataStr[0].Format( TEXT("HEADER1:   0x%.2X\r\nHEADER2:   0x%.2X\r\nSIZE:   0x%.2X\r\nJOINT:   0x%.2X\r\nCMD:   0x%.2X\r\nCHECK:   0x%.2X\r\n\r\n"), 
										longPacket[0], longPacket[1], longPacket[2], longPacket[3], longPacket[4], longPacket[5]);

				for (i=0;i<4;i++)
					txDataStr[i + 1].Format( TEXT("LEG-%01d:   0x%.2X-0x%.2X     0x%.2X-0x%.2X     0x%.2X-0x%.2X\r\n"), 
												i + 1, longPacket[6*(i+1)], longPacket[6*(i+1)+1], longPacket[6*(i+1)+2], longPacket[6*(i+1)+3], longPacket[6*(i+1)+4], longPacket[6*(i+1)+5]);

				txDataStr[5].Format( TEXT("\r\nCHECK1:   0x%.2X\r\nCHECK2:   0x%.2X\r\n\r\n"), 
										longPacket[30], longPacket[31]);

				longPacketString = txDataStr[0] + txDataStr[1] + txDataStr[2] + txDataStr[3] + txDataStr[4] + txDataStr[5];

				if ( isLogging )
					pSendLogWriter->writeFormattedString( longPacketString );
					
				/// 送信用のログ表示
				displayLogData(IDC_SENDING_EDIT, longPacketString);
			}
			break;

			case Comm::DEBUGGING:
			{
			}

			default:
			break;
		
		}	/// end of switch
	}/// end of if
	

	if(isRecievingDataDisplayed)
	{
	
	
	}


	/// 受信用のログ表示
	displayLogData(IDC_RECEIVING_EDIT, rxDataStr);

	return 1;
}

/// ポートのリスニング開始
LRESULT CSerialPortDialog::OnStartPortListening(WPARAM wParam, LPARAM lParam)
{
	pCommPort->startListening(); //ここからSerialPort.cppのスレッド開始される

	//pUdpPort->UDPstartListening(); // ->ここをUDPのスレッドにすればいける？　
	                                  //もしくは、ここにUDPstartListeningを追加すればシリアルとUDPを同じタイミングで使えるかも
	                                  //startListening() -> UDPstartListening()に変える.
	//restartListening();

	return 1;
}

/// ポートのリスニング停止
LRESULT CSerialPortDialog::OnStopPortListening(WPARAM wParam, LPARAM lParam)
{
	pCommPort->stopListening(); //startListening() -> UDPstartListening()に変える.

	//pUdpPort->UDPstopListening(); //

	return 1;
}

/// 指令値をロボットへシリアル送信
LRESULT CSerialPortDialog::OnSendCommandData(WPARAM wParam, LPARAM lParam)//問題　//(WPARAM)(&asuraXData), (LPARAM)(&planData)
{
	if ( (AsuraData*)wParam == NULL )
		return -1;

	/**
	 *		ローカル変数
	 */
	int i;

	/// 送信データに代入
	commAsuraData = *( (AsuraData*)wParam );

	switch (sciPacketType)
	{
	/*	case Comm::SHORT:
		{
			/// パケット送信
			for (i=0;i<SCI_MAX_JOINT_NUM;i++)
			{
				/// パケットデータ作成
				ZeroMemory(shortPacket[i], SCI_PACKET_SIZE_SHORT);
				//buildPacket(Comm::SHORT, shortPacket[i], i+1, PACKET_CMD_JNT_POS);
				//pCommPort->sendData((&shortPacket[i][0]), (DWORD)SCI_PACKET_SIZE_SHORT);
			}
		}
		break;
		*/
		case Comm::LONG:
		{
			/// パケット送信
			ZeroMemory(longPacket, SCI_PACKET_SIZE_LONG);
			//if(getLocomotionStyle()==LEGGED)//問題
				buildPacket(Comm::LONG, longPacket, SCI_ALL_JOINTS, PAKCET_CMD_WALK);
			//else
				//buildPacket(Comm::LONG, longPacket, SCI_ALL_JOINTS, PAKCET_CMD_TRACK);
			pCommPort->sendData(longPacket, (DWORD)SCI_PACKET_SIZE_LONG);		
		}
		break;

		case DEBUGGING:
		{
		}
		break;
	}


	return 1;
}

/// シリアル受信データ処理
LRESULT CSerialPortDialog::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	/// 送られてくるアドレスが有効かどうか
	if ( (char*)wParam == NULL )
		return -1;

	/// メッセージデータを受信
	char* data = (char*)wParam;
	int length = HIWORD(lParam);

	/// 受信バッファに書き込み
	rxBuffer.write( (unsigned char*)data, length);

	unsigned char buffer[4];

	int i;
	for (i=0; i<100; i++)
	{
		rxBuffer.read(&buffer[0]);
		if ( buffer[0] == 0x01)
		{
			int j;
			for (j=1; j<4; j++)
				rxBuffer.read( &buffer[j] );
			
			/// 表示用変数に書き込み
			rxDataStr.Format( TEXT("0x%2X 0x%2X 0x%2X 0x%2X\r\n"), buffer[0], buffer[1], buffer[2], buffer[3]);
			
		//	SetDlgItemText(ID入れる, rxDataStr); //GUI上に受信した計測値表示?

			break;
		}
	}




	return 1;
}

/// シリアル受信データの例外イベント処理
LRESULT CSerialPortDialog::OnHandleReceiveMessage(WPARAM wParam, LPARAM lParam)
{
	switch ( (int)wParam )
	{
		case 0:
			break;

		case 1:
			break;

		case 2:
			break;

		case 3:
			break;

		case 4:
			break;

		default:
			break;
	}

	return 1;
}

/**
 *	ログ関係
 */
LRESULT CSerialPortDialog::OnStartLogging(WPARAM wParam, LPARAM lParam)
{
	/// ログ用ファイルオープン
	pSendLogWriter->openFile(TEXT("test.txt"), File::WRITE_ONLY);
	isLogging = true;

	return 1;
}

LRESULT CSerialPortDialog::OnStopLogging(WPARAM wParam, LPARAM lParam)
{
	/// ログ用ファイルを閉じる
	pSendLogWriter->closeFile();
	isLogging = false;

	return 1;
}


/**
 *	------------------------------------------------------------
 *		オーバーライド関数
 *	------------------------------------------------------------
 */
BOOL CSerialPortDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	return CDialog::Create(IDD, pParentWindow);

	/*
	return CDialog::Create(lpszTemplateName, pParentWnd);
	*/
}

BOOL CSerialPortDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

/**
 *	ダイアログのスタイル
 */
	/// タイトルの設定
	SetWindowText( TEXT("Serial Port Log") );
	/// Viewスタイルの設定
	setDialogStyle(GREEN_THEMED);

/**
 *	オブジェクトの作成
 */
	/// 通信ポートの作成
	pCommPort = new SerialPort();
	/// ログオブジェクト作成
	pSendLogWriter = new FileWriter();


/**
 *	フラグの初期化
 */
	/// ログフラグ
	isLogging = false;
	/// ブラシ
	isBrushCreated = false;
	/// 表示データ
	isSendingDataDisplayed = true;
	isRecievingDataDisplayed = true;

/**
 *	表示用の変数
 */
	sciPacketType = Comm::LONG;

	/// Windowsのタイマセット（スクリーン更新用タイマ）
	timerID = 1;
	SetTimer(timerID, 100, NULL);	

/**
 *	シリアルポート関連
 */
	/// シリアルポートのオープン
	if ( IsWindow(this->m_hWnd) )
		pCommPort->openSerialPort(this->m_hWnd, 5, 38400);//38400//115200		//SH2のCOMポート

	/// オープンと同時にリスナースレッド開始
	pCommPort->startListening();
	
	/// システムの待機
	Sleep(200);
	
	/// リスニングの開始はユーザーに任せるためスレッドを停止
	pCommPort->stopListening();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CSerialPortDialog::DestroyWindow()
{
	return TRUE;
}

void CSerialPortDialog::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	DeleteObject(backColorBrush);
	DeleteObject(editBkColorBrush);
	DeleteObject(staticBkColorBrush);

	pCommPort->closeSerialPort();

	delete pSendLogWriter;

	delete pCommPort;

	delete this;

	CDialog::PostNcDestroy();
}

void CSerialPortDialog::OnOK()
{
	Default();
}

void CSerialPortDialog::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	Default();

	//CDialog::OnCancel();
}

/**
 *	------------------------------------------------------------
 *		自作関数
 *	------------------------------------------------------------
 */

/**
 *	説明
 *		メッセージを送るスレッドIDの設定
 *	引数
 *		id: スレッドID
 */
void CSerialPortDialog::setMotionPlanThreadID(DWORD id)
{
	motionPlanThreadID = id;
	return;
}

/**
 *	説明
 *		ダイアログの背景などを設定
 *	引数
 *		style: 背景のスタイル
 */
void CSerialPortDialog::setDialogStyle(ViewStyle style)
{
	/// スタイルの決定
	viewStyle = style;

	/// 既に作成されていたらブラシ破棄
	if ( isBrushCreated )
	{
		DeleteObject(backColorBrush);
		DeleteObject(editBkColorBrush);
		DeleteObject(staticBkColorBrush);
	}
	
	switch ( viewStyle )
	{
		case GREEN_THEMED:
		{
			/// 色決定
			editColor = TXT_BLUE_COLOR_LIGHT;
			staticColor = Color::YELLOW;
			editBkColor = EDIT_BACK_GREEN_COLOR;
			staticBkColor = DLG_BACK_GREEN_COLOR /* EDIT_BACK_GREEN_COLOR */;

			///ブラシ作成
			backColorBrush		= CreateSolidBrush( DLG_BACK_GREEN_COLOR );
			editBkColorBrush	= CreateSolidBrush( EDIT_BACK_GREEN_COLOR );
			staticBkColorBrush	= CreateSolidBrush( DLG_BACK_GREEN_COLOR /* EDIT_BACK_GREEN_COLOR */ );

			break;
		}
		case BLUE_THEMED:
		{
			/// 色決定
			editColor = TXT_BLUE_COLOR_LIGHT;
			staticColor = TXT_YELLOW_COLOR_LIGHT;
			editBkColor = EDIT_BACK_BLUE_COLOR;
			staticBkColor = EDIT_BACK_BLUE_COLOR;

			///ブラシ作成
			backColorBrush		= CreateSolidBrush( DLG_BACK_BLUE_COLOR );
			editBkColorBrush	= CreateSolidBrush( EDIT_BACK_BLUE_COLOR );
			staticBkColorBrush	= CreateSolidBrush( EDIT_BACK_BLUE_COLOR );

			break;
		}
		default:
			break;
	}

	/// ブラシ作成完了
	isBrushCreated = true;

	return;
}

/**
 *	説明
 *		シリアル通信用パケットデータの作成
 *	引数
 *		type: パケットタイプ
 *		packet: 送信バッファ
 *		adress: 送信先関節番号（単関節命令の場合のみ）
 *		command: 指令種類
 */
bool CSerialPortDialog::buildPacket(SciPacketType type, char* packet, int address, int command ) //unsigned char* packet
{
	/// カウンタ
	int i,j;

	/// 通信フォーマットで場合分け
	switch (type)
	{
		/// 1関節ごとに指令値を送る場合<---干渉駆動
		case Comm::SHORT:
		{
			/// データ部分
			int data;

			/// 指令値で場合分け
			switch (command)
			{
				/// 角度指令
				case PACKET_CMD_JNT_POS:
				{
					/// 脚関節データ取得
					/// 単位をDegに修正し、100倍して有効数字5桁の正の整数にする（16bit）
					data = (int)( ( commAsuraData.getLegJointAngle((int)((address-1) / 3) + 1)(((address-1) % 3) + 1)*RAD2DEG +180.0 )*100.0 );	/// <-オフセットして正値に
					//data =(int)(commAsuraData.getLegActuatorPosition(1)*100);

					/// バッファに一時保存 
					//packet[0] = SCI_PACKET_HEADER;
					//packet[1] = SCI_PACKET_SIZE_SHORT;
					//packet[2] = (unsigned char)address;
					//packet[3] = (unsigned char)command;
					//packet[4] = (unsigned char)((data & 0xFF00) >> 8);
					//packet[5] = (unsigned char)(data & 0x00FF);
					//packet[6] = (unsigned char)((packet[4] + packet[5]) & 0x00FF);
				}
					break;

				/// 速度司令
				case PACKET_CMD_JNT_SPD:
				{
				}
					break;

				default:
					break;
			}
		}
		break;

		/// 全関節に指令値を送る場合
		case Comm::LONG:
		{
			

/*			/// パケットデータに代入
			/// ヘッダ
			packet[0]	= SCI_PACKET_HEADER;
			packet[1]	= SCI_PACKET_HEADER;
			packet[2]	= SCI_PACKET_SIZE_LONG;

				packet[3]	= PACKET_LEG_ALL_JOINTS;

				/// コマンド内容
				packet[4]	= changeflag;//PAKCET_CMD_WALK;

				/// ヘッダチェックサム
				for (i=0;i<(SCI_PACKET_HEADER_SIZE-1);i++)
				{
					headerCheckSum += packet[i];
				}
				packet[5] = headerCheckSum;

				for (i=0;i<SCI_MAX_JOINT_NUM;i++)
				{
					/// 脚関節データ取得
					/// -180～180だが180度加算して、オフセットして0～360に変換し正値に
					///角速度は移動速度を-300～300[mm/s]とし，その後単位を[deg/s]にする　そして+300することでオフセットを行う
					if(i==2 || i==5 || i==8 || i==11){
						if(changeflag & BIT((i-2)/3))
						{
								data = (int)((commAsuraData.getTrackSpeed((int)(i/3)+1)* RAD2DEG/WHEELRADIUS+300) * 100.0);//角速度
					//		if(i==5)
						cerr<<"ANvel  " <<data<<"   vel  " <<commAsuraData.getTrackSpeed((int)(i/3)+1)<<endl;
			
						}
						else
						{data = (int)((commAsuraData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);
							//if(i==5)
						cerr<<i<<" anggg =" <<data<<"   anggg  " <<commAsuraData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1)<<endl;
						}//角度
					}
					else
					{
					data = (int)((commAsuraData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);//角度
					if(i==1)
						cerr<<"2angll ="<<data<<endl;
					}
					dataHigh	= (unsigned char)((data & 0xFF00) >> 8);
					dataLow		= (unsigned char)(data & 0x00FF);
					sum = sum + dataHigh + dataLow;
						
					/// 脚のパケットデータ
					packet[2*(i + 3)]		= dataHigh;
					packet[2*(i + 3) + 1]	= dataLow;
				}
				
				/// チェックサム
				packet[30]	= (unsigned char)((sum & 0x7F00) >> 8);
				packet[31]	= (unsigned char)(sum & 0x00FF);
			
			
		}
		break;

		case Comm::DEBUGGING:
		{
		}
		break;
	default:
		break;
*/	unsigned char headerCheckSum = 0;
			unsigned __int32 data = 0;
			int dataHigh1	= 0;
			int dataHigh2	= 0;
			int dataLow1	= 0;
			int dataLow2	= 0;
			int sum			= 0;

			unsigned __int16  CANID			= 0;
			unsigned __int16  TopID			=	0;
			unsigned __int16  UnderID		=	0;
			unsigned __int32  Top1			=	0;
			unsigned __int32  Top2			=	0;
			unsigned __int32  Under1		=	0;
			unsigned __int32  Under2		=	0;
			int address		= 0;//アクチュエーター番号 1-24
			unsigned char CommandType= 0;
			int Act_Memo	= 0;
			
			for(i=0;i<LEG_NUM;i++){//for(i=0;i<1;i++){//

				for (j=0;j<LEG_ACT_NUM;j++){//for (j=0;j<1;j++){//

					
					switch (j)
					{
						CANID		=	0;
						CommandType	= 0;
						///脚マイコン1
						case 0:{
							CANID		= (i+1)*100 + 10;//controller_2_1;//
							CommandType	= com_ballscllew_1_pos;///ボールねじ1
							
								}
							break;
						case 1:{
							CANID		= (i+1)*100 + 10;//controller_2_1;//
							CommandType	= com_ballscllew_2_pos;///ボールねじ2
							  
							   }							
							break;
							
						///脚マイコン2
						case 2:{
							CANID		= (i+1)*100 + 20;//controller_2_2;//
							CommandType	= com_ballscllew_1_pos;///ボールねじ1
							   }
							break;
						case 3:{
							CANID		= (i+1)*100 + 20;//controller_2_2;//
							CommandType	= com_ballscllew_2_pos;///ボールねじ2
							   }
							break;
							
						default:
							break;				
					}
					TopID	=	CANID & 0x0000ff00;
					UnderID	=	CANID & 0x000000ff;
					TopID	=	TopID >>8;
					///アクチュエーター指令値 = ボールねじ座標 * 100
					//data = (int)( ( commAsuraData.getLegJointAngle(1)(1)*RAD2DEG +180.0 )*100.0 );
					//data =(int)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);
					//data =123*100; 
					//data =(unsigned __int32)(-123*100);
					data =(unsigned __int32)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);

					///マスクとキャストを同時に実行すると指令値エラーなので分ける
					Top1	=	data & 0xff000000;
					Top2	=	data & 0x00ff0000;
					Under1	=	data & 0x0000ff00;
					Under2	=	data & 0x000000ff;
					Top1	=	Top1 	>> 24;
					Top2	=	Top2 	>> 16;
					Under1	=	Under1 	>> 8;

					/// アクチュエータへのパケットデータ
					packet[7*Act_Memo]		= (unsigned char)TopID;//((CANID & 0x0000FF00 ) >> 8);
					packet[7*Act_Memo+1]	= (unsigned char)UnderID;//( CANID & 0x000000FF );
					packet[7*Act_Memo+2]	= (unsigned char)(CommandType);
					packet[7*Act_Memo+3]	= (unsigned char)Top1;//((data & 0xFF000000 ) >> 24);
					packet[7*Act_Memo+4]	= (unsigned char)Top2;//((data & 0x00FF0000 ) >> 12);
					packet[7*Act_Memo+5]	= (unsigned char)Under1;//((data & 0x0000FF00 ) >> 8);
					packet[7*Act_Memo+6]	= (unsigned char)Under2;//( data & 0x000000FF );
					Act_Memo++;
				}//LEG_ACT_NUM
			}///LEG_NUM

/*				for (j=0;j<LEG_ACT_NUM;j++){//for (j=0;j<1;j++){//
					i=0;
					CANID		=	0;
						CommandType	= 0;
					//switch (j)
					//{
						
						///脚マイコン1
						if(j==0){
							CANID		= controller_1_1;//(i+1)*100 + 10;//
							CommandType	= com_ballscllew_1_pos;///ボールねじ1
							
								}

						else if(j==1){
							CANID		= controller_1_1;//(i+1)*100 + 10;//
							CommandType	= com_ballscllew_2_pos;///ボールねじ2							  
							   }							
						///脚マイコン2
						else if(j==2){
							CANID		= controller_1_2;//(i+1)*100 + 20;//
							CommandType	= com_ballscllew_1_pos;///ボールねじ1
							   }
						else if(j==3){
							CANID		= controller_1_2;//(i+1)*100 + 20;//
							CommandType	= com_ballscllew_2_pos;///ボールねじ2
							   }							
						//default:
							//break;				
					//}
					TopID	=	CANID & 0x0000ff00;
					UnderID	=	CANID & 0x000000ff;
					TopID	=	TopID >>8;
					///アクチュエーター指令値 = ボールねじ座標 * 100
					//data = (int)( ( commAsuraData.getLegJointAngle(1)(1)*RAD2DEG +180.0 )*100.0 );
					//data =(int)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);
					//data =123*100; 
					//data =(unsigned __int32)(-123*100);
					data =(unsigned __int32)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);

					///マスクとキャストを同時に実行すると指令値エラーなので分ける
					Top1	=	data & 0xff000000;
					Top2	=	data & 0x00ff0000;
					Under1	=	data & 0x0000ff00;
					Under2	=	data & 0x000000ff;
					Top1	=	Top1 	>> 24;
					Top2	=	Top2 	>> 16;
					Under1	=	Under1 	>> 8;

					/// アクチュエータへのパケットデータ
					packet[7*Act_Memo]		= ( char)TopID;//((CANID & 0x0000FF00 ) >> 8);
					packet[7*Act_Memo+1]	= ( char)UnderID;//( CANID & 0x000000FF );
					packet[7*Act_Memo+2]	= ( char)(CommandType);
					packet[7*Act_Memo+3]	= ( char)Top1;//((data & 0xFF000000 ) >> 24);
					packet[7*Act_Memo+4]	= ( char)Top2;//((data & 0x00FF0000 ) >> 12);
					packet[7*Act_Memo+5]	= ( char)Under1;//((data & 0x0000FF00 ) >> 8);
					packet[7*Act_Memo+6]	= ( char)Under2;//( data & 0x000000FF );
					Act_Memo++;
				}//LEG_ACT_NUM
*/

			///最後にゲートウェイマイコン

			CANID		= controller_0_0;
			CommandType	= 255;//START COMMAND
			data		= 255;//START COMMAND Checker

			///マスクとキャストを同時に実行すると指令値エラーなので分ける
			Top1	=	data & 0xff000000;
			Top2	=	data & 0x00ff0000;
			Under1	=	data & 0x0000ff00;
			Under2	=	data & 0x000000ff;
			Top1	=	Top1 	>> 24;
			Top2	=	Top2 	>> 16;
			Under1	=	Under1 	>> 8;

			//Act_Memo++;
			packet[7*Act_Memo]		= (char)((CANID & 0x0000FF00 ) >> 8);;
			packet[7*Act_Memo+1]	= (char)( CANID & 0x000000FF );
			packet[7*Act_Memo+2]	= (char)(CommandType);
			packet[7*Act_Memo+3]	= (char)((data & 0xFF000000 ) >> 24);
			packet[7*Act_Memo+4]	= (char)((data & 0x00FF0000 ) >> 12);
			packet[7*Act_Memo+5]	= (char)((data & 0x0000FF00 ) >> 8);
			packet[7*Act_Memo+6]	= ( char)( data & 0x000000FF );


	}	/// end of switch (packetFormat)

	}
	return true;
}

/**
 *	ログ表示のための関数
 */
void CSerialPortDialog::displayLogData(UINT itemID, LPCTSTR lpFmt, ...)
{
	/**
	 *		ローカル変数
	 */
	/// 可変引数
	va_list arglist;
	/// バッファ
	TCHAR buf[MAX_BUFFER_SIZE];
	/// CEditへのポインタ
	CEdit* pEdit;
	/// 計算用の変数
	int i,j,k,l,m;

	/// 可変個数の引数並びにアクセス
	va_start(arglist, lpFmt);

	/// 文字列をフォーマット
	wvsprintf(buf, lpFmt, arglist);

	/// 可変パラメータ編集終了
    va_end(arglist);

	/// ログのコントロールを取得
	pEdit = (CEdit*)GetDlgItem(itemID);

	/// ログの最大文字数を取得
	j = pEdit->GetLimitText();
	/// 現在のログの長さを取得
	i = pEdit->GetWindowTextLength();
	
	if ( i + ( k = lstrlen(buf) ) > j )
	{
		/// 新しい文字列がログに追加できない時
		/// 古いログを削除

		/// 再描画フラグをクリア
		pEdit->SetRedraw(FALSE);

		/// 削除しなくてはならないバイト数を求める
		if ( ( k = k - (j-i) ) < 0 ) 
			k = 0;
		
		/// 削除しなくてはならないバイト数を含む行数を求める
		m = pEdit->LineFromChar( k );

		//その行のインデックスを求める
		l = pEdit->LineIndex(m);
		
		if (l < k) 
			l = pEdit->LineIndex( m+1 );

		/// その行の先頭では削除しなくてはならないバイト数に満たない時
		/// 次の行のインデックスを求める
		
		/// 削除する範囲を選択
		pEdit->SetSel(0, l);
		/// 選択した範囲を削除
		pEdit->ReplaceSel( TEXT("") );

		/// 現在のログの長さを取得
		i = pEdit->GetWindowTextLength();
		/// 再描画フラグをセット
		pEdit->SetRedraw();
	}

	/// ログの最後を選択
	pEdit->SetSel(i, i);
	/// 文字列を追加
	pEdit->ReplaceSel( buf );

	return;
}


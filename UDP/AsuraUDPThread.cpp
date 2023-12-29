// AsuraUDPTherad.cpp : 実装ファイル
//

#include "stdafx.h"
#include "AsuraUDPThread.h"
#include "..\Utility\Constants.h"
#include "UDPPacket.h"
#include <winsock2.h>
#include <WS2tcpip.h>


#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")


using namespace std;
using namespace Const;
using namespace Asura;
using namespace Data;
using namespace Plan;
using namespace System;

// AsuraUDPTherad

IMPLEMENT_DYNCREATE(AsuraUDPThread, CWinThread)


BEGIN_MESSAGE_MAP(AsuraUDPThread, CWinThread)

	///カスタムメッセージ
	ON_MESSAGE(WM_DLG_UDPTHREAD_END, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnEndThread)
	ON_MESSAGE(WM_DLG_COMM_START_LISTEN, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStartPortListening)
	ON_MESSAGE(WM_DLG_COMM_STOP_LISTEN, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStopPortListening)
	ON_MESSAGE(WM_PLAN_SENDING_COMM_DATA, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnSendCommandData)

	/* ログ 未実装20210622
	ON_MESSAGE(WM_DLG_COMM_START_LOGGING, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStartLogging)
	ON_MESSAGE(WM_DLG_COMM_STOP_LOGGING, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStopLogging)
	*/

END_MESSAGE_MAP()

AsuraUDPThread::AsuraUDPThread()
	:isAlive(),isCommAlive(),addr(),client_addr(),client_IPAdress()
	,longPacket(),onelegPacket(),pMultiMediaTimer(),pTimedUDPProcedure()
	,port(),sciPacketType(),server_IPAdress(),server_addr(),shortPacket()
{ 
	///生存フラグの初期化
	isAlive = FALSE;

	//通信フラグ
	isCommAlive = FALSE;

	//sock initialize
	sock=INVALID_SOCKET;

}

AsuraUDPThread::~AsuraUDPThread()
{
	//生存フラグの終了処理
	isAlive = FALSE;
}

BOOL AsuraUDPThread::InitInstance()
{
	// TODO:  スレッドごとの初期化をここで実行します。

	/// スレッドをアクティブに
	isAlive = TRUE;



	return TRUE;
}

int AsuraUDPThread::ExitInstance()
{
	// TODO:  スレッドごとの後処理をここで実行します。
	 
	

	finalizeAsuraUDPThread();
	return CWinThread::ExitInstance();
}

// ASuraUDPThread メッセージ ハンドラ
/**
 *	------------------------------------------------------------
 *		生成された、メッセージ割り当て関数
 *		MotionPlanThread メッセージハンドラ 関数
 *	------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	Doc，Viewクラス関連
  *	----------------------------------------
  */
 /**
  *	説明
  *		スレッドの終了処理
 */
void AsuraUDPThread::OnEndThread(WPARAM wParam, LPARAM lParam)
{
	/// スレッド終了処理
	ExitInstance();

	/// システムに終了を要求(WM_DESTORYに対応)
	PostQuitMessage(0);
}


/**
 *	----------------------------------------
 *	通信関連
 *	----------------------------------------
 */
///通信開始
void AsuraUDPThread::OnStartPortListening(WPARAM wParam, LPARAM lParam)
{
	//ローカル変数
	int n=1;

	/*
		sock open
	*/
	// initalize winsock2
	WSADATA wsaData;
	n=WSAStartup(MAKEWORD(2, 0), &wsaData); ///成功したら戻り値0
	

	if (n == 0)
	{
		

		//socket opening
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		
		
		//server configulation
		
		memset(&server_addr,0,sizeof server_addr);
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(c_portNo);
		inet_pton(AF_INET,"192.168.0.169", &server_addr.sin_addr.S_un.S_addr);
		//inet_pton(AF_INET, client_IPAdress, &server_addr.sin_addr.S_un.S_addr);


		isCommAlive = true;
	}
	else
	{
		isCommAlive = false;
	}
}

///通信停止
void AsuraUDPThread::OnStopPortListening(WPARAM wParam, LPARAM lParam)
{
	isCommAlive = false;
	
	/*
		sock close
	*/
	//socket closing
	closesocket(sock);
	// finalize winsock2
	WSACleanup();

	/// スレッド終了処理
	ExitInstance();

	/// システムに終了を要求(WM_DESTORYに対応)
	PostQuitMessage(0);
}

void AsuraUDPThread::OnSendCommandData(WPARAM wParam, LPARAM lParam)
{
	//get command 
	if ((AsuraData*)wParam != NULL)
		viewAsuraXData = *((AsuraData*)wParam);
	if ((PlanData*)lParam != NULL)
		viewPlanData = *((PlanData*)lParam);

	//送信バッファへの書き込み

	/**
	 *		ローカル変数
	 */
	int i;
	sciPacketType = SciPacketType::LONG;

	switch (sciPacketType)
	{
	case SciPacketType::SHORT:
	{
		/// パケット送信
		for (i = 0;i < SCI_MAX_JOINT_NUM;i++)
		{
			/// パケットデータ作成
			ZeroMemory(shortPacket[i], SCI_PACKET_SIZE_SHORT);
			//buildPacket(SciPacketType::SHORT, shortPacket[i], i+1, PACKET_CMD_JNT_POS);
			//pCommPort->sendData((&shortPacket[i][0]), (DWORD)SCI_PACKET_SIZE_SHORT);
		}
	}
	break;

	case  SciPacketType::LONG:
	{
		/// パケット送信
		SecureZeroMemory(&longPacket, sizeof(longPacket));
		//if(getLocomotionStyle()==LEGGED)//問題
		buildPacket(SciPacketType::LONG, SCI_ALL_JOINTS, PAKCET_CMD_WALK);
		//パケット送信
		sendto(sock, longPacket, sizeof(longPacket), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
		
		
	}
	break;

	case  SciPacketType::DEBUGGING:
	{
		return;
	}
	break;
	}



	return;

}




//開始処理
void AsuraUDPThread::initializeAsuraUDPThread(void)
{
	/*
	*
	* バッファ領域の確保
	*
	*/


	if (writeBuffer != NULL) delete[] writeBuffer;
	if (writeBufferSize_ > MAX_BUFFER_SIZE) writeBufferSize_ = MAX_BUFFER_SIZE;
	writeBuffer = new unsigned char[writeBufferSize_];

	/// バッファサイズの設定
	writeBufferSize = writeBufferSize_;

	/// バッファの初期化
	ZeroMemory(writeBuffer, writeBufferSize);



	/**
	 *	説明
	 *		タイマコールバック作成
	 */
	 /// オブジェクト作成
	pTimedUDPProcedure = new System::TimedMotionProcedure();
	/// タイマコールバックに本スレッドのIDを登録
	pTimedUDPProcedure->setThreadID(m_nThreadID);	//// m_nThreadは基底クラスのCWinThreadのメンバ

/**
 *	タイマ本体作成
 */
	pMultiMediaTimer = new System::MultiMediaTimer(*pTimedUDPProcedure);
	pMultiMediaTimer->setTimer(20, 5);

	isAlive = TRUE;
}


//終了処理
void AsuraUDPThread::finalizeAsuraUDPThread(void)
{
	/**
	 *	タイマ終了処理
	 */
	 /// タイマが停止していなかったら
	if (pMultiMediaTimer != NULL)
	{
		pMultiMediaTimer->killTimer();

		/// タイマオブジェクト破棄
		delete pMultiMediaTimer;
		pMultiMediaTimer = NULL;
	}

	if (pTimedUDPProcedure != NULL)
	{
		/// タイマコールバック破棄
		delete pTimedUDPProcedure;
		pTimedUDPProcedure = NULL;
	}

	return;

}

//　パケットデータ作成
char AsuraUDPThread::buildPacket(SciPacketType type, int address, int command)
{
	/// カウンタ
	int i, j;
	

	/// 通信フォーマットで場合分け
	switch (type)
	{
		/// 1関節ごとに指令値を送る場合<---干渉駆動
	case SciPacketType::SHORT:
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
			data = (int)((viewAsuraXData.getLegJointAngle((int)((address - 1) / 3) + 1)(((address - 1) % 3) + 1) * RAD2DEG + 180.0) * 100.0);	/// <-オフセットして正値に
			//data =(int)(commTitanData.getLegActuatorPosition(1)*100);

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
	case SciPacketType::LONG:
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
										data = (int)((commTitanData.getTrackSpeed((int)(i/3)+1)* RAD2DEG/WHEELRADIUS+300) * 100.0);//角速度
							//		if(i==5)
								cerr<<"ANvel  " <<data<<"   vel  " <<commTitanData.getTrackSpeed((int)(i/3)+1)<<endl;

								}
								else
								{data = (int)((commTitanData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);
									//if(i==5)
								cerr<<i<<" anggg =" <<data<<"   anggg  " <<commTitanData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1)<<endl;
								}//角度
							}
							else
							{
							data = (int)((commTitanData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);//角度
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
		*/
	
		unsigned __int32 data = 0;
		double Actpos=0;
		unsigned char symbol=0;
		
		unsigned __int16  CANID = 0;
		unsigned __int16  TopID = 0;
		unsigned __int16  UnderID = 0;
		unsigned __int16  Data1 = 0;
		unsigned __int16  Data2 = 0;
		int address = 0;//アクチュエーター番号 1-24
		unsigned char CommandType = 0;
		unsigned char Checksum=0;
		unsigned short Cdata1=0;
		unsigned short Cdata2 = 0;
		int Act_Memo = 0;
		unsigned __int16 Pinit = 127;

		for (i = 1;i < (LEG_NUM-4);i++) {//for(i=0;i<1;i++){//

			for (j = 0;j < LEG_ACT_NUM;j++) {//for (j=0;j<1;j++){//


				switch (j)
				{
					CANID = 0;
					CommandType = 0;
					///脚マイコン1
				case 0: {
					CANID = (i + 1) * 100 + 10;//controller__1;//
					CommandType = com_ballscllew_1_pos;///ボールねじ1

				}
					  break;
				case 1: {
					CANID = (i + 1) * 100 + 10;//controller_2_1;//
					CommandType = com_ballscllew_2_pos;///ボールねじ2

				}
					  break;

					  ///脚マイコン2
				case 2: {
					CANID = (i + 1) * 100 + 20;//controller_2_2;//
					CommandType = com_ballscllew_1_pos;///ボールねじ1
				}
					  break;
				case 3: {
					CANID = (i + 1) * 100 + 20;//controller_2_2;//
					CommandType = com_ballscllew_2_pos;///ボールねじ2
				}
					  break;

				default:
					break;
				}
				TopID = CANID & 0x0000ff00;
				UnderID = CANID & 0x000000ff;
				TopID = TopID >> 8;
				///アクチュエーター指令値 = ボールねじ座標 * 100
				//data = (int)( (viewAsuraXData.getLegJointAngle(1)(1)*RAD2DEG +180.0 )*100.0 );
				//data =(int)(commTitanData.getLegActuatorPosition(i+1)(j+1)*100.0);
				//data =123*100; 
				//data =(unsigned __int32)(-123*100);
			
				Actpos = 100*viewAsuraXData.getLegActuatorPosition(i+1)(j + 1);
				
				if (Actpos >= 0)
				{
					data = (unsigned int)Actpos;
					symbol = 0x00;
				}
				else
				{
					Actpos = (-1) * Actpos;		//符号反転
					data = (unsigned int)Actpos;
					symbol = 0xff;
				}
				
							

				///マスクとキャストを同時に実行すると指令値エラーなので分ける
				Data1 = data & 0xff00;
				Data2= data & 0x00ff;
				Data1 = Data1 >> 8;

				//パケットチェックサム
				Cdata1 = Data1&0x00f0;
				Cdata2 = Data2&0x000f;
				Checksum = (unsigned char)(Cdata1 + Cdata2);


				/// アクチュエータへのパケットデータ
				longPacket[8 * Act_Memo]	 = (unsigned char)Pinit;
				longPacket[8 * Act_Memo	+ 1] = (unsigned char)TopID;//((CANID & 0x0000FF00 ) >> 8);
				longPacket[8 * Act_Memo + 2] = (unsigned char)UnderID;//( CANID & 0x000000FF );
				longPacket[8 * Act_Memo + 3] = (unsigned char)CommandType;
				longPacket[8 * Act_Memo + 4] = (unsigned char)symbol;
				longPacket[8 * Act_Memo + 5] = (unsigned char)Data1;
				longPacket[8 * Act_Memo + 6] = (unsigned char)Data2;
				longPacket[8 * Act_Memo + 7] = Checksum;

					
				
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
					//data = (int)( ( commTitanData.getLegJointAngle(1)(1)*RAD2DEG +180.0 )*100.0 );
					//data =(int)(commTitanData.getLegActuatorPosition(i+1)(j+1)*100.0);
					//data =123*100;
					//data =(unsigned __int32)(-123*100);
					data =(unsigned __int32)(commTitanData.getLegActuatorPosition(i+1)(j+1)*100.0);

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
		/*
		CANID = controller_0_0;
		CommandType = 255;//START COMMAND
		data = 255;//START COMMAND Checker

		///マスクとキャストを同時に実行すると指令値エラーなので分ける
		Top1 = data & 0xff000000;
		Top2 = data & 0x00ff0000;
		Under1 = data & 0x0000ff00;
		Under2 = data & 0x000000ff;
		Top1 = Top1 >> 24;
		Top2 = Top2 >> 16;
		Under1 = Under1 >> 8;

		//Act_Memo++;
		packet[7 * Act_Memo] = (char)((CANID & 0x0000FF00) >> 8);;
		packet[7 * Act_Memo + 1] = (char)(CANID & 0x000000FF);
		packet[7 * Act_Memo + 2] = (char)(CommandType);
		packet[7 * Act_Memo + 3] = (char)((data & 0xFF000000) >> 24);
		packet[7 * Act_Memo + 4] = (char)((data & 0x00FF0000) >> 12);
		packet[7 * Act_Memo + 5] = (char)((data & 0x0000FF00) >> 8);
		packet[7 * Act_Memo + 6] = (char)(data & 0x000000FF);
		*/

	}	/// end of switch (packetFormat)

	}
	

	return 0;
}

// AsuraUDPTherad メッセージ ハンドラー

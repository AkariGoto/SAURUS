
#include "pch.h"

#include <winsock2.h>
#include <WS2tcpip.h>

#include "UDP/asura_udp_thread.h"
#include "UDP/udp_packet.h"


#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")


using namespace Const;
using namespace System;

namespace designlab_robot_gui::udp
{

IMPLEMENT_DYNCREATE(AsuraUdpThread, CWinThread)


BEGIN_MESSAGE_MAP(AsuraUdpThread, CWinThread)

    // カスタムメッセージ
    ON_MESSAGE(WM_DLG_UDPTHREAD_END,
               (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnEndThread)

    ON_MESSAGE(WM_DLG_COMM_START_LISTEN,
               (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStartPortListening)

    ON_MESSAGE(WM_DLG_COMM_STOP_LISTEN,
               (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStopPortListening)

    ON_MESSAGE(WM_PLAN_SENDING_COMM_DATA,
               (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnSendCommandData)

END_MESSAGE_MAP()

AsuraUdpThread::AsuraUdpThread() :
    isAlive(),
    isCommAlive(),
    addr(),
    client_addr(),
    client_ip_address(),
    longPacket(),
    one_leg_packet(),
    multi_media_timer_ptr(),
    timed_udp_procedure_ptr(),
    port(),
    sciPacketType(),
    server_ip_address(),
    server_addr(),
    shortPacket()
{
    ///生存フラグの初期化
    isAlive = FALSE;

    //通信フラグ
    isCommAlive = FALSE;

    //sock initialize
    sock = INVALID_SOCKET;

}

AsuraUdpThread::~AsuraUdpThread()
{
    //生存フラグの終了処理
    isAlive = FALSE;
}

BOOL AsuraUdpThread::InitInstance()
{
    // TODO:  スレッドごとの初期化をここで実行します。

    /// スレッドをアクティブに
    isAlive = TRUE;



    return TRUE;
}

int AsuraUdpThread::ExitInstance()
{
    // TODO:  スレッドごとの後処理をここで実行します。



    finalizeAsuraUDPThread();
    return CWinThread::ExitInstance();
}

void AsuraUdpThread::OnEndThread(WPARAM wParam, LPARAM lParam)
{
    /// スレッド終了処理
    ExitInstance();

    /// システムに終了を要求(WM_DESTORYに対応)
    PostQuitMessage(0);
}

void AsuraUdpThread::OnStartPortListening(WPARAM wParam, LPARAM lParam)
{
    //ローカル変数
    int n = 1;

    // sock open
    // initialize winsock2

    WSADATA wsaData;
    n = WSAStartup(MAKEWORD(2, 0), &wsaData);  // 成功したら戻り値 0．


    if (n == 0)
    {
        //socket opening
        sock = socket(AF_INET, SOCK_DGRAM, 0);

        //server configuration

        memset(&server_addr, 0, sizeof server_addr);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(c_portNo);
        inet_pton(AF_INET, "192.168.0.169", &server_addr.sin_addr.S_un.S_addr);
        //inet_pton(AF_INET, client_ip_address, &server_addr.sin_addr.S_un.S_addr);

        isCommAlive = true;
    }
    else
    {
        isCommAlive = false;
    }
}

void AsuraUdpThread::OnStopPortListening(WPARAM wParam, LPARAM lParam)
{
    isCommAlive = false;

    //  sock close

    // socket closing
    closesocket(sock);

    // finalize winsock2
    WSACleanup();

    // スレッド終了処理
    ExitInstance();

    // システムに終了を要求(WM_DESTORYに対応)
    PostQuitMessage(0);
}

void AsuraUdpThread::OnSendCommandData(WPARAM wParam, LPARAM lParam)
{
    //get command 
    if ((AsuraData*)wParam != NULL)
    {
        viewAsuraXData = *((AsuraData*)wParam);
    }

    if ((PlanData*)lParam != NULL)
    {
        viewPlanData = *((PlanData*)lParam);
    }


    //送信バッファへの書き込み

    int i;
    sciPacketType = SciPacketType::LONG;

    switch (sciPacketType)
    {
        case SciPacketType::SHORT:
        {
            /// パケット送信
            for (i = 0; i < SCI_MAX_JOINT_NUM; i++)
            {
                /// パケットデータ作成
                ZeroMemory(shortPacket[i], SCI_PACKET_SIZE_SHORT);
            }

            break;
        }
        case  SciPacketType::LONG:
        {
            /// パケット送信
            SecureZeroMemory(&longPacket, sizeof(longPacket));
            //if(getLocomotionStyle()==LEGGED)//問題
            buildPacket(SciPacketType::LONG, SCI_ALL_JOINTS, PAKCET_CMD_WALK);
            //パケット送信
            sendto(sock, longPacket, sizeof(longPacket), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

            break;
        }
        case  SciPacketType::DEBUGGING:
        {
            return;
        }
        break;
    }

    return;
}




//開始処理
void AsuraUdpThread::initializeAsuraUDPThread()
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
    timed_udp_procedure_ptr = new system::TimedMotionProcedure();
    /// タイマコールバックに本スレッドのIDを登録
    timed_udp_procedure_ptr->SetThreadID(m_nThreadID);	//// m_nThreadは基底クラスのCWinThreadのメンバ

    /**
     *	タイマ本体作成
     */
    multi_media_timer_ptr = new system::MultiMediaTimer(*timed_udp_procedure_ptr);
    multi_media_timer_ptr->setTimer(20, 5);

    isAlive = TRUE;
}


//終了処理
void AsuraUdpThread::finalizeAsuraUDPThread(void)
{
    /**
     *	タイマ終了処理
     */
     /// タイマが停止していなかったら
    if (multi_media_timer_ptr != NULL)
    {
        multi_media_timer_ptr->killTimer();

        /// タイマオブジェクト破棄
        delete multi_media_timer_ptr;
        multi_media_timer_ptr = NULL;
    }

    if (timed_udp_procedure_ptr != NULL)
    {
        /// タイマコールバック破棄
        delete timed_udp_procedure_ptr;
        timed_udp_procedure_ptr = NULL;
    }

    return;

}

//　パケットデータ作成
char AsuraUdpThread::buildPacket(SciPacketType type, int address, int command)
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
                    data = (int)((viewAsuraXData.leg_joint_angle[(int)((address - 1) / 3)](((address - 1) % 3) + 1) * RAD2DEG + 180.0) * 100.0);	/// <-オフセットして正値に
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
            unsigned __int32 data = 0;
            double Actpos = 0;
            unsigned char symbol = 0;

            unsigned __int16  CANID = 0;
            unsigned __int16  TopID = 0;
            unsigned __int16  UnderID = 0;
            unsigned __int16  Data1 = 0;
            unsigned __int16  Data2 = 0;
            int address = 0;//アクチュエーター番号 1-24
            unsigned char CommandType = 0;
            unsigned char Checksum = 0;
            unsigned short Cdata1 = 0;
            unsigned short Cdata2 = 0;
            int Act_Memo = 0;
            unsigned __int16 Pinit = 127;

            for (i = 1; i < (asura::LEG_NUM - 4); i++) {//for(i=0;i<1;i++){//

                for (j = 0; j < asura::LEG_ACT_NUM; j++) {//for (j=0;j<1;j++){//


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

                    Actpos = 100 * viewAsuraXData.leg_actuator_position[i](j + 1);

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
                    Data2 = data & 0x00ff;
                    Data1 = Data1 >> 8;

                    //パケットチェックサム
                    Cdata1 = Data1 & 0x00f0;
                    Cdata2 = Data2 & 0x000f;
                    Checksum = (unsigned char)(Cdata1 + Cdata2);


                    /// アクチュエータへのパケットデータ
                    longPacket[8 * Act_Memo] = (unsigned char)Pinit;
                    longPacket[8 * Act_Memo + 1] = (unsigned char)TopID;//((CANID & 0x0000FF00 ) >> 8);
                    longPacket[8 * Act_Memo + 2] = (unsigned char)UnderID;//( CANID & 0x000000FF );
                    longPacket[8 * Act_Memo + 3] = (unsigned char)CommandType;
                    longPacket[8 * Act_Memo + 4] = (unsigned char)symbol;
                    longPacket[8 * Act_Memo + 5] = (unsigned char)Data1;
                    longPacket[8 * Act_Memo + 6] = (unsigned char)Data2;
                    longPacket[8 * Act_Memo + 7] = Checksum;

                    Act_Memo++;
                }  // LEG_ACT_NUM


            }  // LEG_NUM
        }
    }


    return 0;
}

}  // namespace designlab_robot_gui::udp


#ifndef DESIGNLABROBOTGUI_UDP_ASURAUDPTHREAD_H_
#define DESIGNLABROBOTGUI_UDP_ASURAUDPTHREAD_H_

#include "..\resource.h"

#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <afx.h>
#include <windows.h>
#include <afxwin.h>

#include "Data/asura_data.h"
#include "Data/plan_data.h"
#include "File/FileWriter.h"
#include "Plan/time_manager.h"
#include "UDP/udp.h"
#include "UDP/udp_packet.h"
#include "UDP/udp_ring_buffer.h"
#include "Utility/RingBuffer.h"
#include "System/CriticalSection.h"
#include "System/WinUserMessage.h"
#include "System/Timer/MultiMediaTimer.h"
#include "System/Thread/TimedMotionProcedure.h"


namespace designlab_robot_gui::udp
{

// AsuraUdpThread

class AsuraUdpThread : public CWinThread
{
    using AsuraData = data::AsuraData;
    using PlanData = data::PlanData;

    DECLARE_DYNCREATE(AsuraUdpThread)


    System::TimedMotionProcedure* pTimedUDPProcedure;
    System::MultiMediaTimer* pMultiMediaTimer;
    static plan::TimeManager		 timeManager;

    /**
     *		フラグ
     */
     /// スレッドが有効かどうか
    BOOL isAlive;

    /// 通信が有効かどうか
    bool isCommAlive;

    //bool isListeningStopped;



protected:
    AsuraUdpThread();           // 動的生成で使用される protected コンストラクター
    virtual ~AsuraUdpThread();

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

    char buildPacket(SciPacketType type, int address, int command);

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


    /*
     *	ポート番号
     */
    u_short s_portNo = 10004;			/// 自分のポート番号
    u_short c_portNo = 10003;				/// 相手のポート番号
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


    /// バッファサイズ
    static const int MAX_BUFFER_SIZE = 2048;

    /**
     *	一時バッファ
     */
     /// 受信バッファ
    //unsigned char* readBuffer;
    /// 送信バッファ
    unsigned char* writeBuffer{};


    /// 送受信サイズ
    DWORD bytesToWrite{};

    /// 送信バッファサイズ
    DWORD writeBufferSize{};
    UINT writeBufferSize_{};
    int bytesToSend{};

private:

    AsuraData viewAsuraXData;
    PlanData viewPlanData;




    /**
     *	パケットタイプ
     */
    SciPacketType sciPacketType;

    /**
     *	送信バッファ
     */
    char longPacket[32];
    unsigned char shortPacket[SCI_MAX_JOINT_NUM][SCI_PACKET_SIZE_SHORT];

    char onelegPacket[SCI_PACKET_SIZE_ONELEG];
};

} // namespace designlab_robot_gui::udp


#endif // DESIGNLABROBOTGUI_UDP_ASURAUDPTHREAD_H_

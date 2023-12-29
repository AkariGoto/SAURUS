#pragma once

#include "..\resource.h"

#include "..\Data\AsuraData.h"
#include "..\Data\PlanData.h"

#include "..\System\ViewSetting.h"
#include "..\File\FileWriter.h"
#include "..\Utility\RingBuffer.h"

//2020/08/21�ǉ�
#include "Udp.h"
#include "UDPPacket.h"

// WinSock 20210622�ǉ�
#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//SerialPort.h��
#include <afx.h>
#include <windows.h>
#include <afxwin.h>

#include "UdpRingBuffer.h"
#include "..\System\WinUserMessage.h"
#include "..\System\CriticalSection.h"
#include "..\System\Timer\MultiMediaTimer.h"
#include "..\System\Thread\TimedMotionProcedure.h"
#include "..\System\Thread\MotionPlanThread.h"

//�N���X�̑O���錾
class UDPSocket;

using namespace UDP;



// AsuraUDPTherad

class AsuraUDPThread : public CWinThread
{

	DECLARE_DYNCREATE(AsuraUDPThread)
	
/**
 *		�^�C�}�I�u�W�F�N�g
 */
	System::TimedMotionProcedure*	 pTimedUDPProcedure;
	System::MultiMediaTimer*		 pMultiMediaTimer;
	static Plan::TimeManager		 timeManager;

/**
 *		�t���O
 */
 /// �X���b�h���L�����ǂ���
	BOOL isAlive;

	/// �ʐM���L�����ǂ���
	bool isCommAlive;

	//bool isListeningStopped;
	


protected:
	AsuraUDPThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~AsuraUDPThread();

public:
	virtual BOOL InitInstance();	//������
	virtual int ExitInstance();


	/**
	 *	����
	 *		�J�n����
	 */
	void initializeAsuraUDPThread(void);

	/**
	 *	����
	 *		�I������
	 */
	void finalizeAsuraUDPThread(void);

	/*
	*
	* �p�P�b�g�f�[�^�쐬
	*
	*/

	char buildPacket(UDP::SciPacketType type, int address, int command);

	/**
	 *	����
	 *		�X���b�h���L�����ǂ���
	 */
	BOOL isRunning(void) const { return isAlive; }

	DECLARE_MESSAGE_MAP()

protected:
	

	/// �X���b�h�I��
	afx_msg void OnEndThread(WPARAM wParam, LPARAM lParam);
	/// �ʐM�J�n
	afx_msg void OnStartPortListening(WPARAM wParam, LPARAM lParam);
	/// �ʐM��~
	afx_msg void OnStopPortListening(WPARAM wParam, LPARAM lParam);

	/// �w�ߒl�����{�b�g�֑��M
	afx_msg void OnSendCommandData(WPARAM wParam, LPARAM lParam);

	/*������20210622
	/// ���O�J�n
	afx_msg void OnStartLogging(WPARAM wParam, LPARAM lParam);
	/// ���O��~
	afx_msg void OnStopLogging(WPARAM wParam, LPARAM lParam);
	*/





/*
 *	�|�[�g�ԍ�
 */
	u_short s_portNo=10004;			/// �����̃|�[�g�ԍ�
	u_short c_portNo=10003;				/// ����̃|�[�g�ԍ�
 /*
 *	IP�A�h���X
 */
	//char server_IPAdress[14]="192.168.0.154";			/// ������IP�A�h���X
	const char server_IPAdress[14] = "192.168.0.157";			/// ������IP�A�h���X
	const char client_IPAdress[14] = "192.168.0.169";			/// ���M��IP�A�h���X


	SOCKET sock;
	struct sockaddr_in server_addr, client_addr;
	int port;
	const char* addr;

/*********************  �\����  ***************************/

	/// overlapped�\����
	//OVERLAPPED		readOverLappedStruct;
	//OVERLAPPED		writeOverLappedStruct;


/**
 *	�C�x���g�z��
 */
 /// �e�C�x���g���ɗp�ӂ����C�x���g�n���h��
 /// WaitForMultipleObjects�֐��̂��߂ɏ���
	//HANDLE		readEventHandles[2];
	//HANDLE		writeEventHandles[2];

	/// �X���b�h�I���n���h��
	//HANDLE		readThreadTerminateHandle;
//HANDLE		writeThreadTerminateHandle;


/**
	 *	�o�b�t�@
	 */
	 /// �o�b�t�@�T�C�Y
	static const int MAX_BUFFER_SIZE = 2048;

	/**
	 *	�ꎞ�o�b�t�@
	 */
	 /// ��M�o�b�t�@
	//unsigned char* readBuffer;
	/// ���M�o�b�t�@
	unsigned char* writeBuffer{};

	/// ��M�L�^�o�b�t�@
	//unsigned char* SciRingBufferreceiveBuffer;

	/// ����M�T�C�Y
	//DWORD bytesToRead{};
	DWORD bytesToWrite{};

	/// ��M�o�b�t�@�T�C�Y
	//DWORD		readBufferSize;
	/// ���M�o�b�t�@�T�C�Y
	DWORD writeBufferSize{};
	UINT writeBufferSize_{};
	int bytesToSend{};




private:

	Data::AsuraData viewAsuraXData;
	Data::PlanData viewPlanData;

	


	/**
	 *	�p�P�b�g�^�C�v
	 */
	UDP::SciPacketType sciPacketType;

	/**
	 *	���M�o�b�t�@
	 */
	//char longPacket[UDP::SCI_PACKET_SIZE_LONG];//unsigned char
	char longPacket[32];
	unsigned char shortPacket[UDP::SCI_MAX_JOINT_NUM][UDP::SCI_PACKET_SIZE_SHORT];

	char onelegPacket[UDP::SCI_PACKET_SIZE_ONELEG];//�ǉ�


	//bool buildPacket(UDP::SciPacketType type, char* packet, int address, int command);

	//UDPSocket* pUdpPort;

	//void writeData(AsuraUDPThread* port, COMSTAT& comStat);



	/// �����I�u�W�F�N�g
	//System::CriticalSection	criticalSection;

	//UDPSocket* pUDPSocket;

};



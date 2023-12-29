/**
 *  �t�@�C����
 *		Socket.h
 *  ����
 *		UDP�ʐM�Ɋւ���N���X
 *  ���t
 *		�쐬��: 2019/11/18(Sat)		�X�V��: 2007/11/03(Sat)
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

//Winsock�֘A
#include<winSock2.h>

namespace UDP
{
/**
 *	----------------------------------------------------------------------
 *		UdpSocket�N���X
 *	----------------------------------------------------------------------
 */
class UdpSocket {
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

private:
    //SOCKET sock ; // �\�P�b�g
	UdpSocket* udpsocket;
    //struct sockaddr_in addr ;
	// sockaddr_in server_addr, client_addr;   // ����M�A�h���X

protected :

	/******************  �l�b�g���[�N�ݒ�(yoshida���񂪉������Ƃ�)*****************/
 
               	       /*�|�[�g�ԍ�*/
	u_short local_portNo	;			 /// �����̃|�[�g�ԍ�
	u_short remort_portNo;				 /// ����̃|�[�g�ԍ�

	                   /*IP�A�h���X*/
	char local_IPAdress[13]	;			 /// ������IP�A�h���X
	char remort_IPAdress[13];			 /// ���M��IP�A�h���X

	                  /*winsock�֘A*/
	WSAData				wsaData;         ///WSAStartup�̏�Ԋm�F�̍\����
	int					WSAHandle;
	SOCKET				sockHandle;      ///�\�P�b�g�쐬
	struct sockaddr_in	addr;           ///IP�A�h���X�̍\����


/**
 *	COM�|�[�g�֘A
 */
	/// COM�|�[�g�̐e�E�B���h�E�ւ̃n���h��
	HWND				commOwnerWindowHandle;
	/// COM�|�[�g�n���h��
	HANDLE				commHandle;
	/// �ʐM�C�x���g
	DWORD				commEvent;
	/// �|�[�g�ԍ�
	UINT				commPortNumber;
	/// �ʐM�̃^�C���A�E�g�ݒ�������\����
	COMMTIMEOUTS	commTimeoutsStruct;
	/// Device-Control Block�\���́i�ʐM�����̐ݒ�j
	DCB					commDCBStruct;

/*********************  �\����  ***************************/

	/// overlapped�\����
	OVERLAPPED		readOverLappedStruct;
	OVERLAPPED		writeOverLappedStruct;

/*****************  ����M�p�X���b�h�֘A  ******************/

	/// �X���b�h�|�C���^
	CWinThread* UDPreadThreadPointer;
	CWinThread* UDPwriteThreadPointer;

	/// �X���b�h��������̃t���O
	BOOL UDPisReadThreadAlive;
	BOOL UDPisWriteThreadAlive;

	/// �����I�u�W�F�N�g
	System::CriticalSection	criticalSection;

	/// �X���b�h�I���n���h��
	HANDLE		UDPreadThreadTerminateHandle;
	HANDLE		UDPwriteThreadTerminateHandle;

/****************  �C�x���g�z��  *************************/	

	/// �e�C�x���g���ɗp�ӂ����C�x���g�n���h��
	/// WaitForMultipleObjects�֐��̂��߂ɏ���
	HANDLE		readEventHandles[2];
	HANDLE	    writeEventHandles[2];




/*********************  �o�b�t�@  ************************/
	/// �o�b�t�@�T�C�Y
	static const int MAX_BUFFER_SIZE = 1024;


/*******************  �ꎞ�o�b�t�@ **************************/

	/// ��M�o�b�t�@
	unsigned char* readBuffer; //unsigned char* UDPreadBuffer;  
	/// ���M�o�b�t�@
	char* writeBuffer;
	//unsigned char* writeBuffer;

	/// ��M�L�^�o�b�t�@
	SciRingBuffer receiveBuffer;

	/// ����M�T�C�Y
	DWORD bytesToRead;
	DWORD bytesToWrite;

	/// ��M�o�b�t�@�T�C�Y
	DWORD		readBufferSize;
	/// ���M�o�b�t�@�T�C�Y
	DWORD		writeBufferSize;

/**
 *		�t���O
 */
	bool UDPisListeningStopped;
	bool UDPisCommOpen;




public:
/**************************************************************************/
                  //�R���X�g���N�^���f�X�g���N�^
/**************************************************************************/
	explicit UdpSocket();                  
	virtual ~UdpSocket();                  

/************************  UDP���J��  ******************************/
	BOOL  openUDP(	HWND hOwnerWnd,								/// COM�|�[�g�̐e�E�B���h�E
							UINT portNum = 1,							/// COM�|�[�g�ԍ�
							UINT baudRate = 115200,						/// �{�[���[�g
							char parity = 'N',							/// �p���e�B
							UINT dataBits = 8,							/// �f�[�^�r�b�g 
							UINT stopBits = 1,							/// �X�g�b�v�r�b�g
							DWORD commEvents = EV_RXCHAR | EV_CTS,		/// ���[�U���ݒ肷��ʐM�C�x���g
							UINT readBufferSize_ = MAX_BUFFER_SIZE,		/// ��M�o�b�t�@�̃T�C�Y
							UINT writeBufferSize_ = MAX_BUFFER_SIZE		/// ���M�o�b�t�@�̃T�C�Y
						);   // ������           �i���s:0 ����:1�j


/**************************  UDP�����  *******************************/
    void closeUDP(void);
	    

	//���M�f�[�^�������ɓ����
	char buf[2048] ;  //�Ƃ�ܕۗ�


/**************************************************************************/
                  //�A�N�Z�X�֐�
/**************************************************************************/
	
	/// ���M/��M�o�b�t�@�T�C�Y
	DWORD UDPgetReadBufferSize(void) const{return readBufferSize;}
	DWORD UDPgetWriteBufferSize(void) const{return writeBufferSize;}



	/**
 *	����
 *		COM�|�[�g�փf�[�^�𑗐M
 *	����
 *		sendData: �������݃f�[�^�ւ̃|�C���^
 *		bytesToSend: �������݃T�C�Y
 */
	void	sendData( char* sendData, DWORD bytesToSend);//unsigned

    void receiveData( char* receiveData, DWORD bytesToReceive);



/******************  COM�|�[�g�̊Ď��֐��i�X���b�h�J�n�E��~�E�I���j***************************/

	/// �Ď��J�n
	BOOL UDPstartListening(void); //��������X���b�h�J�n���Ă�
	/// �Ď��ĊJ
	BOOL UDPrestartListening(void);
	/// �Ď���~
	BOOL UDPstopListening(void);



protected:
	/**	
 *	����
 *		�ʐM�G���[�̎擾
 *	����
 *		�������ݐ�ւ̃|�C���^
 */
	void outputProcessErrorMessage(char* errorText);



	

/*	----------------------------------------
 	����M�ɗp����X���b�h�֐�
 *	----------------------------------------*/
	/// ��M�X���b�h�{��
	static UINT UDPreadEventListenThread(LPVOID pParam);//
	/// ���M�X���b�h�{��
	static UINT UDPwriteEventListenThread(LPVOID pParam);//

	/// �f�[�^��M�֐�
	static void	UDPreadData(UdpSocket* udpsocket, COMSTAT& comStat);   //������Socket�N���X�̃I�u�W�F�N�g, ���̒���UDP��M������s��
	/// �f�[�^���M�֐�
	static void	UDPwriteData(UdpSocket* udpsocket, COMSTAT& comStat);  //������Socket�N���X�̃I�u�W�F�N�g, ���̒���UDP���M������s��


private:
	/// �R�s�[�R���X�g���N�^����
	UdpSocket(const UdpSocket& serial);

	/// ������Z�q����
	UdpSocket& operator=(const UdpSocket& serial);

}; //end of class Socket

}//end of namespace UDP


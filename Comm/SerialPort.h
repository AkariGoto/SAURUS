/**
 *  �t�@�C����
 *		SerialPort.h
 *  ����
 *		�V���A���|�[�g���`�F�b�N���f�[�^�𑗎�M����
 *		�|�[�g�ɕω����N�������烆�[�U�ɒʒm����
 *		����M�p�̃X���b�h���ʓr�쐬����
 *  ���t
 *		�쐬��: 2007/04/01(Sat)		�X�V��: 2007/11/03(Sat)
 */

#ifndef __SerialPort_h__
#define __SerialPort_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */

#include <afx.h>
#include <windows.h>
#include <afxwin.h>

#include "SciRingBuffer.h"
#include "..\System\WinUserMessage.h"
#include "..\System\CriticalSection.h"

namespace Comm
{

/**
 *	----------------------------------------------------------------------
 *		SerialPort�N���X
 *	----------------------------------------------------------------------
 */
class SerialPort
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
protected:
/*
*	�l�b�g���[�N�ݒ�
*/	//yoshida���񂪉������Ƃ�
 /*
 *	�|�[�g�ԍ�
 */
	u_short local_portNo	;			/// �����̃|�[�g�ԍ�
	u_short remort_portNo;				/// ����̃|�[�g�ԍ�
 /*
 *	IP�A�h���X
 */
	char local_IPAdress[13]	;			/// ������IP�A�h���X
	char remort_IPAdress[13];			/// ���M��IP�A�h���X
/**
 *	winsock�֘A
 */
	WSAData				wsaData; ///	WSAStartup�̏�Ԋm�F�̍\����
	int					WSAHandle;
	SOCKET				sockHandle;
	struct sockaddr_in	addr; /// sendto�֐�(winsock�̑��M�֐�)�֓n���|�[�g,IP�A�h���X�̍\����

/****************�������牺�����Ƃ���̋L�q************************/
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

/**
 *	�\����
 */
	/// overlapped�\����
	OVERLAPPED		readOverLappedStruct;
	OVERLAPPED		writeOverLappedStruct;

/**
 *	����M�p�X���b�h�֘A
 */
	/// �X���b�h�|�C���^
	CWinThread* readThreadPointer;
	CWinThread* writeThreadPointer;

	/// �X���b�h��������̃t���O
	BOOL isReadThreadAlive;
	BOOL isWriteThreadAlive;

	/// �����I�u�W�F�N�g
	System::CriticalSection	criticalSection;

	/// �X���b�h�I���n���h��
	HANDLE		readThreadTerminateHandle;
	HANDLE		writeThreadTerminateHandle;

/**
 *	�C�x���g�z��
 */	
	/// �e�C�x���g���ɗp�ӂ����C�x���g�n���h��
	/// WaitForMultipleObjects�֐��̂��߂ɏ���
	HANDLE		readEventHandles[2];
	HANDLE		writeEventHandles[2];

/**
 *	�o�b�t�@
 */
	/// �o�b�t�@�T�C�Y
	static const int MAX_BUFFER_SIZE = 1024;

	/**
	 *	�ꎞ�o�b�t�@
	 */
	/// ��M�o�b�t�@
	unsigned char* readBuffer;   //�����O�Funsigned char* readBuffer
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
	bool isListeningStopped;
	bool isCommOpen;

private:

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	/// �R���X�g���N�^
	explicit SerialPort();
	/// �f�X�g���N�^
	virtual ~SerialPort();

/**
 *	����
 *		�V���A���|�[�g���J��
 *	����
 *		hOwnerWnd: COM�|�[�g�̐e�E�B���h�E
 *		portNum: COM�|�[�g�ԍ�
 *		baudRate: �{�[���[�g
 *		parity: �p���e�B
 *		dataBits: �f�[�^�r�b�g
 *		stopBits: �X�g�b�v�r�b�g
 *		commEvents: ���[�U���ݒ肷��ʐM�C�x���g
 *		readBufferSize_: ��M�o�b�t�@�̃T�C�Y
 *		writeBufferSize_: ���M�o�b�t�@�̃T�C�Y
 */
	BOOL openSerialPort(	HWND hOwnerWnd,								/// COM�|�[�g�̐e�E�B���h�E
							UINT portNum = 1,							/// COM�|�[�g�ԍ�
							UINT baudRate = 115200,						/// �{�[���[�g
							char parity = 'N',							/// �p���e�B
							UINT dataBits = 8,							/// �f�[�^�r�b�g 
							UINT stopBits = 1,							/// �X�g�b�v�r�b�g
							DWORD commEvents = EV_RXCHAR | EV_CTS,		/// ���[�U���ݒ肷��ʐM�C�x���g
							UINT readBufferSize_ = MAX_BUFFER_SIZE,		/// ��M�o�b�t�@�̃T�C�Y
							UINT writeBufferSize_ = MAX_BUFFER_SIZE		/// ���M�o�b�t�@�̃T�C�Y
						);

/**
 *	����
 *		�V���A���|�[�g�����
 */
	void closeSerialPort(void);

/**
 *	����
 *		DCB�̐ݒ�i�f�[�^����u���b�N�j
 *	����
 *		deviceCtrlStr: �f�o�C�X�������\��������ւ̃|�C���^
 */
	BOOL initializeCommDCB(LPTSTR deviceCtrlStr);

/**
 *	����
 *		COMMTIMEOUTS�̐ݒ�
 */
	BOOL initializeCommTimeouts(void);

/**
 *	----------------------------------------	
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
	/// COM�|�[�g�n���h��
	HANDLE getCommPortHandle(void) const{return commHandle;}
	/// �ʐM�C�x���g
	DWORD getCommPortEvent(void) const{return commEvent;}
	/// �|�[�g�ԍ�
	UINT getCommPortNumber(void) const{return commPortNumber;}
	/// �ʐM�̃^�C���A�E�g�ݒ�������\����
	COMMTIMEOUTS getCommTimeoutsStruct(void) const{return commTimeoutsStruct;}
	/// Device-Control Block�\���́i�ʐM�����̐ݒ�j
	DCB getCommDCBStruct(void) const{return commDCBStruct;}

	/// ���M/��M�o�b�t�@�T�C�Y
	DWORD getReadBufferSize(void) const{return readBufferSize;}
	DWORD getWriteBufferSize(void) const{return writeBufferSize;}

	/// �����O�o�b�t�@�{��
	SciRingBuffer& getReceiveBuffer(void) {return receiveBuffer;}

/**
 *	COM�|�[�g�̊Ď��֐��i�X���b�h�J�n�E��~�E�I���j
 */
	/// �Ď��J�n
	BOOL startListening(void); //��������X���b�h�J�n���Ă�
	/// �Ď��ĊJ
	BOOL restartListening(void);
	/// �Ď���~
	BOOL stopListening(void);

/**
 *	����
 *		COM�|�[�g�փf�[�^�𑗐M
 *	����
 *		sendData: �������݃f�[�^�ւ̃|�C���^
 *		bytesToSend: �������݃T�C�Y
 */
	void	sendData( char* sendData, DWORD bytesToSend);//unsigned

protected:
/**	
 *	����
 *		�ʐM�G���[�̎擾
 *	����
 *		�������ݐ�ւ̃|�C���^
 */
	void outputProcessErrorMessage(char* errorText);

/**
 *	----------------------------------------
 *	����M�ɗp����X���b�h�֐�
 *	----------------------------------------
 */
	/// ��M�X���b�h�{��
	static UINT readEventListenThread(LPVOID pParam);//
	/// ���M�X���b�h�{��
	static UINT writeEventListenThread(LPVOID pParam);//

	/// �f�[�^��M�֐�
	static void	readData(SerialPort* port, COMSTAT& comStat);//
	/// �f�[�^���M�֐�
	static void	writeData(SerialPort* port, COMSTAT& comStat);//

private:
	/// �R�s�[�R���X�g���N�^����
	SerialPort(const SerialPort& serial);

	/// ������Z�q����
	SerialPort& operator=(const SerialPort& serial)=delete;
};	/// end of class SerialPort

}	/// end of namespace Comm

#endif	/// __SerialPort_h__
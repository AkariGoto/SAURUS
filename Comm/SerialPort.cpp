/**
 *  �t�@�C����
 *		SerialPort.cpp
 *  ����
 *		�V���A���|�[�g���`�F�b�N���f�[�^�𑗎�M����
 *		�|�[�g�ɕω����N�������烆�[�U�ɒʒm����
 *		����M�p�̃X���b�h���ʓr�쐬����
 *  ���t
 *		�쐬��: 2007/04/01(Sat)		�X�V��: 2007/11/03(Sat)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <cassert>
#include <process.h>
//#include "ASURA2GUI/pch.h"
#include "..\pch.h"
#include "SerialPort.h"
#include "..\System\DebugOutput.h"
#include <winsock2.h>//�ǉ�

namespace Comm
{
/**
 *	----------------------------------------------------------------------
 *		SerialPort�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		SerialPort�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */
/**
 *	�R���X�g���N�^
 */
SerialPort::SerialPort()
{
/**
 *	�|�[�g�ԍ�
 */
	/// �����̃|�[�g�ԍ�
	local_portNo		= 10004;
	/// ����̃|�[�g�ԍ�
	remort_portNo		= 10003;
/**
 *	IP�A�h���X
 */
	/// ������IP�A�h���X
	//sprintf(local_IPAdress,"192.168.0.154");
	sprintf(local_IPAdress, "192.168.0.157");
	/// �����IP�A�h���X
	sprintf(remort_IPAdress,"192.168.0.169");	//���M��IP�A�h���X


/**
 *	WASData,Soket�n���h��������
 *
 *	WASStartup	���s:  
 *				����:  0	
 *
 *	soket		���s: SOCKET_ERROR(-1)
 *				����: ���łȂ�(��0)�\�P�b�g�L�q�q
 */
	WSAHandle	= 1;
	sockHandle	= INVALID_SOCKET ;

/**
 *	�I�[�i�[�E�B���h�E������
 */
	commOwnerWindowHandle = NULL;

/**
 *	COM�|�[�g�n���h���̏�����
 */
	commHandle = NULL;

/** 
 *	OVERLAPPED�\���̂̃����o�ϐ���0�ɂ���
 */
	readOverLappedStruct.Offset = 0;
	writeOverLappedStruct.Offset = 0;
	readOverLappedStruct.OffsetHigh = 0;
	writeOverLappedStruct.OffsetHigh = 0;

	/// �C�x���g�n���h���̏�����
	readOverLappedStruct.hEvent		= NULL;
	writeOverLappedStruct.hEvent	= NULL;

	/// �o�b�t�@�̏�����
	/// ��M�p
	readBuffer = NULL;
	/// ���M�p
	writeBuffer = NULL;

	/// ����M�T�C�Y
	bytesToRead = 0;
	bytesToWrite = 0;

/**
 *	�X���b�h�֘A�̏�����
 */
	/// �n���h��
	readThreadTerminateHandle = NULL;
	writeThreadTerminateHandle = NULL;

	/// �X���b�h�t���O
	isReadThreadAlive = FALSE;
	isWriteThreadAlive = FALSE;

/**
 *	�t���O�̏�����
 */
	isListeningStopped = true;
	isCommOpen = false;
}

/**
 *	�f�X�g���N�^
 */
SerialPort::~SerialPort()
{
	closeSerialPort();
}

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
BOOL SerialPort::openSerialPort(	HWND hOwnerWnd,				/// ��M�|�[�g�̐e�E�B���h�E�n���h��
									UINT portNo,				/// �|�[�g�ԍ�
									UINT baudRate,				/// �{�[���[�g
									char parity,				/// �p���e�B
									UINT dataBits,				/// �f�[�^�r�b�g
									UINT stopBits,				/// �X�g�b�v�r�b�g
									DWORD commEvent_,			/// �C�x���g
									UINT readBufferSize_,		/// ��M�o�b�t�@�T�C�Y
									UINT writeBufferSize_		/// ���M�o�b�t�@�T�C�Y
								)
{
/**
 *	�����`�F�b�N
 */
	//DEBUG_ASSERT(portNo > 0 && portNo < 6);
//	DEBUG_ASSERT(commOwnerWindowHandle != NULL);

/**
 *	�X���b�h�m�F
 */
	/// ���ɃX���b�h�������Ă��������
	if (isWriteThreadAlive)
	{
		do
		{
			SetEvent(writeThreadTerminateHandle);//�C�x���g�I�u�W�F�N�g���V�O�i����Ԃɂ���
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
 *	�C�x���g�쐬
 */
	///OVERLAPPED�\���̂̑���M�C�x���g
	if (readOverLappedStruct.hEvent != NULL)
		ResetEvent(readOverLappedStruct.hEvent);
	readOverLappedStruct.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (writeOverLappedStruct.hEvent != NULL)
		ResetEvent(writeOverLappedStruct.hEvent);
	writeOverLappedStruct.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	/// �X���b�h�j���C�x���g
	if (readThreadTerminateHandle != NULL)
		ResetEvent(readThreadTerminateHandle);
	readThreadTerminateHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (writeThreadTerminateHandle != NULL)
		ResetEvent(writeThreadTerminateHandle);
	writeThreadTerminateHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	/// �C�x���g�z��̏������iHANDLE�^��void*�Ȃ̂ŏ������͂����݂̂ŗǂ��j
	readEventHandles[0] = readThreadTerminateHandle;
	readEventHandles[1] = readOverLappedStruct.hEvent;

	writeEventHandles[0] = writeThreadTerminateHandle;
	writeEventHandles[1] = writeOverLappedStruct.hEvent;

/**
 *	�|�[�g�ݒ�
 */
	/// �V���A���|�[�g�̏��L�������i���b�Z�[�W���󂯂�j�E�B���h�E�n���h�����Z�b�g
	commOwnerWindowHandle = hOwnerWnd;

	/// �|�[�g�ԍ��Z�b�g
	commPortNumber = portNo;

/*************************************************************************************
 *	                       winsock�ݒ�
 */
///***********************************************************************************
	
	///winsock������
	WSAHandle = WSAStartup(MAKEWORD(2,0), &wsaData);///Ver.2.0
	//WSAHandle = WSASYSNOTREADY;//�Ӑ}�I�G���[


	///�G���[�����@(�����������s�����winsock�̋@�\���g���Ȃ��̂ŗv�m�F)
	///WSAStartup :���������ꍇ,�߂�l0	
	assert(WSAHandle == 0);
	if (WSAHandle == 0){
		///WSASETUP����
		//DEBUG_TRACE(TEXT("WSASETUP\n"));
	}
	else if (WSAHandle != 0) {
		switch (WSAHandle) {
			///�l�b�g���[�N�T�u�V�X�e�����l�b�g���[�N�ւ̐ڑ��������ł��Ă��Ȃ�
			case WSASYSNOTREADY:
				DEBUG_TRACE(TEXT("WSASYSNOTREADY\n"));
			break;
			///�v�����ꂽwinsock�̃o�[�W�������T�|�[�g����Ă��Ȃ�
			case WSAVERNOTSUPPORTED:
				DEBUG_TRACE(TEXT("WSAVERNOTSUPPORTED\n"));
			break;
			///�u���b�L���O����̎��s���ł��邩�A �܂��̓T�[�r�X�v���o�C�_���R�[���o�b�N�֐����������Ă���
			case WSAEINPROGRESS:
				DEBUG_TRACE(TEXT("WSAEINPROGRESS\n"));
			break;
			///winsock�������ł���ő�v���Z�X���ɒB����
			case WSAEPROCLIM:
				DEBUG_TRACE(TEXT("WSAEPROCLIM\n"));
			break;
			///	�������ł���lpWSAData �͗L���ȃ|�C���^�ł͂Ȃ�
			case WSAEFAULT:
				DEBUG_TRACE(TEXT("WSAEFAULT\n"));
			break;
			assert(WSAHandle =! 0);
		}
		return FALSE;
	}//*/
/**
 *	�o�b�t�@�֌W
 */
	/// �o�b�t�@�̈�̊m��
	if (readBufferSize != NULL) delete [] readBuffer;
	if (readBufferSize_ > MAX_BUFFER_SIZE) readBufferSize_ = MAX_BUFFER_SIZE;
	readBuffer = new unsigned char[readBufferSize_];

	if (writeBuffer != NULL) delete [] writeBuffer;
	if (writeBufferSize_ > MAX_BUFFER_SIZE) writeBufferSize_ = MAX_BUFFER_SIZE;
	writeBuffer = new char[writeBufferSize_];
	//writeBuffer = new unsigned char[writeBufferSize_];

	/// �����O�o�b�t�@�̐ݒ�
	receiveBuffer.initialize(11);

	/// �o�b�t�@�T�C�Y�̐ݒ�
	readBufferSize = readBufferSize_;
	writeBufferSize = writeBufferSize_;

	/// �o�b�t�@�̏�����
	ZeroMemory(readBuffer, readBufferSize);
	ZeroMemory(writeBuffer, writeBufferSize);

/**
 *	���̑�
 */
	/// �ʐM�C�x���g�̐ݒ�
	commEvent = commEvent_;

	/// COM�|�[�g�̂��߂̕�����
	LPTSTR portNoStr = (LPTSTR)malloc(256);
	LPTSTR dcbStr = (LPTSTR)malloc(256);

/**
 *	�r�������J�n
 */
	criticalSection.lock();
	
	/// �\�P�b�g�����ɊJ���Ă�����C����
	if (commHandle != NULL)
	{
		///CloseHandle(commHandle);
		//closesocket(sockHandle); ///�\�P�b�g��
		commHandle = NULL;
	}
///*

	
	///�\�P�b�g�쐬
	sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
	//AF_INET��IPv4�ASOCK_DGRAM��UDP�ʐM�A0�́H
	
	
	///�G���[����
	///���s������socket��-1��Ԃ�
	if(sockHandle =! -1){
		#define SOCK_HANDLE_VALUE ((HANDLE)(2))
		commHandle = SOCK_HANDLE_VALUE;///<---�n���h���̒l���킩��Ȃ�
	}
	///�G���[
	else if (sockHandle == -1){//INVALID_SOCKET
		///�G���[�l��\�� (VC++�G���[���b�N�A�b�v�ɃG���[�l����͂��Ċm�F���Ă�������)
		DEBUG_TRACE(TEXT("socket error : %d\n", WSAGetLastError()));
		assert(sockHandle=! -1);
        return FALSE;
	}
//*/
	/// COM�|�[�g�����ɊJ���Ă�����C����
	if (commHandle != NULL)
	{
		CloseHandle(commHandle);
		commHandle = NULL;
	}

	/// �|�[�g���J�����߂̕�����̏���
	wsprintf(portNoStr, TEXT("COM%d"), portNo);
	wsprintf(dcbStr, TEXT("baud=%d parity=%c data=%d stop=%d"), baudRate, parity, dataBits, stopBits);

	/// COM�|�[�g�ւ̃n���h�����擾
/*	commHandle = CreateFile(	portNoStr,						/// �ʐM�|�[�g������ (COMX)
								GENERIC_READ | GENERIC_WRITE,	/// �ǂݍ���/�������݂̎��
								0,								/// �ʐM�|�[�g�Ȃ̂Ŕr���A�N�Z�X�ŊJ��
								NULL,							/// �擾�����n���h���̎q�v���Z�X�ւ̌p���������邩�ǂ��������߂�SECURITY_ATTRIBUTES�\���̂ւ̃|�C���^���w��
								OPEN_EXISTING,					/// �ʐM�f�o�C�X���J�����͂��̃t���O�iOPEN_EXISTING�j���w�肷��
								FILE_FLAG_OVERLAPPED,			/// �񓯊����o�́i�t�@�C���ɑ΂��ĕ����̑���𓯎��ɍs�����Ƃ��\�j
								0);								/// �e���v���[�g�t�@�C���ւ�GENERIC_READ�A�N�Z�X�����n���h�����w��i0���w�肷��j

	/// COM�|�[�g���J���ꂽ�ǂ����̃`�F�b�N
	if (commHandle == INVALID_HANDLE_VALUE)
	{
		/// �|�[�g��������Ȃ�������
		free(portNoStr);
		free(dcbStr);

		DEBUG_TRACE(TEXT("Failed to open COM Port Reason: %d\n"), GetLastError());

		return FALSE;
	}
*/
	/// ����M�o�b�t�@�̗e�ʂ�ݒ�

/**
 *	COM�|�[�g�̏����ݒ�
 */
/*	if (SetupComm(commHandle, readBufferSize, writeBufferSize))
	{
		/// COMMTIMEOUTS�\���̂̐ݒ�
		if (initializeCommTimeouts())
		{
			//
			 *	SetCommMask()
			 *		COM�|�[�g�ł̃f�[�^�̒��M������ԃG���[�̔����C�o�̓o�b�t�@�̍Ō�̕������M�Ȃǂ�
			 *		�����̃C�x���g���܂Ƃ߂ĊĎ�����֐�
			 *		��1����: �ʐM�f�o�C�X�ւ̃n���h��
			 *		��2����: �Ď�����C�x���g
			//
			if (SetCommMask(commHandle, commEvent_))
			{
				/// ���݂�COM�|�[�g�̒ʐM�ݒ���擾����
				if (GetCommState(commHandle, &commDCBStruct))
				{
					/// DCB�\���̂̐ݒ�
					if (initializeCommDCB(dcbStr))
					{
						/// COM�|�[�g�̒ʐM�ݒ���s��
						if ( SetCommState(commHandle, &commDCBStruct))
						{
						}
						else	/// COM�|�[�g�̒ʐM�ݒ莸�s
						{
							outputProcessErrorMessage("SetCommState()");
						}
					}
					else	/// DCB�\���̂̐ݒ莸�s
					{
						outputProcessErrorMessage("initializeCommTimeouts");
					}
				}
				else	/// ���݂�COM�|�[�g�̒ʐM�ݒ�̎擾���s
				{
					outputProcessErrorMessage("GetCommState()");
				}
			}
			else	/// SetCommMask()���s
			{
				outputProcessErrorMessage("SetCommMask()");
			}
		}
		else	/// COMMTIMEOUTS�\���̂̐ݒ莸�s
		{
			outputProcessErrorMessage("initializeCommTimeouts()");		
		}
	}
	else
	{
		/// ����M�o�b�t�@�̐ݒ莸�s
		outputProcessErrorMessage("SetupComm()");
	}


	/// �ʐM�|�[�g�ݒ�̂��߂̕��������
	free( portNoStr );
	free( dcbStr );

	/// ���ɂ���COM�|�[�g�̑���M�o�b�t�@���N���A����
	PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
*/

/**
 *	�r�������I��
 */
	criticalSection.unlock();

	/// �t���O�̏�����
	isListeningStopped = false;
	isCommOpen = true;

	/// �f�o�b�O�p
	DEBUG_TRACE( TEXT("Initialization for communication port %d completed.\nUse Start monitor to communicate.\n"), portNo );

	return TRUE;

}	/// end of openSerialPort()

/**
 *	����
 *		�V���A���|�[�g�����
 *		�\�P�b�g�����
 */
void SerialPort::closeSerialPort(void)
{
	// ���ɕ����Ă�����I��
	if (!isCommOpen)
		return;

	/// �X���b�h��suspend����Ă���A�ĊJ
	if (isListeningStopped)
		restartListening();

	///�\�P�b�g�����
	closesocket(sockHandle);

	///winsock�I��
	WSACleanup();

/**
 *	�X���b�h�I��
 */
	do
	{
		SetEvent(writeThreadTerminateHandle);
	} while (isWriteThreadAlive);

	/// �X���b�h�̏I���҂�
	WaitForSingleObject(writeThreadPointer->m_hThread, INFINITE);


	do
	{
		SetEvent(readThreadTerminateHandle);
	} while (isReadThreadAlive);

	/// �X���b�h�̏I���҂�
	WaitForSingleObject(readThreadPointer->m_hThread, INFINITE);

	DEBUG_TRACE( TEXT("Threads ended\n") );

	/// �X���b�h�I�u�W�F�N�g�̏���
	delete writeThreadPointer;
	delete readThreadPointer;

/**
 *	�o�b�t�@�̌�n��
 */
	if (writeBuffer != NULL)
	{
		delete [] writeBuffer;
		writeBuffer = NULL;
	}

	if (readBuffer != NULL)
	{
		delete [] readBuffer;
		readBuffer = NULL;
	}

/**
 *	�n���h�������
 */
	/// OVERLAPPED�\���̂̃C�x���g�n���h��
	if (readOverLappedStruct.hEvent != NULL)
	{
		CloseHandle(readOverLappedStruct.hEvent);
		readOverLappedStruct.hEvent = NULL;
	}

	/// OVERLAPPED�\���̂̃C�x���g�n���h��
	if (writeOverLappedStruct.hEvent != NULL)
	{
		CloseHandle(writeOverLappedStruct.hEvent);
		writeOverLappedStruct.hEvent = NULL;
	}

	/// �X���b�h�I���̃C�x���g�n���h��
	if (readThreadTerminateHandle != NULL)
	{
		CloseHandle(readThreadTerminateHandle);
		readThreadTerminateHandle = NULL;
	}

	/// �X���b�h�I���̃C�x���g�n���h��
	if (writeThreadTerminateHandle != NULL)
	{
		CloseHandle(writeThreadTerminateHandle);
		writeThreadTerminateHandle = NULL;
	}


	/// �������I���C�o�b�t�@�̃N���A
	if (commHandle)
		PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/// COM�|�[�g�n���h��
	if (commHandle != INVALID_HANDLE_VALUE && commHandle != NULL)
	{
		CloseHandle(commHandle);
		commHandle = NULL;
	}

	/// �t���O����
	isListeningStopped = true;
	isCommOpen = false;
}

/**
 *	����
 *		DCB�̐ݒ�
 *	����
 *		deviceCtrlStr: �f�o�C�X�������\��������ւ̃|�C���^
 */
BOOL SerialPort::initializeCommDCB(LPTSTR deviceCtrlStr)
{
	/**
	 *	��{�ݒ�͈����ł���deviceCtrlStr�ɂ���Č��肷��
	 *	BaudRate, Parity, DataBit, StopBit
	 */

    /**
	 *	�t���[����
	 */
	//commDCBStruct.fOutxCtsFlow		= FALSE;				/// CTS�t���[����Ȃ�
	//commDCBStruct.fOutxDsrFlow		= FALSE;				/// DSR�t���[����Ȃ�
	commDCBStruct.fDtrControl			= DTR_CONTROL_ENABLE;	/// DTR���C���L��
	commDCBStruct.fRtsControl			= RTS_CONTROL_ENABLE;	/// RTS���C���L��
	//commDCBStruct.fDsrSensitivity		= FALSE;				/// DSR����Ȃ� 
    
	/**
	 *	XON/XOFF
	 */
	commDCBStruct.fOutX					= FALSE;				/// XON�Ȃ�
	commDCBStruct.fInX					= FALSE;				/// XOFF�Ȃ�
	//commDCBStruct.fTXContinueOnXoff	= TRUE;					/// XON���M������M���p��
	//commDCBStruct.XonLim				= 2048;					/// XON��������܂łɊi�[�ł���ŏ��o�C�g��
	//commDCBStruct.XoffLim				= 2048;					/// XOFF��������܂łɊi�[�ł���ŏ��o�C�g��
	//commDCBStruct.XonChar				= 0x11;					/// XON�L�����N�^
	//commDCBStruct.XoffChar			= 0x13;					/// XOFF�L�����N�^ 
    
	/** 
	 *	���̑��̐���
	 */
	commDCBStruct.fBinary				= TRUE;					/// �o�C�i�����[�h
	//commDCBStruct.fNull				= FALSE;				/// NULL�o�C�g�͔j�����Ȃ�
	commDCBStruct.fAbortOnError			= FALSE;				/// �G���[���͓ǂݏ���������I��
	//commDCBStruct.fErrorChar			= FALSE;				/// �p���e�B�G���[���̃L�����N�^�u���Ȃ�
	//commDCBStruct.ErrorChar			= 0x00;					/// �u���L�����N�^
	//commDCBStruct.EofChar				= 0x03;					/// �f�[�^�I���L�����N�^
	//commDCBStruct.EvtChar				= 0x0a;					/// �C�x���g�L�����N�^ 

	/// DCB�\���̂�ݒ肷��
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
 *	����
 *		COMMTIMEOUTS�̐ݒ�
 */
BOOL SerialPort::initializeCommTimeouts(void)
{
/**
 *	�^�C���A�E�g��ݒ肵�C��M�J�n����10ms�ȓ��ɉ�����������Ζ�����
 */
/**
 *	��M�^�C���A�E�g
 *		ReadTotalTimeoutMultiplier x ��M�o�C�g�� + ReadTotalTimeoutConstant
 *	���M�^�C���A�E�g
 *		WriteTotalTimeoutMultiplier x ���M�o�C�g�� + WriteTotalTimeoutConstant
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
 *	COM�|�[�g�̊Ď��֐��i�X���b�h�J�n�E��~�E�I���j
 */
/// ���j�^�����O�J�n
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

//////////��M�X���b�h�J�n///////////////////////////////////////////////////////////
	readThreadPointer = AfxBeginThread(	readEventListenThread,    //����֐�
										(LPVOID)this,             //����֐��ɓn���p�����[�^
										THREAD_PRIORITY_NORMAL,   //�X���b�h�̗D�揇��
										0,                        //�쐬����ɃX���b�h���J�n
										CREATE_SUSPENDED
										);

	readThreadPointer->m_bAutoDelete=FALSE;
	readThreadPointer->ResumeThread();

	
///////// ���M�X���b�h�J�n///////////////////////////////////////////////////////////
	writeThreadPointer = AfxBeginThread(	writeEventListenThread,
											(LPVOID)this,
											THREAD_PRIORITY_NORMAL,
											0,
											CREATE_SUSPENDED
										);

	writeThreadPointer->m_bAutoDelete=FALSE;
	writeThreadPointer->ResumeThread();

	DEBUG_TRACE( TEXT("Thread started\n") );

	/// �t���O�̔��]
	isListeningStopped = false;

	return TRUE;	
}

/// ���j�^�����O�ĊJ
BOOL SerialPort::restartListening(void)
{
	if (!isListeningStopped)
		return FALSE;

	/// �X���b�h�ĊJ
	readThreadPointer->ResumeThread();
	writeThreadPointer->ResumeThread();

	DEBUG_TRACE( TEXT("Thread resumed\n") );

	/// �t���O�̌���
	isListeningStopped = false;

	return TRUE;	
}

/// ���j�^�����O��~
BOOL SerialPort::stopListening(void)
{
	if (isListeningStopped)
		return FALSE;

	/// �X���b�h��~
	readThreadPointer->SuspendThread();
	writeThreadPointer->SuspendThread();

	DEBUG_TRACE( TEXT("Thread suspended\n") );

	/// �t���O�̌���
	isListeningStopped = true;

	return TRUE;
}

/**
 *	����
 *		COM�|�[�g�փf�[�^�𑗐M
 *	����
 *		sendData: �������݃f�[�^�ւ̃|�C���^
 *		bytesToSend: �������݃T�C�Y
 */
void SerialPort::sendData( char* sendData, DWORD bytesToSend)//unsigned char
{
	//sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
	/// COM�|�[�g���J����Ă��邩�̃`�F�b�N
	//assert(commHandle != 0);
	
	/// ������
	memset(writeBuffer, 0, sizeof(writeBuffer));

	/// �f�[�^�R�s�[
	int i;
	for (i=0; i<(int)bytesToSend; i++)
	{
		///Winsock2��sendto��char�^�z��Ȃ̂Œ���
		writeBuffer[i] = sendData[i];//(char)
	}

	/// ���M�o�C�g�����
	bytesToWrite = bytesToSend;

	/// �������݃X���b�h�̂��߂̑��M�C�x���g�Z�b�g
	SetEvent(writeOverLappedStruct.hEvent);

	return;
}

/**
 *	------------------------------------------------------------
 *		SerialPort��protected�ȃ����o�֐�
 *	------------------------------------------------------------
 */

/**	
 *	����
 *		�ʐM�G���[�̎擾
 *	����
 *		�������ݐ�ւ̃|�C���^
 */
void SerialPort::outputProcessErrorMessage(char* errorText)
{
	TCHAR* message;
	TCHAR error[1024];

	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	/// �f�t�H���g����
					(TCHAR*)&message,
					0,
					NULL
					);

	_stprintf(	error, 
				TEXT("WARNING:  %S Failed with the following error: \n%s\nPort: %d\n"), 
				errorText, 
				message, 
				commPortNumber
				);

	MessageBox(NULL, error, TEXT("Serial Port Application Error"), MB_ICONSTOP);

	LocalFree(message);
}









/**
 *	��M�ɗp����X���b�h�֐�
 */
UINT SerialPort::readEventListenThread(LPVOID pUserData)//LPVOID UserData
{
/*	/// ������VOID�|�C���^��SerialPort�N���X�̃|�C���^�փL���X�g
	SerialPort* port = (SerialPort*) pUserData;
	
	/// �X���b�h�������Ă��邱�Ƃ�ʒm���邽�߁C��ԕϐ���TRUE�ɂ���
	port->isReadThreadAlive = TRUE;	
		
	/// ���[�J���ϐ��̐錾
	DWORD		eventMask = 0;
	DWORD		readThreadEvents = 0;
	DWORD		readEventMask = 0;
	DWORD		commErrors = 0;
	BOOL		isEventOccurred = TRUE;
	COMSTAT		comStat;

	/// �|�[�g���J����Ă��邩�`�F�b�N���āC�N�����ɒʐM�o�b�t�@���N���A
	if (port->commHandle)
		PurgeComm(port->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	//
	 //	�������[�v
	 //	���̃��[�v�̓X���b�h�������Ă������p��
	 //
	while (1) 
	{ 
		//
		 //	WaitCommEvent�֐��̌Ăяo��
		 //		�������C�ʐM�|�[�g��񓯊��ŊJ�������߂����ɏI������
		 //
		 //		���̌Ăяo���ɂ��CoverLappedStruct�̃����o�ł���hEvent�ireadEventHandles[1]�j��
		 //		�ǂݍ��݉\��byte�f�[�^���Ȃ���΃��Z�b�g��ԂɁC�ǂݍ��݉\�ȃf�[�^������΃Z�b�g��Ԃɂ���
		 //
		 //		���̃C�x���g�n���h�������Z�b�g��Ԃł���ƁC�ʐM�|�[�g�Ƀf�[�^���������ɃZ�b�g��ԂɂȂ�
		 //		�Ȃ��C���̎���OverLapped�\���̂Ɏ蓮���Z�b�g�C�x���g�̃n���h�����w�肵�Ă����K�v������
		 //
		isEventOccurred = WaitCommEvent(port->commHandle, &eventMask, &port->readOverLappedStruct);

		/// �ȉ��Ŏ�M�����������ǂ����̊m�F
		if ( !isEventOccurred )	/// ��M�C�x���g���Ȃ�������
		{
			 //
			 //	WaitCommEvent�֐���FALSE��Ԃ����ꍇ�C�v���Z�X�ɂ��G���[�����肳���
			 //	�ȉ��ł̂��̃G���[�̓�����s��
			 //
			switch (commErrors = GetLastError()) 
			{ 
				case ERROR_IO_PENDING: 	
				{ 
					/// ���̃G���[�̓|�[�g�Ƀf�[�^���Ȃ��ꍇ�̒ʏ�̖߂�l
					/// ���ɉ��������ɑ�����
					break;
				}
				case 87:
				{
					/// Windows NT�n�̏ꍇ�C�����̗��R�ŕԋp����邱�Ƃ�����
					/// ���������ɖ��Ȃ��̂Ōp����
					break;
				}
				default:
				{
					/// ���̑S�ẴG���[�F�[���ȃG���[���N�������Ƃ�����
					/// ���������Ă��̃G���[����������
					port->outputProcessErrorMessage( "WaitCommEvent()" );
					break;
				}
			}	/// switch ( commErrors = GetLastError() ) 
		}	/// end if isEventOccurred
*/
		/**
		 *	��M�ҋ@�֐�
		 *	���̊֐��ɂ��1�̃C�x���g����������܂ŃX���b�h���ʏ�u���b�N�����
		 */
		/**
		 *	DWORD WaitForMultipleObjects(
		 *				DWORD  nCount,				/// �I�u�W�F�N�g�̐�
		 *				CONST HANDLE *pHandles,		/// �n���h���̔z��
		 *				BOOL   bWaitAll,			/// �ҋ@�C�x���g�̃V�O�i����Ԃ�1�����ׂđ҂��̃t���O
		 *				DWORD  dwMilliseconds		/// �^�C���A�E�g����
		 *				);
		 */
		//readThreadEvents = WaitForMultipleObjects(2, port->readEventHandles, FALSE, INFINITE);

		/**
		 *	WaitCommEvent�֐��ɂ��C�x���g�����m�����ꍇ
		 *	���ۂɃo�b�t�@�Ƀf�[�^�����邩�`�F�b�N����
		 */
		/**
		 *	ClearCommError()
		 *		�G���[���N���A���邱�Ƃ��ړI�ł���̂ŁC�{���̖ړI�Ƃ͈قȂ�D
		 *		�������C���̊֐��ɂ��Ԃ����\����COMSTAT�ɁC
		 *		��M�o�b�t�@���̃f�[�^��������킷�����ocbInQue ������̂�, 
		 *		���̊֐����Ăяo���C��M�f�[�^�����擾����
		 */
/*		isEventOccurred = ClearCommError(port->commHandle, &commErrors, &comStat);

		switch (readThreadEvents)
		{
			case WAIT_OBJECT_0:	/// �X���b�h�V���b�g�_�E���C�x���g
			{
				/// ���̃C�x���g�͍ŗD��ł���ŏ��ɒ񋟂����
			 	port->isReadThreadAlive = FALSE;
				
				/// �X���b�h�j��
				AfxEndThread(100);
				
				break;
			}
		
			case WAIT_OBJECT_0 + 1:	/// �ǂݍ��݃C�x���g
			{
				/// ��M�o�b�t�@���m�F���āC�f�[�^���Ȃ���Ζ������[�v�̍ŏ��ɖ߂�
				if ( comStat.cbInQue == 0 )
					continue;

				/// COM�|�[�g�̃C�x���g���擾����
				GetCommMask(port->commHandle, &readEventMask);

				//
				 //	��M�ȊO�̃C�x���g���N�����Ă�����܂���������
				 //
				/// CTS�i���M�j�M���̏�Ԃ��ω�
				if ( readEventMask & EV_CTS && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_CTS_DETECTED, 
										(WPARAM) 0, 
										(LPARAM) port->commPortNumber);

				/// ���͏�Ńu���[�N�����o
				if ( readEventMask & EV_BREAK && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_BREAK_DETECTED, 
										(WPARAM) 2, 
										(LPARAM) port->commPortNumber);

				/// �����ԃG���[�������iCE_FRAME�ACE_OVERRUN�ACE_RXPARITY�j
				if ( readEventMask & EV_ERR && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_ERR_DETECTED, 
										(WPARAM) 3, 
										(LPARAM) port->commPortNumber);

				/// �Ăяo���M�������o
				if ( readEventMask & EV_RING && port->commOwnerWindowHandle != NULL)
					::SendMessage(port->commOwnerWindowHandle, 
										WM_COMM_RING_DETECTED, 
										(WPARAM) 4, 
										(LPARAM) port->commPortNumber);

				/// COM�|�[�g���當����f�[�^����M
				if ( readEventMask & EV_RXCHAR )
				{
					readData(port, comStat);//�������������ďo��
				}

				break;
			}
			default:
				break;
		} /// end of switch

		/// ��M�o�b�t�@�̃f�[�^�ǂݍ��݂��I��������_�ŃC�x���g���Z�b�g
		ResetEvent(port->readOverLappedStruct.hEvent);

	} /// �������[�v
	*/
	return 0;
	
}

/**
 *	��M�X���b�h�ŗp�����M�o�b�t�@����̓ǂݏo���֐�
 */
void SerialPort::readData(SerialPort* port, COMSTAT& comStat)
{
	BOOL	isReadingToTry		= TRUE;		/// �ǂݍ��݂��s�����ǂ���
	BOOL	isReadingCompleted	= TRUE;		/// �ǂݍ��݂������������ǂ���
	BOOL	isReadingSucceeded	= TRUE;		/// �ǂݍ��݂������������ǂ���
	DWORD	errorCode				= 0;		/// �G���[
	DWORD	bytesRead				= 0;		/// �ǂݍ��񂾃o�C�g��
	int i;										/// �J�E���^			

	
	/////////////////////UDP��M////////////////////////////////////
	///�\�P�b�g�쐬
/*	port->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);

	assert(port->WSAHandle =! 0);
	
	
	port->addr.sin_family	= AF_INET;									///�A�h���X�t�@�~��
	port->addr.sin_port		= htons(port->remort_portNo);//htons(10003);//					///����̃|�[�g�ԍ�
	port->addr.sin_addr.S_un.S_addr = inet_addr(port->remort_IPAdress);//inet_addr("192.168.0.169");//	///�����IP�A�h���X
	
		
	bind(port->sockHandle, (struct sockaddr *)&(port->addr), sizeof(port->addr));
	
	while(1){
	
	///�\�P�b�g�̃f�[�^����M
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
	 *	�������[�v�J�n
	 *	�ǂݍ��ނׂ��S�Ẵf�[�^�������I�����ď��߂ă��[�v�𔲂���
	 */
	while (1)
	{ 

		ZeroMemory(port->readBuffer, MAX_BUFFER_SIZE);//�������[�̒��g��0�ɂ���

		/**
		 *	�r�������J�n
		 *	COM�|�[�g�̏��L�����擾����
		 */ 
		port->criticalSection.lock();

		/**
		 *	ClearCommError()�ɂ��COMSTAT�\���̂��X�V�����̃G���[������
		 */
		isReadingSucceeded = ClearCommError(port->commHandle, &errorCode, &comStat);
		port->bytesToRead = comStat.cbInQue;

		/// �r�������I��
		port->criticalSection.unlock();

		/// �ǂݍ��ނׂ��f�[�^���邩�ǂ����m�F���C�Ȃ������烋�[�v���甲����
		if (port->bytesToRead == 0)
		{
			break;
		}
		else if (port->bytesToRead > 0)
		{
			isReadingToTry = TRUE;

			/// �ǂݍ������Ƃ���o�b�t�@�T�C�Y���ő�l���傫��������
			if ( port->bytesToRead > port->readBufferSize )
				port->bytesToRead = port->readBufferSize;
		}

		/**
		 *	�r�������J�n
		 *	�ǂݍ��ޑO��COM�|�[�g�̏��L�����擾����
		 */
		port->criticalSection.lock();
		
		/// �f�[�^�ǂݍ��݂��s���ꍇ
		if (isReadingToTry)
		{
			/// �ϐ��̏�����
			port->readOverLappedStruct.Offset = 0;
			port->readOverLappedStruct.OffsetHigh = 0;

			//�|�[�g����f�[�^��ǂݍ���
			isReadingSucceeded = ReadFile(	
											port->commHandle,					/// �Ώ�COM�|�[�g�ւ̃n���h�� 
											port->readBuffer,					/// ��M�o�b�t�@�ւ̃|�C���^
											port->bytesToRead,					/// �ǂݍ������Ƃ���o�C�g��
											&bytesRead,							/// ���ۂɓǂݍ��񂾃o�C�g��
											&port->readOverLappedStruct		/// ������������OVERLAPPED�\���̂ւ̃|�C���^
										);

			/// �ǂݍ��݃G���[���o���ꍇ�̏��� 
			if (!isReadingSucceeded)  
			{ 
				switch ( errorCode = GetLastError() )
				{
					case ERROR_IO_PENDING: 	
					{ 
						/**
						 *	�񓯊����o�́iReadFile()�j���܂��������ł���
						 *	���������āCGetOverlappedResults()�Ŏ��㏈�����s��
						 */
						isReadingCompleted = FALSE;
						break;
					}
					default:
					{
						/// �\�����Ȃ��G���[�Ȃ̂ŁC���̃G���[����
						port->outputProcessErrorMessage( "ReadFile()" );
						break;
					} 
				}	/// end of switch
			}
			else
			{
				/// ReadFile()���I�������ꍇ
				/// ���̏ꍇGetOverlappedResults()���Ăяo���K�v�͖���
				isReadingCompleted = TRUE;
			}	/// close if ( !isReadingSuccessed )

		}	/// close if ( isReadingToTry )

		/**
		 *	ReadFile()���I�����Ȃ������̂�GetOverlappedResult()�ɂ��ǂݍ��ݓ��������������
		 */
		if (!isReadingCompleted)
		{
			/**
			 *	GetOverlappedResult
			 *		��P�����F�t�@�C���n���h��
			 *		��Q�����FOVERLAPPED�\���̂̃A�h���X
			 *		��R�����F�����ς݂̃o�C�g�����󂯎��ϐ��̃A�h���X
			 *		��S�����F�t���O
			 *
			 *	�񓯊�I/O���܂��������Ă��Ȃ������ꍇ�C��4�����̃t���O��
			 *	TRUE���Ɗ�������܂Ŋ֐��̓����őҋ@���CFALSE���Ƒ҂����Ɋ֐�����߂�D
			 */
			isReadingSucceeded = GetOverlappedResult(
														port->commHandle,				/// COM�|�[�g�n���h�� 
														&port->readOverLappedStruct,	/// ������������Overlapped�\����
														&bytesRead,						/// �ǂݍ��񂾃o�C�g��
														TRUE							/// �ҋ@�t���O
													);

			/// �G���[�R�[�h���� 
			if ( !isReadingSucceeded )  
			{
				port->outputProcessErrorMessage( "GetOverlappedResults() in ReadFile()" );
			}

		}  /// close if ( !isReadingCompleted )

		/// �ꎞ��M�o�b�t�@����L�^�p�����O�o�b�t�@�ɕۑ�
		for (i=0;i<(int)bytesRead;i++)
			port->receiveBuffer.get(&port->readBuffer[i]);

		/**
		 *	�r�������I��
		 *	�ǂݍ��ޏI������̂�COM�|�[�g�̏��L�������
		 */
		port->criticalSection.unlock();

		/**
		 *	���L���������Ă�e�E�B���h�E�֎�M��ʒm
		 *	�ʒm�p�����[�^�F
		 *		wParam�F�ǂݍ��݃f�[�^
		 *		lParam�FCOM�|�[�g�ԍ�	
		 */
		/// �I�[�i�[�E�B���h�E�փ|�X�g
		if (port->commOwnerWindowHandle != NULL) 
			::PostMessage(
							port->commOwnerWindowHandle, 
							WM_COMM_RXCHAR, 
							(WPARAM)(port->readBuffer), 
							MAKELPARAM( (WORD)port->commPortNumber, (WORD)bytesRead )
						);
	}	/// �������[�v�̏I���
}


/**
 *	���M�ɗp����X���b�h�֐�
 */
UINT SerialPort::writeEventListenThread(LPVOID pParam)//����֐�
{
	/// ������VOID�|�C���^��SerialPort�N���X�̃|�C���^�փL���X�g
	SerialPort* port = (SerialPort*) pParam;
	
	/// �X���b�h�������Ă��邱�Ƃ�ʒm���邽�߁C��ԕϐ���TRUE�ɂ���
	port->isWriteThreadAlive = TRUE;

	/// ���[�J���ϐ��̐錾
//	DWORD		eventMask = 0;
//	DWORD		writeEventMask = 0;
	DWORD		writeThreadEvents = 0;
//	DWORD		writeEvents = 0;
//	DWORD		commErrors = 0;
//	BOOL		isEventOccurred = TRUE;
	COMSTAT		comStat;

	/// �|�[�g���J����Ă��邩�`�F�b�N���āC�N�����ɒʐM�o�b�t�@���N���A
	if (port->commHandle)
		PurgeComm(port->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/**
	 *	 �������[�v
	 *	���̃��[�v�̓X���b�h�������Ă������p��
	 */
	while (1) 
	{
		/// WaitForMultipleObjects()�ŃC�x���g�҂�
		writeThreadEvents = WaitForMultipleObjects(2, port->writeEventHandles, FALSE, INFINITE);

		switch (writeThreadEvents)
		{
			case WAIT_OBJECT_0:	/// �X���b�h�V���b�g�_�E���C�x���g
			{
				/// ���̃C�x���g�͍ŗD��ł���ŏ��ɒ񋟂����
			 	port->isWriteThreadAlive = FALSE;

				/// �X���b�h�j��
				AfxEndThread(200);
				break;
			}
			case WAIT_OBJECT_0 + 1:	/// �������݃C�x���g
			{
				/// �|�[�g�փf�[�^��������
				writeData(port, comStat);

				break;
			}
			default:
				break;
		}	/// end of switch (writeThreadEvents)

	}	/// �������[�v

	return 0;
}

/**
 *	���M�X���b�h�ŗp���鑗�M�o�b�t�@�ւ̏������݊֐�
 */
void SerialPort::writeData(SerialPort* port, COMSTAT& comStat)
{
	/// ���[�J���ϐ��̐錾
	BOOL	isWritingToTry		= TRUE;		/// �������݂��s�����ǂ���
	BOOL	isWritingCompleted	= TRUE;		/// �������݂������������ǂ���
	BOOL	isWritingSucceeded	= TRUE;		/// �������݂������������ǂ���
	DWORD	errorCode			= 0;		/// �G���[�R�[�h
	DWORD	bytesWritten		= 0;
	char	ErrMassage[32]		= {0};		///�G���[�R�[�h�\���p

	///�\�P�b�g�쐬
	port->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
	//assert(port->sockHandle =! -1);
	assert(port->WSAHandle =! 0);

	//TEST_BUFFER
	char Txbuffer[8]={0};
		///�Q�[�g�E�F�CID
		Txbuffer[0]	= (char)(0x00);
		Txbuffer[1]	= (char)(0x0A);
	int length2		= sizeof(Txbuffer);

	/// �ҋ@�C�x���g�����Z�b�g
	ResetEvent(port->writeOverLappedStruct.hEvent);

	port->criticalSection.lock();

	/**
	 *	ClearCommError()�ɂ��COMSTAT�\���̂��X�V�����̃G���[������
	 */
	isWritingSucceeded = ClearCommError(port->commHandle, &errorCode, &comStat);

	//SOCKET sock2;
	//struct sockaddr_in addr2;
	//sock2 = socket(AF_INET, SOCK_DGRAM, 0);	//�\�P�b�g�쐬
/*	
	if(sock2 == INVALID_SOCKET){
		sprintf(ErrMassage,"error : %d\n", WSAGetLastError());
		port->outputProcessErrorMessage( ErrMassage );
		assert(sock2 =! INVALID_SOCKET);
	}
	addr2.sin_family = AF_INET;
	addr2.sin_port = htons(10003);	//���M��|�[�g�ԍ�
	addr2.sin_addr.S_un.S_addr = inet_addr("192.168.0.169");	//���M��IP�A�h���X
*/
	//isWritingSucceeded =sendto(sock2, Txbuffer, length2, 0, (struct sockaddr *)&addr2, sizeof(addr2));
	
	
	//closesocket(sock2);

	/// �������ނׂ��f�[�^���邩�ǂ����m�F���A�Ȃ��ꍇ������������ <--�V���A���p?
/*	if (comStat.cbOutQue > 0)
	{
		port->criticalSection.unlock();
		return;
	}
*/		///���M��̃l�b�g���[�N��������
		///�|�[�gIP�A�h���X�ԍ�,IP�A�h���X�̕����X�g�����O���l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ�
		port->addr.sin_family	= AF_INET;									///�A�h���X�t�@�~��
		port->addr.sin_port		= htons(port->remort_portNo);//htons(10003);//					///����̃|�[�g�ԍ�
		port->addr.sin_addr.S_un.S_addr = inet_addr(port->remort_IPAdress);//inet_addr("192.168.0.169");//	///�����IP�A�h���X

		

		///�\�P�b�g�̃f�[�^�𑗐M
		isWritingSucceeded =sendto( port->sockHandle, 
									port->writeBuffer,//Txbuffer,
									port->bytesToWrite,//length2,
									0, 
									(struct sockaddr *)&port->addr, 
									sizeof(port->addr));
/*
		isWritingSucceeded = sendto(port->sockHandle,							///�\�P�b�g�L�q�q
									port->writeBuffer,					///���M���郁�b�Z�[�W�������Ă���o�b�t�@�[�ւ̃|�C���^�[
									port->bytesToWrite,					///�o�b�t�@�[���̃��b�Z�[�W�̒���
									0,									///�t���O�̐ݒ�
									(struct sockaddr *)&port->addr,		///�A�h���X�ꎮ
									sizeof(port->addr));						///�A�h���X�̃T�C�Y
*/	
	
/*
	if (isWritingToTry)
	{

		/// �ϐ��̏�����
		port->writeOverLappedStruct.Offset = 0;
		port->writeOverLappedStruct.OffsetHigh = 0;

		isWritingSucceeded = WriteFile(
										port->commHandle,				/// COM�|�[�g�ւ̃n���h��
										port->writeBuffer,				/// �������݃o�b�t�@�ւ̃|�C���^
										port->bytesToWrite,				/// �������ރf�[�^����
										&bytesWritten,					/// ���ۂɏ������񂾃o�C�g�����L�^����|�C���^
										&port->writeOverLappedStruct	/// �������ݗp��OVERLAPPED�\����
										);
		
		/// �G���[���̏���
		assert(isWritingSucceeded =! -1);	
	
		if (isWritingSucceeded == -1)  
		{
			/// �G���[�R�[�h�擾

			errorCode = errno ;///GetLastError();
			
			switch (errorCode)
			{	
				case ERROR_IO_PENDING:
				{
						/// WriteFile()�������ɏI�����Ȃ������ꍇ�̒ʏ�̕Ԃ�l�Ȃ̂�GetOverlappedResults()�ֈڍs
						bytesWritten = 0;

						/// �������ݏI���t���O��OFF
						isWritingCompleted = FALSE;
						break;
				}				
	
				default:
				{
						/// ���̑S�ẴG���[
						sprintf(ErrMassage,"sendto Err code:%4d",errorCode);
						port->outputProcessErrorMessage( ErrMassage );
						port->criticalSection.unlock();
				}
			}	/// end of switch
		}
		/// �ϐ��̏�����
		port->writeOverLappedStruct.Offset = 0;
		port->writeOverLappedStruct.OffsetHigh = 0;

		///���M�֐�
		isWritingSucceeded = WriteFile(
										port->commHandle,				/// COM�|�[�g�ւ̃n���h��
										port->writeBuffer,				/// �������݃o�b�t�@�ւ̃|�C���^
										port->bytesToWrite,				/// �������ރf�[�^����
										&bytesWritten,					/// ���ۂɏ������񂾃o�C�g�����L�^����|�C���^
										&port->writeOverLappedStruct	/// �������ݗp��OVERLAPPED�\����
										);

		/// �G���[���̏���
		if (!isWritingSucceeded)  
		{
			/// �G���[�R�[�h�擾
			errorCode = GetLastError();
			
			switch (errorCode)
			{
				case ERROR_IO_PENDING:
				{
						/// WriteFile()�������ɏI�����Ȃ������ꍇ�̒ʏ�̕Ԃ�l�Ȃ̂�GetOverlappedResults()�ֈڍs
						bytesWritten = 0;

						/// �������ݏI���t���O��OFF
						isWritingCompleted = FALSE;
						break;
				}
				default:
				{
						/// ���̑S�ẴG���[
						port->outputProcessErrorMessage( "WriteFile()" );
						port->criticalSection.unlock();
				}
			}	/// end of switch
		} 
		else
		{
			port->criticalSection.unlock();
		}	/// end of if (!isWritingSuccessed)
		
	} // end of if( isWritingToTry )
*/
/*
	/// �������݂���莞�ԂŏI�����Ȃ�������
	if (!isWritingCompleted)
	{
		
		 //	GetOverlappedResult
		 //		��P�����F�t�@�C���n���h��
		 //		��Q�����FOVERLAPPED�\���̂̃A�h���X
		 //		��R�����F�����ς݂̃o�C�g�����󂯎��ϐ��̃A�h���X
		 //		��S�����F�t���O
		 //
		 //	�񓯊�I/O���܂��������Ă��Ȃ������ꍇ�C��4�����̃t���O��
		 //	TRUE���Ɗ�������܂Ŋ֐��̓����őҋ@���CFALSE���Ƒ҂����Ɋ֐�����߂�D
		
		isWritingSucceeded = GetOverlappedResult(
													port->commHandle,				/// COM�|�[�g�ւ̃n���h�� 
													&port->writeOverLappedStruct,	/// OVERLAPPED�\����
													&bytesWritten,					/// �������񂾃o�C�g��
													TRUE							/// �ҋ@�t���O�i�������ݏI���܂őҋ@����j
												);

		port->criticalSection.unlock();

		/// GetOverlappedResult()�ɂ�菑�����ݏI��
		isWritingCompleted = TRUE;
		
		/// �G���[�R�[�h����
		if (!isWritingSucceeded)  
		{
			port->outputProcessErrorMessage( "GetOverlappedResults() in WriteFile()" );
		}

	} // end if ( !isWritingCompleted )

	/// �������ޗ\�肾�����o�C�g���Ǝ��ۂɏ������񂾃o�C�g���̃`�F�b�N
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
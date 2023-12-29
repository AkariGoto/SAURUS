
/**
 *  �t�@�C����
 *		UdpSocket.cpp
 *  ����
 *		UDP�ʐM�Ɋւ���N���X
 *  ���t
 *		�쐬��: 2019/11/19	�X�V��: 2019/11/19
 */

#include"UdpSocket.h"

//
#include <cassert>
#include <process.h>
#include "Comm\SerialPort.h"
#include "..\System\DebugOutput.h"

//�ǉ�
#include <winsock2.h>



namespace UDP
{

//�R���X�g���N�^
UdpSocket::UdpSocket()
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
	sprintf(local_IPAdress,"192.168.0.18");
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
	UDPreadThreadTerminateHandle = NULL;
	UDPwriteThreadTerminateHandle = NULL;

	/// �X���b�h�t���O
	UDPisReadThreadAlive = FALSE;
	UDPisWriteThreadAlive = FALSE;

/**
 *	�t���O�̏�����
 */
	UDPisListeningStopped = true;
	UDPisCommOpen = false;








}

//�f�X�g���N�^
UdpSocket::~UdpSocket(){
	//�f�X�g���N�^����Socket�N���X��j������Ƃ��ɌĂ�
}

//-----------------------------------------------------------------------------------------
//������(�\�P�b�g������肢�낢��)
//-----------------------------------------------------------------------------------------
int UdpSocket::openUDP(	HWND hOwnerWnd,				/// ��M�|�[�g�̐e�E�B���h�E�n���h��
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
	//�\�P�b�g�ʐMwinsock�̗����グ
	///winsock������
	WSAHandle = WSAStartup(MAKEWORD(2,0), &wsaData);///Ver.2.0

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
	}

///�\�P�b�g�쐬
	sockHandle = socket(AF_INET, SOCK_DGRAM, 0);
	//AF_INET��IPv4�ASOCK_DGRAM��UDP�ʐM�A0�́H
	
	
	///�G���[����
	///���s������socket��-1��Ԃ�
	if(sockHandle =! -1){
		#define SOCK_HANDLE_VALUE ((HANDLE)(2))
		commHandle = SOCK_HANDLE_VALUE;///<---�n���h���̒l���킩��Ȃ�
	}                                   //���������sockHandle�����H�H
	///�G���[
	else if (sockHandle == -1){//INVALID_SOCKET
		///�G���[�l��\�� (VC++�G���[���b�N�A�b�v�ɃG���[�l����͂��Ċm�F���Ă�������)
		DEBUG_TRACE(TEXT("socket error : %d\n", WSAGetLastError()));
		assert(sockHandle=! -1);
        return FALSE;
	}











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









}






/***********************************************************************
                 //  �X���b�h�J�n�E��~�E�I��
*************************************************************************/

//���j�^�����O�J�n
BOOL UdpSocket::UDPstartListening(void)
{
///////////////////  UDP��M�X���b�h�J�n  //////////////////////////
	UDPreadThreadPointer = AfxBeginThread(	UDPreadEventListenThread,    //����֐�
										(LPVOID)this,             //����֐��ɓn���p�����[�^
										THREAD_PRIORITY_NORMAL,   //�X���b�h�̗D�揇��
										0,                        //�쐬����ɃX���b�h���J�n
										CREATE_SUSPENDED
										);
	UDPreadThreadPointer->m_bAutoDelete=FALSE;
	UDPreadThreadPointer->ResumeThread();


//////////////////  UDP���M�X���b�h�J�n  ///////////////////////////
	UDPwriteThreadPointer = AfxBeginThread(	UDPwriteEventListenThread,
											(LPVOID)this,
											THREAD_PRIORITY_NORMAL,
											0,
											CREATE_SUSPENDED
										);

	UDPwriteThreadPointer->m_bAutoDelete=FALSE;
	UDPwriteThreadPointer->ResumeThread();

	DEBUG_TRACE( TEXT("Thread started\n") );

	/// �t���O�̔��]
	UDPisListeningStopped = false;

	return TRUE;	
}

/// ���j�^�����O�ĊJ
BOOL UdpSocket::UDPrestartListening(void)
{
	//�Ȃ񂩂���
	if (!UDPisListeningStopped)
		return FALSE;

	/// �X���b�h�ĊJ
	UDPreadThreadPointer->ResumeThread();
	UDPwriteThreadPointer->ResumeThread();

	DEBUG_TRACE( TEXT("Thread resumed\n") );

	/// �t���O�̌���
	UDPisListeningStopped = false;

	return TRUE;	
}

/// ���j�^�����O��~
BOOL UdpSocket::UDPstopListening(void)
{
	//�Ȃ񂩂���
	if (UDPisListeningStopped)
		return FALSE;

	/// �X���b�h��~
	UDPreadThreadPointer->SuspendThread();
	UDPwriteThreadPointer->SuspendThread();

	DEBUG_TRACE( TEXT("Thread suspended\n") );

	/// �t���O�̌���
	UDPisListeningStopped = true;

	return TRUE;
}






/**
 *	����
 *		COM�|�[�g�փf�[�^�𑗐M
 *	����
 *		sendData: �������݃f�[�^�ւ̃|�C���^
 *		bytesToSend: �������݃T�C�Y
 */
void UdpSocket::sendData( char* sendData, DWORD bytesToSend)//unsigned char
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


void UdpSocket::receiveData(char* receiveData, DWORD bytesToReceive)
{

}




//////////////////////  �ʐM�G���[�̎擾  ////////////////////////////////////////////
void UdpSocket::outputProcessErrorMessage(char* errorText)
{

}



///////////////////  UDP��M�ɗp����X���b�h�֐�  ////////////////////////////////////
UINT UdpSocket::UDPreadEventListenThread(LPVOID pUserData)
{
	/// ������VOID�|�C���^��SerialPort�N���X�̃|�C���^�փL���X�g
	UdpSocket* udpsocket = (UdpSocket*) pUserData;

	/// �X���b�h�������Ă��邱�Ƃ�ʒm���邽�߁C��ԕϐ���TRUE�ɂ���
	udpsocket->UDPisReadThreadAlive = TRUE;	
		
	/// ���[�J���ϐ��̐錾
	DWORD		eventMask = 0;
	DWORD		readThreadEvents = 0;
	DWORD		readEventMask = 0;
	DWORD		commErrors = 0;
	BOOL		isEventOccurred = TRUE;
	COMSTAT		comStat;

	/// �|�[�g���J����Ă��邩�`�F�b�N���āC�N�����ɒʐM�o�b�t�@���N���A
	if (udpsocket->commHandle)
		PurgeComm(udpsocket->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

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
		isEventOccurred = WaitCommEvent(udpsocket->commHandle, &eventMask, &udpsocket->readOverLappedStruct);

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
					udpsocket->outputProcessErrorMessage( "WaitCommEvent()" );
					break;
				}
			}	/// switch ( commErrors = GetLastError() ) 
		}	/// end if isEventOccurred

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
		isEventOccurred = ClearCommError(udpsocket->commHandle, &commErrors, &comStat);

		switch (readThreadEvents)
		{
			case WAIT_OBJECT_0:	/// �X���b�h�V���b�g�_�E���C�x���g
			{
				/// ���̃C�x���g�͍ŗD��ł���ŏ��ɒ񋟂����
			 	udpsocket->UDPisReadThreadAlive = FALSE;
				
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
				GetCommMask(udpsocket->commHandle, &readEventMask);

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
					UDPreadData(udpsocket, comStat);//�������������ďo��
				}

				break;
			}
			default:
				break;
		} /// end of switch

		/// ��M�o�b�t�@�̃f�[�^�ǂݍ��݂��I��������_�ŃC�x���g���Z�b�g
		ResetEvent(port->readOverLappedStruct.hEvent);

	} /// �������[�v
	
	return 0;
}

///////////  UDP��M�X���b�h�ŗp�����M�o�b�t�@����̓ǂݏo���֐�  ///////////////////
void UdpSocket::UDPreadData(UdpSocket* udpsocket, COMSTAT& comsStat)
{
	BOOL	UDPisReadingToTry		= TRUE;		/// �ǂݍ��݂��s�����ǂ���
	BOOL	UDPisReadingCompleted	= TRUE;		/// �ǂݍ��݂������������ǂ���
	BOOL	UDPisReadingSucceeded	= TRUE;		/// �ǂݍ��݂������������ǂ���
	DWORD	UDPerrorCode				= 0;		/// �G���[
	DWORD	UDPbytesRead				= 0;		/// �ǂݍ��񂾃o�C�g��
	int i=0;										/// �J�E���^		

	///�\�P�b�g�쐬
	//udpsocket->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);

	udpsocket->addr.sin_family	= AF_INET;									///�A�h���X�t�@�~��
	udpsocket->addr.sin_port	= htons(udpsocket->remort_portNo);//htons(10003);//					///����̃|�[�g�ԍ�
	udpsocket->addr.sin_addr.S_un.S_addr = inet_addr(udpsocket->remort_IPAdress);//inet_addr("192.168.0.169");//	///�����IP�A�h���X


	//�|�[�g
	//bind���ĂȂ��ꍇ��recvfrom���g��
	bind(udpsocket->sockHandle, (struct sockaddr *)&(udpsocket->addr), sizeof(udpsocket->addr));

	///�\�P�b�g�̃f�[�^����M
	UDPisReadingSucceeded =recvfrom( udpsocket->sockHandle, 
									udpsocket->readBuffer,//Rxbuffer,
									udpsocket->bytesToRead,//length2,
									0, 
									(struct sockaddr *)&udpsocket->addr, 
									sizeof(udpsocket->addr));



}





///////////////////  UDP���M�ɗp����X���b�h�֐�  /////////////////////////////////////
UINT UdpSocket::UDPwriteEventListenThread(LPVOID pParam)
{
	/// ������VOID�|�C���^��SerialPort�N���X�̃|�C���^�փL���X�g
	UdpSocket* udpsocket = (UdpSocket*) pParam ;

	/// �X���b�h�������Ă��邱�Ƃ�ʒm���邽�߁C��ԕϐ���TRUE�ɂ���
	udpsocket->UDPisWriteThreadAlive = TRUE ;

	/// ���[�J���ϐ��̐錾
//	DWORD		eventMask = 0;
//	DWORD		writeEventMask = 0;
	DWORD		writeThreadEvents = 0;
//	DWORD		writeEvents = 0;
//	DWORD		commErrors = 0;
//	BOOL		isEventOccurred = TRUE;
	COMSTAT		comStat;

	/// �|�[�g���J����Ă��邩�`�F�b�N���āC�N�����ɒʐM�o�b�t�@���N���A
	if (udpsocket->commHandle)
		PurgeComm(udpsocket->commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	/**
	 *	 �������[�v
	 *	���̃��[�v�̓X���b�h�������Ă������p��
	 */
	while (1) 
	{
		/// WaitForMultipleObjects()�ŃC�x���g�҂�
		writeThreadEvents = WaitForMultipleObjects(2, udpsocket->writeEventHandles, FALSE, INFINITE);

		switch (writeThreadEvents)
		{
			case WAIT_OBJECT_0:	/// �X���b�h�V���b�g�_�E���C�x���g
			{
				/// ���̃C�x���g�͍ŗD��ł���ŏ��ɒ񋟂����
			 	udpsocket->UDPisWriteThreadAlive = FALSE;

				/// �X���b�h�j��
				AfxEndThread(200);
				break;
			}
			case WAIT_OBJECT_0 + 1:	/// �������݃C�x���g
			{
				/// �|�[�g�փf�[�^��������
				UDPwriteData(udpsocket, comStat);

				break;
			}
			default:
				break;
		}	/// end of switch (writeThreadEvents)

	}	/// �������[�v

	return 0;
}


///////////  UDP���M�X���b�h�ŗp���鑗�M�o�b�t�@����̏������݊֐�  ///////////////////
void UdpSocket::UDPwriteData(UdpSocket* udpsocket, COMSTAT& comStat)
{
	/// ���[�J���ϐ��̐錾
	BOOL	UDPisWritingToTry		= TRUE;		/// �������݂��s�����ǂ���
	BOOL	UDPisWritingCompleted	= TRUE;		/// �������݂������������ǂ���
	BOOL	UDPisWritingSucceeded	= TRUE;		/// �������݂������������ǂ���
	DWORD	UDPerrorCode			= 0;		/// �G���[�R�[�h
	DWORD	UDPbytesWritten		    = 0;
	char	UDPErrMassage[32]		= {0};		///�G���[�R�[�h�\���p

	///�\�P�b�g�쐬
	udpsocket->sockHandle = socket(AF_INET, SOCK_DGRAM, 0);

	assert(udpsocket->WSAHandle =! 0);

	/// �ҋ@�C�x���g�����Z�b�g
	ResetEvent(udpsocket->writeOverLappedStruct.hEvent);

	udpsocket->criticalSection.lock();

	/**
	 *	ClearCommError()�ɂ��COMSTAT�\���̂��X�V�����̃G���[������
	 */
	UDPisWritingSucceeded = ClearCommError(udpsocket->commHandle, &UDPerrorCode, &comStat);

    udpsocket->addr.sin_family	= AF_INET;									///�A�h���X�t�@�~��
	udpsocket->addr.sin_port		= htons(udpsocket->remort_portNo);//htons(10003);//					///����̃|�[�g�ԍ�
	udpsocket->addr.sin_addr.S_un.S_addr = inet_addr(udpsocket->remort_IPAdress);//inet_addr("192.168.0.169");//	///�����IP�A�h���X

	///�\�P�b�g�̃f�[�^�𑗐M
	UDPisWritingSucceeded =sendto( udpsocket->sockHandle, 
									udpsocket->writeBuffer,//Txbuffer,
									udpsocket->bytesToWrite,//length2,
									0, 
									(struct sockaddr *)&udpsocket->addr, 
									sizeof(udpsocket->addr));






}

























//-----------------------------------------------------------------------------------------//
//�\�P�b�g����
//
//----------------------------------------------------------------------------------------//

/*void UdpSocket::close(void)
{
	///�\�P�b�g�����
	closesocket(sockHandle);
	///winsock�I��
	WSACleanup();

    if (sock != INVALID_SOCKET) {
        closesocket(sockHandle);
        sock = INVALID_SOCKET;
    }
}*/
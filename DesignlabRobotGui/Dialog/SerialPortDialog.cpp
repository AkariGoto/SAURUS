/**
 *  �t�@�C����
 *		SerialPortDialog.cpp
 *  ����
 *		�V���A���ʐM�̃��O�\���̂��߂̃_�C�A���O
 *  ���t
 *		�쐬��: 2007/06/11(Mon)		�X�V��: 2007/11/10(Sat)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
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
//�ǉ�
using namespace UDP;

unsigned char changeflag=0x0F;//�N���[�����
#define BIT(num)                 ((unsigned int)1 << (num))
char LED=0;
long LEDD;
long OV=0;
char LLED=0;
long LLEEDD=0;
/**
 *	----------------------------------------------------------------------
 *		CSerialPortDialog�N���X
 *	----------------------------------------------------------------------
 */

/**
 *		�t���[�����[�N�����s���ɃN���X����N���X�K�w�ł̃N���X�̈ʒu��m�邽�߂�
 *		CObject �N���X�̓��I�h���I�u�W�F�N�g�ɕK�v�� C++ �̃R�[�h�𐶐�����D
 *
 *		�������͔h���N���X�i���̃N���X�FCSerialPortDialog�j�C�������͊�{�N���X�iCDialog�j���Ƃ�D
 */
IMPLEMENT_DYNAMIC(CSerialPortDialog, CDialog)

BEGIN_MESSAGE_MAP(CSerialPortDialog, CDialog)

	/// Windows�\��
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()

	/// �J�X�^�����b�Z�[�W
	ON_MESSAGE(WM_DLG_COMM_DISPLAY,			OnSwitchDisplayMode)
	ON_MESSAGE(WM_DLG_COMM_UPDATE,			OnRedrawDialog)
	ON_MESSAGE(WM_DLG_COMM_START_LISTEN,	OnStartPortListening)
	ON_MESSAGE(WM_DLG_COMM_STOP_LISTEN,		OnStopPortListening)
	ON_MESSAGE(WM_PLAN_SENDING_COMM_DATA,	OnSendCommandData)

	/// ���O
	ON_MESSAGE(WM_DLG_COMM_START_LOGGING,	OnStartLogging)
	ON_MESSAGE(WM_DLG_COMM_STOP_LOGGING,	OnStopLogging)

/**
 *		�ʐM�C�x���g
 */
	ON_MESSAGE(WM_COMM_CTS_DETECTED,		OnHandleReceiveMessage)
	ON_MESSAGE(WM_COMM_BREAK_DETECTED,		OnHandleReceiveMessage)
	ON_MESSAGE(WM_COMM_ERR_DETECTED,		OnHandleReceiveMessage)
	ON_MESSAGE(WM_COMM_RING_DETECTED,		OnHandleReceiveMessage)

	/// �p�P�b�g��M�C�x���g
	ON_MESSAGE(WM_COMM_RXFLAG_DETECTED,		OnHandleReceiveMessage)

	/// ��M�C�x���g
	ON_MESSAGE(WM_COMM_RXCHAR,				OnReceiveData)

END_MESSAGE_MAP()

/**
 *	----------------------------------------------------------------------
 *		CSerialPortDialog�N���X�̃����o�֐���`
 *	----------------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
 CSerialPortDialog::CSerialPortDialog(CWnd* pParent /*=NULL*/)
{
	/// �e�E�B���h�E�̃|�C���^�擾
	pParentWindow = pParent;

	/// �^�C�}ID
	timerID = 0;

	/// �J�E���^
	int i, j;
	/// ���M�p�P�b�g�o�b�t�@
	for (i=0; i<SCI_MAX_JOINT_NUM; i++)
	{
		for (j=0; j<SCI_PACKET_SIZE_SHORT; j++)
			ZeroMemory(shortPacket[i], SCI_PACKET_SIZE_SHORT);
	}

	ZeroMemory(longPacket, SCI_PACKET_SIZE_LONG);

	/// ��M�o�b�t�@�̍쐬
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
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *		CSerialPortDialog ���b�Z�[�W �n���h��
 *	------------------------------------------------------------
 */
/// Windows�Ǘ�
HBRUSH CSerialPortDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ������ DC �̑�����ύX���Ă��������B
    switch  ( nCtlColor )
	{
		case CTLCOLOR_DLG:	/// �w�i�F�̐ݒ�
		{
			if (*pWnd == *this)
				hbr = backColorBrush;

			break;
		}	/// case CTLCOLOR_DLG:
		case CTLCOLOR_MSGBOX:
		case CTLCOLOR_EDIT:
		{
			/// �����F
			pDC->SetTextColor( editColor );
			/// �w�i�F
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
				/// �����F
				pDC->SetTextColor( staticColor );
				/// �w�i�F
				pDC->SetBkColor( staticBkColor );
				
				hbr = staticBkColorBrush;
			}
			else
			{
				/// �����F
				pDC->SetTextColor( staticColor );
				/// �w�i�F
				pDC->SetBkColor( staticBkColor );
				
				hbr = staticBkColorBrush;
			}

			break;
		}	/// end of case CTLCOLOR_STATIC:
		default:
			break;
	}	/// switch  ( nCtlColor )
	// TODO:  ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}

/// Windows�^�C�}
void CSerialPortDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	PostMessage(WM_DLG_COMM_UPDATE);

	CDialog::OnTimer(nIDEvent);
}

/**
 *	�J�X�^�����b�Z�[�W
 */
/// �_�C�A���O�̕\��/��\���̐ؑ�
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

/// ��ʂ̍X�V
LRESULT CSerialPortDialog::OnRedrawDialog(WPARAM wParam, LPARAM lParam)
{
	int i;

	/// �p�P�b�g�f�[�^�̃��O�쐬
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
				/// ���M�p�̃��O�\��
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
					
				/// ���M�p�̃��O�\��
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


	/// ��M�p�̃��O�\��
	displayLogData(IDC_RECEIVING_EDIT, rxDataStr);

	return 1;
}

/// �|�[�g�̃��X�j���O�J�n
LRESULT CSerialPortDialog::OnStartPortListening(WPARAM wParam, LPARAM lParam)
{
	pCommPort->startListening(); //��������SerialPort.cpp�̃X���b�h�J�n�����

	//pUdpPort->UDPstartListening(); // ->������UDP�̃X���b�h�ɂ���΂�����H�@
	                                  //�������́A������UDPstartListening��ǉ�����΃V���A����UDP�𓯂��^�C�~���O�Ŏg���邩��
	                                  //startListening() -> UDPstartListening()�ɕς���.
	//restartListening();

	return 1;
}

/// �|�[�g�̃��X�j���O��~
LRESULT CSerialPortDialog::OnStopPortListening(WPARAM wParam, LPARAM lParam)
{
	pCommPort->stopListening(); //startListening() -> UDPstartListening()�ɕς���.

	//pUdpPort->UDPstopListening(); //

	return 1;
}

/// �w�ߒl�����{�b�g�փV���A�����M
LRESULT CSerialPortDialog::OnSendCommandData(WPARAM wParam, LPARAM lParam)//���@//(WPARAM)(&asuraXData), (LPARAM)(&planData)
{
	if ( (AsuraData*)wParam == NULL )
		return -1;

	/**
	 *		���[�J���ϐ�
	 */
	int i;

	/// ���M�f�[�^�ɑ��
	commAsuraData = *( (AsuraData*)wParam );

	switch (sciPacketType)
	{
	/*	case Comm::SHORT:
		{
			/// �p�P�b�g���M
			for (i=0;i<SCI_MAX_JOINT_NUM;i++)
			{
				/// �p�P�b�g�f�[�^�쐬
				ZeroMemory(shortPacket[i], SCI_PACKET_SIZE_SHORT);
				//buildPacket(Comm::SHORT, shortPacket[i], i+1, PACKET_CMD_JNT_POS);
				//pCommPort->sendData((&shortPacket[i][0]), (DWORD)SCI_PACKET_SIZE_SHORT);
			}
		}
		break;
		*/
		case Comm::LONG:
		{
			/// �p�P�b�g���M
			ZeroMemory(longPacket, SCI_PACKET_SIZE_LONG);
			//if(getLocomotionStyle()==LEGGED)//���
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

/// �V���A����M�f�[�^����
LRESULT CSerialPortDialog::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	/// �����Ă���A�h���X���L�����ǂ���
	if ( (char*)wParam == NULL )
		return -1;

	/// ���b�Z�[�W�f�[�^����M
	char* data = (char*)wParam;
	int length = HIWORD(lParam);

	/// ��M�o�b�t�@�ɏ�������
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
			
			/// �\���p�ϐ��ɏ�������
			rxDataStr.Format( TEXT("0x%2X 0x%2X 0x%2X 0x%2X\r\n"), buffer[0], buffer[1], buffer[2], buffer[3]);
			
		//	SetDlgItemText(ID�����, rxDataStr); //GUI��Ɏ�M�����v���l�\��?

			break;
		}
	}




	return 1;
}

/// �V���A����M�f�[�^�̗�O�C�x���g����
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
 *	���O�֌W
 */
LRESULT CSerialPortDialog::OnStartLogging(WPARAM wParam, LPARAM lParam)
{
	/// ���O�p�t�@�C���I�[�v��
	pSendLogWriter->openFile(TEXT("test.txt"), File::WRITE_ONLY);
	isLogging = true;

	return 1;
}

LRESULT CSerialPortDialog::OnStopLogging(WPARAM wParam, LPARAM lParam)
{
	/// ���O�p�t�@�C�������
	pSendLogWriter->closeFile();
	isLogging = false;

	return 1;
}


/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
BOOL CSerialPortDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	return CDialog::Create(IDD, pParentWindow);

	/*
	return CDialog::Create(lpszTemplateName, pParentWnd);
	*/
}

BOOL CSerialPortDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

/**
 *	�_�C�A���O�̃X�^�C��
 */
	/// �^�C�g���̐ݒ�
	SetWindowText( TEXT("Serial Port Log") );
	/// View�X�^�C���̐ݒ�
	setDialogStyle(GREEN_THEMED);

/**
 *	�I�u�W�F�N�g�̍쐬
 */
	/// �ʐM�|�[�g�̍쐬
	pCommPort = new SerialPort();
	/// ���O�I�u�W�F�N�g�쐬
	pSendLogWriter = new FileWriter();


/**
 *	�t���O�̏�����
 */
	/// ���O�t���O
	isLogging = false;
	/// �u���V
	isBrushCreated = false;
	/// �\���f�[�^
	isSendingDataDisplayed = true;
	isRecievingDataDisplayed = true;

/**
 *	�\���p�̕ϐ�
 */
	sciPacketType = Comm::LONG;

	/// Windows�̃^�C�}�Z�b�g�i�X�N���[���X�V�p�^�C�}�j
	timerID = 1;
	SetTimer(timerID, 100, NULL);	

/**
 *	�V���A���|�[�g�֘A
 */
	/// �V���A���|�[�g�̃I�[�v��
	if ( IsWindow(this->m_hWnd) )
		pCommPort->openSerialPort(this->m_hWnd, 5, 38400);//38400//115200		//SH2��COM�|�[�g

	/// �I�[�v���Ɠ����Ƀ��X�i�[�X���b�h�J�n
	pCommPort->startListening();
	
	/// �V�X�e���̑ҋ@
	Sleep(200);
	
	/// ���X�j���O�̊J�n�̓��[�U�[�ɔC���邽�߃X���b�h���~
	pCommPort->stopListening();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

BOOL CSerialPortDialog::DestroyWindow()
{
	return TRUE;
}

void CSerialPortDialog::PostNcDestroy()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
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
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	Default();

	//CDialog::OnCancel();
}

/**
 *	------------------------------------------------------------
 *		����֐�
 *	------------------------------------------------------------
 */

/**
 *	����
 *		���b�Z�[�W�𑗂�X���b�hID�̐ݒ�
 *	����
 *		id: �X���b�hID
 */
void CSerialPortDialog::setMotionPlanThreadID(DWORD id)
{
	motionPlanThreadID = id;
	return;
}

/**
 *	����
 *		�_�C�A���O�̔w�i�Ȃǂ�ݒ�
 *	����
 *		style: �w�i�̃X�^�C��
 */
void CSerialPortDialog::setDialogStyle(ViewStyle style)
{
	/// �X�^�C���̌���
	viewStyle = style;

	/// ���ɍ쐬����Ă�����u���V�j��
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
			/// �F����
			editColor = TXT_BLUE_COLOR_LIGHT;
			staticColor = Color::YELLOW;
			editBkColor = EDIT_BACK_GREEN_COLOR;
			staticBkColor = DLG_BACK_GREEN_COLOR /* EDIT_BACK_GREEN_COLOR */;

			///�u���V�쐬
			backColorBrush		= CreateSolidBrush( DLG_BACK_GREEN_COLOR );
			editBkColorBrush	= CreateSolidBrush( EDIT_BACK_GREEN_COLOR );
			staticBkColorBrush	= CreateSolidBrush( DLG_BACK_GREEN_COLOR /* EDIT_BACK_GREEN_COLOR */ );

			break;
		}
		case BLUE_THEMED:
		{
			/// �F����
			editColor = TXT_BLUE_COLOR_LIGHT;
			staticColor = TXT_YELLOW_COLOR_LIGHT;
			editBkColor = EDIT_BACK_BLUE_COLOR;
			staticBkColor = EDIT_BACK_BLUE_COLOR;

			///�u���V�쐬
			backColorBrush		= CreateSolidBrush( DLG_BACK_BLUE_COLOR );
			editBkColorBrush	= CreateSolidBrush( EDIT_BACK_BLUE_COLOR );
			staticBkColorBrush	= CreateSolidBrush( EDIT_BACK_BLUE_COLOR );

			break;
		}
		default:
			break;
	}

	/// �u���V�쐬����
	isBrushCreated = true;

	return;
}

/**
 *	����
 *		�V���A���ʐM�p�p�P�b�g�f�[�^�̍쐬
 *	����
 *		type: �p�P�b�g�^�C�v
 *		packet: ���M�o�b�t�@
 *		adress: ���M��֐ߔԍ��i�P�֐ߖ��߂̏ꍇ�̂݁j
 *		command: �w�ߎ��
 */
bool CSerialPortDialog::buildPacket(SciPacketType type, char* packet, int address, int command ) //unsigned char* packet
{
	/// �J�E���^
	int i,j;

	/// �ʐM�t�H�[�}�b�g�ŏꍇ����
	switch (type)
	{
		/// 1�֐߂��ƂɎw�ߒl�𑗂�ꍇ<---���쓮
		case Comm::SHORT:
		{
			/// �f�[�^����
			int data;

			/// �w�ߒl�ŏꍇ����
			switch (command)
			{
				/// �p�x�w��
				case PACKET_CMD_JNT_POS:
				{
					/// �r�֐߃f�[�^�擾
					/// �P�ʂ�Deg�ɏC�����A100�{���ėL������5���̐��̐����ɂ���i16bit�j
					data = (int)( ( commAsuraData.getLegJointAngle((int)((address-1) / 3) + 1)(((address-1) % 3) + 1)*RAD2DEG +180.0 )*100.0 );	/// <-�I�t�Z�b�g���Đ��l��
					//data =(int)(commAsuraData.getLegActuatorPosition(1)*100);

					/// �o�b�t�@�Ɉꎞ�ۑ� 
					//packet[0] = SCI_PACKET_HEADER;
					//packet[1] = SCI_PACKET_SIZE_SHORT;
					//packet[2] = (unsigned char)address;
					//packet[3] = (unsigned char)command;
					//packet[4] = (unsigned char)((data & 0xFF00) >> 8);
					//packet[5] = (unsigned char)(data & 0x00FF);
					//packet[6] = (unsigned char)((packet[4] + packet[5]) & 0x00FF);
				}
					break;

				/// ���x�i��
				case PACKET_CMD_JNT_SPD:
				{
				}
					break;

				default:
					break;
			}
		}
		break;

		/// �S�֐߂Ɏw�ߒl�𑗂�ꍇ
		case Comm::LONG:
		{
			

/*			/// �p�P�b�g�f�[�^�ɑ��
			/// �w�b�_
			packet[0]	= SCI_PACKET_HEADER;
			packet[1]	= SCI_PACKET_HEADER;
			packet[2]	= SCI_PACKET_SIZE_LONG;

				packet[3]	= PACKET_LEG_ALL_JOINTS;

				/// �R�}���h���e
				packet[4]	= changeflag;//PAKCET_CMD_WALK;

				/// �w�b�_�`�F�b�N�T��
				for (i=0;i<(SCI_PACKET_HEADER_SIZE-1);i++)
				{
					headerCheckSum += packet[i];
				}
				packet[5] = headerCheckSum;

				for (i=0;i<SCI_MAX_JOINT_NUM;i++)
				{
					/// �r�֐߃f�[�^�擾
					/// -180�`180����180�x���Z���āA�I�t�Z�b�g����0�`360�ɕϊ������l��
					///�p���x�͈ړ����x��-300�`300[mm/s]�Ƃ��C���̌�P�ʂ�[deg/s]�ɂ���@������+300���邱�ƂŃI�t�Z�b�g���s��
					if(i==2 || i==5 || i==8 || i==11){
						if(changeflag & BIT((i-2)/3))
						{
								data = (int)((commAsuraData.getTrackSpeed((int)(i/3)+1)* RAD2DEG/WHEELRADIUS+300) * 100.0);//�p���x
					//		if(i==5)
						cerr<<"ANvel  " <<data<<"   vel  " <<commAsuraData.getTrackSpeed((int)(i/3)+1)<<endl;
			
						}
						else
						{data = (int)((commAsuraData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);
							//if(i==5)
						cerr<<i<<" anggg =" <<data<<"   anggg  " <<commAsuraData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1)<<endl;
						}//�p�x
					}
					else
					{
					data = (int)((commAsuraData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);//�p�x
					if(i==1)
						cerr<<"2angll ="<<data<<endl;
					}
					dataHigh	= (unsigned char)((data & 0xFF00) >> 8);
					dataLow		= (unsigned char)(data & 0x00FF);
					sum = sum + dataHigh + dataLow;
						
					/// �r�̃p�P�b�g�f�[�^
					packet[2*(i + 3)]		= dataHigh;
					packet[2*(i + 3) + 1]	= dataLow;
				}
				
				/// �`�F�b�N�T��
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
			int address		= 0;//�A�N�`���G�[�^�[�ԍ� 1-24
			unsigned char CommandType= 0;
			int Act_Memo	= 0;
			
			for(i=0;i<LEG_NUM;i++){//for(i=0;i<1;i++){//

				for (j=0;j<LEG_ACT_NUM;j++){//for (j=0;j<1;j++){//

					
					switch (j)
					{
						CANID		=	0;
						CommandType	= 0;
						///�r�}�C�R��1
						case 0:{
							CANID		= (i+1)*100 + 10;//controller_2_1;//
							CommandType	= com_ballscllew_1_pos;///�{�[���˂�1
							
								}
							break;
						case 1:{
							CANID		= (i+1)*100 + 10;//controller_2_1;//
							CommandType	= com_ballscllew_2_pos;///�{�[���˂�2
							  
							   }							
							break;
							
						///�r�}�C�R��2
						case 2:{
							CANID		= (i+1)*100 + 20;//controller_2_2;//
							CommandType	= com_ballscllew_1_pos;///�{�[���˂�1
							   }
							break;
						case 3:{
							CANID		= (i+1)*100 + 20;//controller_2_2;//
							CommandType	= com_ballscllew_2_pos;///�{�[���˂�2
							   }
							break;
							
						default:
							break;				
					}
					TopID	=	CANID & 0x0000ff00;
					UnderID	=	CANID & 0x000000ff;
					TopID	=	TopID >>8;
					///�A�N�`���G�[�^�[�w�ߒl = �{�[���˂����W * 100
					//data = (int)( ( commAsuraData.getLegJointAngle(1)(1)*RAD2DEG +180.0 )*100.0 );
					//data =(int)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);
					//data =123*100; 
					//data =(unsigned __int32)(-123*100);
					data =(unsigned __int32)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);

					///�}�X�N�ƃL���X�g�𓯎��Ɏ��s����Ǝw�ߒl�G���[�Ȃ̂ŕ�����
					Top1	=	data & 0xff000000;
					Top2	=	data & 0x00ff0000;
					Under1	=	data & 0x0000ff00;
					Under2	=	data & 0x000000ff;
					Top1	=	Top1 	>> 24;
					Top2	=	Top2 	>> 16;
					Under1	=	Under1 	>> 8;

					/// �A�N�`���G�[�^�ւ̃p�P�b�g�f�[�^
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
						
						///�r�}�C�R��1
						if(j==0){
							CANID		= controller_1_1;//(i+1)*100 + 10;//
							CommandType	= com_ballscllew_1_pos;///�{�[���˂�1
							
								}

						else if(j==1){
							CANID		= controller_1_1;//(i+1)*100 + 10;//
							CommandType	= com_ballscllew_2_pos;///�{�[���˂�2							  
							   }							
						///�r�}�C�R��2
						else if(j==2){
							CANID		= controller_1_2;//(i+1)*100 + 20;//
							CommandType	= com_ballscllew_1_pos;///�{�[���˂�1
							   }
						else if(j==3){
							CANID		= controller_1_2;//(i+1)*100 + 20;//
							CommandType	= com_ballscllew_2_pos;///�{�[���˂�2
							   }							
						//default:
							//break;				
					//}
					TopID	=	CANID & 0x0000ff00;
					UnderID	=	CANID & 0x000000ff;
					TopID	=	TopID >>8;
					///�A�N�`���G�[�^�[�w�ߒl = �{�[���˂����W * 100
					//data = (int)( ( commAsuraData.getLegJointAngle(1)(1)*RAD2DEG +180.0 )*100.0 );
					//data =(int)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);
					//data =123*100; 
					//data =(unsigned __int32)(-123*100);
					data =(unsigned __int32)(commAsuraData.getLegActuatorPosition(i+1)(j+1)*100.0);

					///�}�X�N�ƃL���X�g�𓯎��Ɏ��s����Ǝw�ߒl�G���[�Ȃ̂ŕ�����
					Top1	=	data & 0xff000000;
					Top2	=	data & 0x00ff0000;
					Under1	=	data & 0x0000ff00;
					Under2	=	data & 0x000000ff;
					Top1	=	Top1 	>> 24;
					Top2	=	Top2 	>> 16;
					Under1	=	Under1 	>> 8;

					/// �A�N�`���G�[�^�ւ̃p�P�b�g�f�[�^
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

			///�Ō�ɃQ�[�g�E�F�C�}�C�R��

			CANID		= controller_0_0;
			CommandType	= 255;//START COMMAND
			data		= 255;//START COMMAND Checker

			///�}�X�N�ƃL���X�g�𓯎��Ɏ��s����Ǝw�ߒl�G���[�Ȃ̂ŕ�����
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
 *	���O�\���̂��߂̊֐�
 */
void CSerialPortDialog::displayLogData(UINT itemID, LPCTSTR lpFmt, ...)
{
	/**
	 *		���[�J���ϐ�
	 */
	/// �ψ���
	va_list arglist;
	/// �o�b�t�@
	TCHAR buf[MAX_BUFFER_SIZE];
	/// CEdit�ւ̃|�C���^
	CEdit* pEdit;
	/// �v�Z�p�̕ϐ�
	int i,j,k,l,m;

	/// �ό��̈������тɃA�N�Z�X
	va_start(arglist, lpFmt);

	/// ��������t�H�[�}�b�g
	wvsprintf(buf, lpFmt, arglist);

	/// �σp�����[�^�ҏW�I��
    va_end(arglist);

	/// ���O�̃R���g���[�����擾
	pEdit = (CEdit*)GetDlgItem(itemID);

	/// ���O�̍ő啶�������擾
	j = pEdit->GetLimitText();
	/// ���݂̃��O�̒������擾
	i = pEdit->GetWindowTextLength();
	
	if ( i + ( k = lstrlen(buf) ) > j )
	{
		/// �V���������񂪃��O�ɒǉ��ł��Ȃ���
		/// �Â����O���폜

		/// �ĕ`��t���O���N���A
		pEdit->SetRedraw(FALSE);

		/// �폜���Ȃ��Ă͂Ȃ�Ȃ��o�C�g�������߂�
		if ( ( k = k - (j-i) ) < 0 ) 
			k = 0;
		
		/// �폜���Ȃ��Ă͂Ȃ�Ȃ��o�C�g�����܂ލs�������߂�
		m = pEdit->LineFromChar( k );

		//���̍s�̃C���f�b�N�X�����߂�
		l = pEdit->LineIndex(m);
		
		if (l < k) 
			l = pEdit->LineIndex( m+1 );

		/// ���̍s�̐擪�ł͍폜���Ȃ��Ă͂Ȃ�Ȃ��o�C�g���ɖ����Ȃ���
		/// ���̍s�̃C���f�b�N�X�����߂�
		
		/// �폜����͈͂�I��
		pEdit->SetSel(0, l);
		/// �I�������͈͂��폜
		pEdit->ReplaceSel( TEXT("") );

		/// ���݂̃��O�̒������擾
		i = pEdit->GetWindowTextLength();
		/// �ĕ`��t���O���Z�b�g
		pEdit->SetRedraw();
	}

	/// ���O�̍Ō��I��
	pEdit->SetSel(i, i);
	/// �������ǉ�
	pEdit->ReplaceSel( buf );

	return;
}


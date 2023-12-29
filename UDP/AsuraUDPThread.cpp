// AsuraUDPTherad.cpp : �����t�@�C��
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

	///�J�X�^�����b�Z�[�W
	ON_MESSAGE(WM_DLG_UDPTHREAD_END, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnEndThread)
	ON_MESSAGE(WM_DLG_COMM_START_LISTEN, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStartPortListening)
	ON_MESSAGE(WM_DLG_COMM_STOP_LISTEN, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStopPortListening)
	ON_MESSAGE(WM_PLAN_SENDING_COMM_DATA, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnSendCommandData)

	/* ���O ������20210622
	ON_MESSAGE(WM_DLG_COMM_START_LOGGING, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStartLogging)
	ON_MESSAGE(WM_DLG_COMM_STOP_LOGGING, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnStopLogging)
	*/

END_MESSAGE_MAP()

AsuraUDPThread::AsuraUDPThread()
	:isAlive(),isCommAlive(),addr(),client_addr(),client_IPAdress()
	,longPacket(),onelegPacket(),pMultiMediaTimer(),pTimedUDPProcedure()
	,port(),sciPacketType(),server_IPAdress(),server_addr(),shortPacket()
{ 
	///�����t���O�̏�����
	isAlive = FALSE;

	//�ʐM�t���O
	isCommAlive = FALSE;

	//sock initialize
	sock=INVALID_SOCKET;

}

AsuraUDPThread::~AsuraUDPThread()
{
	//�����t���O�̏I������
	isAlive = FALSE;
}

BOOL AsuraUDPThread::InitInstance()
{
	// TODO:  �X���b�h���Ƃ̏������������Ŏ��s���܂��B

	/// �X���b�h���A�N�e�B�u��
	isAlive = TRUE;



	return TRUE;
}

int AsuraUDPThread::ExitInstance()
{
	// TODO:  �X���b�h���Ƃ̌㏈���������Ŏ��s���܂��B
	 
	

	finalizeAsuraUDPThread();
	return CWinThread::ExitInstance();
}

// ASuraUDPThread ���b�Z�[�W �n���h��
/**
 *	------------------------------------------------------------
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *		MotionPlanThread ���b�Z�[�W�n���h�� �֐�
 *	------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	Doc�CView�N���X�֘A
  *	----------------------------------------
  */
 /**
  *	����
  *		�X���b�h�̏I������
 */
void AsuraUDPThread::OnEndThread(WPARAM wParam, LPARAM lParam)
{
	/// �X���b�h�I������
	ExitInstance();

	/// �V�X�e���ɏI����v��(WM_DESTORY�ɑΉ�)
	PostQuitMessage(0);
}


/**
 *	----------------------------------------
 *	�ʐM�֘A
 *	----------------------------------------
 */
///�ʐM�J�n
void AsuraUDPThread::OnStartPortListening(WPARAM wParam, LPARAM lParam)
{
	//���[�J���ϐ�
	int n=1;

	/*
		sock open
	*/
	// initalize winsock2
	WSADATA wsaData;
	n=WSAStartup(MAKEWORD(2, 0), &wsaData); ///����������߂�l0
	

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

///�ʐM��~
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

	/// �X���b�h�I������
	ExitInstance();

	/// �V�X�e���ɏI����v��(WM_DESTORY�ɑΉ�)
	PostQuitMessage(0);
}

void AsuraUDPThread::OnSendCommandData(WPARAM wParam, LPARAM lParam)
{
	//get command 
	if ((AsuraData*)wParam != NULL)
		viewAsuraXData = *((AsuraData*)wParam);
	if ((PlanData*)lParam != NULL)
		viewPlanData = *((PlanData*)lParam);

	//���M�o�b�t�@�ւ̏�������

	/**
	 *		���[�J���ϐ�
	 */
	int i;
	sciPacketType = SciPacketType::LONG;

	switch (sciPacketType)
	{
	case SciPacketType::SHORT:
	{
		/// �p�P�b�g���M
		for (i = 0;i < SCI_MAX_JOINT_NUM;i++)
		{
			/// �p�P�b�g�f�[�^�쐬
			ZeroMemory(shortPacket[i], SCI_PACKET_SIZE_SHORT);
			//buildPacket(SciPacketType::SHORT, shortPacket[i], i+1, PACKET_CMD_JNT_POS);
			//pCommPort->sendData((&shortPacket[i][0]), (DWORD)SCI_PACKET_SIZE_SHORT);
		}
	}
	break;

	case  SciPacketType::LONG:
	{
		/// �p�P�b�g���M
		SecureZeroMemory(&longPacket, sizeof(longPacket));
		//if(getLocomotionStyle()==LEGGED)//���
		buildPacket(SciPacketType::LONG, SCI_ALL_JOINTS, PAKCET_CMD_WALK);
		//�p�P�b�g���M
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




//�J�n����
void AsuraUDPThread::initializeAsuraUDPThread(void)
{
	/*
	*
	* �o�b�t�@�̈�̊m��
	*
	*/


	if (writeBuffer != NULL) delete[] writeBuffer;
	if (writeBufferSize_ > MAX_BUFFER_SIZE) writeBufferSize_ = MAX_BUFFER_SIZE;
	writeBuffer = new unsigned char[writeBufferSize_];

	/// �o�b�t�@�T�C�Y�̐ݒ�
	writeBufferSize = writeBufferSize_;

	/// �o�b�t�@�̏�����
	ZeroMemory(writeBuffer, writeBufferSize);



	/**
	 *	����
	 *		�^�C�}�R�[���o�b�N�쐬
	 */
	 /// �I�u�W�F�N�g�쐬
	pTimedUDPProcedure = new System::TimedMotionProcedure();
	/// �^�C�}�R�[���o�b�N�ɖ{�X���b�h��ID��o�^
	pTimedUDPProcedure->setThreadID(m_nThreadID);	//// m_nThread�͊��N���X��CWinThread�̃����o

/**
 *	�^�C�}�{�̍쐬
 */
	pMultiMediaTimer = new System::MultiMediaTimer(*pTimedUDPProcedure);
	pMultiMediaTimer->setTimer(20, 5);

	isAlive = TRUE;
}


//�I������
void AsuraUDPThread::finalizeAsuraUDPThread(void)
{
	/**
	 *	�^�C�}�I������
	 */
	 /// �^�C�}����~���Ă��Ȃ�������
	if (pMultiMediaTimer != NULL)
	{
		pMultiMediaTimer->killTimer();

		/// �^�C�}�I�u�W�F�N�g�j��
		delete pMultiMediaTimer;
		pMultiMediaTimer = NULL;
	}

	if (pTimedUDPProcedure != NULL)
	{
		/// �^�C�}�R�[���o�b�N�j��
		delete pTimedUDPProcedure;
		pTimedUDPProcedure = NULL;
	}

	return;

}

//�@�p�P�b�g�f�[�^�쐬
char AsuraUDPThread::buildPacket(SciPacketType type, int address, int command)
{
	/// �J�E���^
	int i, j;
	

	/// �ʐM�t�H�[�}�b�g�ŏꍇ����
	switch (type)
	{
		/// 1�֐߂��ƂɎw�ߒl�𑗂�ꍇ<---���쓮
	case SciPacketType::SHORT:
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
			data = (int)((viewAsuraXData.getLegJointAngle((int)((address - 1) / 3) + 1)(((address - 1) % 3) + 1) * RAD2DEG + 180.0) * 100.0);	/// <-�I�t�Z�b�g���Đ��l��
			//data =(int)(commTitanData.getLegActuatorPosition(1)*100);

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
	case SciPacketType::LONG:
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
										data = (int)((commTitanData.getTrackSpeed((int)(i/3)+1)* RAD2DEG/WHEELRADIUS+300) * 100.0);//�p���x
							//		if(i==5)
								cerr<<"ANvel  " <<data<<"   vel  " <<commTitanData.getTrackSpeed((int)(i/3)+1)<<endl;

								}
								else
								{data = (int)((commTitanData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);
									//if(i==5)
								cerr<<i<<" anggg =" <<data<<"   anggg  " <<commTitanData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1)<<endl;
								}//�p�x
							}
							else
							{
							data = (int)((commTitanData.getLegJointAngle((int)(i / 3) + 1)(i%3 + 1) * RAD2DEG + 180.0) * 100.0);//�p�x
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
		*/
	
		unsigned __int32 data = 0;
		double Actpos=0;
		unsigned char symbol=0;
		
		unsigned __int16  CANID = 0;
		unsigned __int16  TopID = 0;
		unsigned __int16  UnderID = 0;
		unsigned __int16  Data1 = 0;
		unsigned __int16  Data2 = 0;
		int address = 0;//�A�N�`���G�[�^�[�ԍ� 1-24
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
					///�r�}�C�R��1
				case 0: {
					CANID = (i + 1) * 100 + 10;//controller__1;//
					CommandType = com_ballscllew_1_pos;///�{�[���˂�1

				}
					  break;
				case 1: {
					CANID = (i + 1) * 100 + 10;//controller_2_1;//
					CommandType = com_ballscllew_2_pos;///�{�[���˂�2

				}
					  break;

					  ///�r�}�C�R��2
				case 2: {
					CANID = (i + 1) * 100 + 20;//controller_2_2;//
					CommandType = com_ballscllew_1_pos;///�{�[���˂�1
				}
					  break;
				case 3: {
					CANID = (i + 1) * 100 + 20;//controller_2_2;//
					CommandType = com_ballscllew_2_pos;///�{�[���˂�2
				}
					  break;

				default:
					break;
				}
				TopID = CANID & 0x0000ff00;
				UnderID = CANID & 0x000000ff;
				TopID = TopID >> 8;
				///�A�N�`���G�[�^�[�w�ߒl = �{�[���˂����W * 100
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
					Actpos = (-1) * Actpos;		//�������]
					data = (unsigned int)Actpos;
					symbol = 0xff;
				}
				
							

				///�}�X�N�ƃL���X�g�𓯎��Ɏ��s����Ǝw�ߒl�G���[�Ȃ̂ŕ�����
				Data1 = data & 0xff00;
				Data2= data & 0x00ff;
				Data1 = Data1 >> 8;

				//�p�P�b�g�`�F�b�N�T��
				Cdata1 = Data1&0x00f0;
				Cdata2 = Data2&0x000f;
				Checksum = (unsigned char)(Cdata1 + Cdata2);


				/// �A�N�`���G�[�^�ւ̃p�P�b�g�f�[�^
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
					//data = (int)( ( commTitanData.getLegJointAngle(1)(1)*RAD2DEG +180.0 )*100.0 );
					//data =(int)(commTitanData.getLegActuatorPosition(i+1)(j+1)*100.0);
					//data =123*100;
					//data =(unsigned __int32)(-123*100);
					data =(unsigned __int32)(commTitanData.getLegActuatorPosition(i+1)(j+1)*100.0);

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
		/*
		CANID = controller_0_0;
		CommandType = 255;//START COMMAND
		data = 255;//START COMMAND Checker

		///�}�X�N�ƃL���X�g�𓯎��Ɏ��s����Ǝw�ߒl�G���[�Ȃ̂ŕ�����
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

// AsuraUDPTherad ���b�Z�[�W �n���h���[

/**
 *  �t�@�C����
 *		MotionPlanThread.cpp
 *  ����
 *		���e�v����s���X���b�h�����N���X�̐錾�N���X
 *		 UI�X���b�h�Ƃ��č쐬���邽��CWinThread����h��
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/26(SAT)
 */

// MotionPlanThread.cpp : �����t�@�C��
//

//  20200820  trotGait��tripodGait�ɏC���EAxisControl�_�C�A���O�֘A�R�����g�A�E�g
//  20201005  1�����ŏI��

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
//#include "..\..\stdafx.h"
#include "..\..\pch.h"

#include "..\..\ASURA2GUI.h"
#include "MotionPlanThread.h"

//#include "..\..\Dialog\AxisControlDialog.h"
//#include "..\..\Dialog\SerialPortDialog.h"
#include "..\..\Utility\Constants.h"


using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;
using namespace Data;
using namespace System;

// MotionPlanThread

/**
 *	----------------------------------------------------------------------
 *		MotionPlanThread�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		MotionPlanThread�N���X��static�ϐ��̏�����
 *	------------------------------------------------------------
 */
AsuraX MotionPlanThread::asuraX;

AsuraData MotionPlanThread::asuraXData;
PlanData MotionPlanThread::planData;
//DataHandler MotionPlanThread::dataHandler(&asuraX, &asuraXData, &hybridMotion, &planData);
DataHandler MotionPlanThread::dataHandler(&asuraX, &asuraXData, &tripodGait, &planData);  //20200820

TimeManager MotionPlanThread::timeManager;

PlannerManager MotionPlanThread::plannerManager;
//AxisControlPlanner MotionPlanThread::axisControl(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);

//CrawlGaitPlanner MotionPlanThread::crawlGait(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
TripodGaitPlanner MotionPlanThread::tripodGait(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);  //  20200820
//TrackDrivePlanner MotionPlanThread::trackDrive(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
//ModeChangePlanner MotionPlanThread::modeChange(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
//HybridMotionPlanner MotionPlanThread::hybridMotion(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);
//URGMotionPlanner MotionPlanThread::URGMotion(&MotionPlanThread::asuraX, &MotionPlanThread::timeManager);		//�ǉ�

//double MotionPlanThread::axisCtrlData[AC_DLG_AXIS_NUM] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};  20200820


/**
 *	------------------------------------------------------------
 *		MotionPlanThread�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */
IMPLEMENT_DYNCREATE(MotionPlanThread, CWinThread)

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
MotionPlanThread::MotionPlanThread():pMultiMediaTimer(),pTimedMotionProcedure(),pUDPThread()
{
	/// �����t���O�̏�����
	isAlive = FALSE;

	/// �ʐM�t���O
	isCommAlive = false;

}

MotionPlanThread::~MotionPlanThread()
{
	/// �����t���O�̏I������
	isAlive = FALSE;
}

BOOL MotionPlanThread::InitInstance()
{
	// TODO:  �X���b�h���Ƃ̏������������Ŏ��s���܂��B
	initializeMotionPlanThread();
	
	return TRUE;
}

int MotionPlanThread::ExitInstance()
{
	// TODO:  �X���b�h���Ƃ̌㏈���������Ŏ��s���܂��B
	finalizeMotionPlanThread();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(MotionPlanThread, CWinThread)

	ON_THREAD_MESSAGE(WM_PLAN_END_THREAD,			OnEndThread)					/// �X���b�h�I��
	ON_THREAD_MESSAGE(WM_PLAN_REQUEST_VIEW_DATA,	OnSendViewData)					/// �\���f�[�^���M
	ON_THREAD_MESSAGE(WM_PLAN_SETUP_MOTION,			OnSetupMotion)					/// �v�����̃Z�b�g�A�b�v
	ON_THREAD_MESSAGE(WM_PLAN_START_MOTION,			OnStartMotion)					/// �v�����J�n
	ON_THREAD_MESSAGE(WM_PLAN_STOP_MOTION,			OnStopMotion)					/// �v������~
	ON_THREAD_MESSAGE(WM_PLAN_GENERATE_MOTION,		OnGenerateMotion)				/// �v��������

	ON_THREAD_MESSAGE(WM_PLAN_COMM_START,			OnStartDataSending)				/// �f�[�^���M�J�n
	ON_THREAD_MESSAGE(WM_PLAN_COMM_STOP,			OnStopDataSending)				/// �f�[�^���M��~

	//ON_THREAD_MESSAGE(WM_PLAN_AC_LEG_NO,			OnAxisCtrlDlgLegNo)				/// �r�ԍ��I��
	//ON_THREAD_MESSAGE(WM_PLAN_AC_CTRL_MODE,			OnAxisCtrlDlgCtrlMode)			/// ���䃂�[�h�I��
	//ON_THREAD_MESSAGE(WM_PLAN_AC_SENDING_SLDR_DATA,	OnAxisCtrlDlgRecieveSldrData)	/// �f�[�^��M
	//ON_THREAD_MESSAGE(WM_PLAN_AC_REQUEST_DATA,		OnAxisCtrlDlgSendData)			/// �f�[�^���M

	//ON_THREAD_MESSAGE(WM_PLAN_URGTHREAD_SET,		OnURGThreadSet)					/// URGThread�A�h���X�Z�b�g

END_MESSAGE_MAP()

// MotionPlanThread ���b�Z�[�W �n���h��
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
 void MotionPlanThread::OnEndThread(WPARAM wParam, LPARAM lParam)
{
	/// �X���b�h�I������
	ExitInstance();

	/// �V�X�e���ɏI����v��(WM_DESTORY�ɑΉ�)
	PostQuitMessage(0);
}

/**
 *	����
 *		���C���X���b�h�ւ̃f�[�^���M
 */
void MotionPlanThread::OnSendViewData(WPARAM wParam, LPARAM lParam)
{
	/// View��AsuraX�̕\���f�[�^�𑗐M
	::PostMessage( (HWND)wParam, WM_PLAN_SENDING_VIEW_DATA, (WPARAM)&asuraXData, (LPARAM)&planData );
}

/**
 *	����
 *		���e�Z�b�g�A�b�v
 */
void MotionPlanThread::OnSetupMotion(WPARAM wParam, LPARAM lParam)
{
	Plan::Strategy strategy = (Plan::Strategy)lParam;

	switch (strategy)
	{
		/*case CRAWL:

			plannerManager.switchPlan(&crawlGait, Plan::CRAWL);
			break;*/

		/*case TROT:
			plannerManager.switchPlan(&tripodGait, Plan::TROT);  //  20200820
			break;*/

		case TRIPOD:
			plannerManager.switchPlan(&tripodGait, Plan::TRIPOD);  //  20200820
			break;

		//20201005
		case TRIPOD_1_CYCLE:
			plannerManager.switchPlan(&tripodGait, Plan::TRIPOD_1_CYCLE);  //  20200820
			break;

		/*case TRACK_DRIVE:
			plannerManager.switchPlan(&trackDrive, Plan::TRACK_DRIVE);
			break;*/

		/*case MODE_CHANGE:
			plannerManager.switchPlan(&modeChange, Plan::MODE_CHANGE);
			break;*/

		/*case AXIS_CONTROL:
			plannerManager.switchPlan(&axisControl, Plan::AXIS_CONTROL);
			break;*/

	/*	case HYBRID_MOTION:
			plannerManager.switchPlan(&hybridMotion, Plan::HYBRID_MOTION);
			break;*/

		/*case URG_MOTION:
			plannerManager.switchPlan(&URGMotion, Plan::URG_MOTION);		//�ǉ�
			break;*/

		default:
			break;
	}

	plannerManager.setup();
}

/**
 *	����
 *		���e�J�n
 */
void MotionPlanThread::OnStartMotion(WPARAM wParam, LPARAM lParam)
{
	plannerManager.startMotion();
}

/**
 *	����
 *		���e��~
 */
void MotionPlanThread::OnStopMotion(WPARAM wParam, LPARAM lParam)
{
	plannerManager.stopMotion();
}

/**
 *	����
 *		���e����
 *		�^�C�}�ɂ���ČĂ΂�钆�g
 */
void MotionPlanThread::OnGenerateMotion(WPARAM wParam, LPARAM lParam)
{
	/// ���m�Ȍ��ݎ������擾
	timeManager.setRealTime( pMultiMediaTimer->getPresentTime() );

	if ( plannerManager.isActive() )
	{
		/// ����e�̃X�i�b�v�V���b�g�f�[�^�쐬
		plannerManager.createSnapshot();
	}

	/// �f�[�^�̕ۑ�
	dataHandler.handleAsuraData();
	dataHandler.handlePlanData();

	/// <UDP�Ń{�[���˂��X�g���[�N�i���C���ψʁj�w�ߒl���M>

	if ( isCommAlive )
		pUDPThread->PostThreadMessage(WM_PLAN_SENDING_COMM_DATA, (WPARAM)(&asuraXData), (LPARAM)(&planData) );


	return;
}

/**
 *	----------------------------------------
 *	�ʐM�֘A
 *	----------------------------------------
 */
/// �f�[�^���M�J�n
void MotionPlanThread::OnStartDataSending(WPARAM wParam, LPARAM lParam)
{
	isCommAlive = true;
}

/// �f�[�^���M��~
void MotionPlanThread::OnStopDataSending(WPARAM wParam, LPARAM lParam)
{
	isCommAlive = false;
}

///**
// *	----------------------------------------
// *	CAxisControlDialog�֘A
// *	----------------------------------------
// */
///// �r�ԍ��ݒ�
//void MotionPlanThread::OnAxisCtrlDlgLegNo(WPARAM wParam, LPARAM lParam)
//{
//	/// �r�ԍ���ݒ�
//	int legNo = (int)wParam;
//	axisControl.setCtrlTargetLegNo( legNo );
//
//	/// ���ݒl��Planner�ɕێ�
//	axisControl.setJointAngle(legNo, asuraXData.getLegJointAngle( legNo ) );
//	axisControl.setFootPosition(legNo, asuraXData.getLegFootPosition( legNo ) );
//
//	return;
//}
//
///// ���䃂�[�h�ݒ�
//void MotionPlanThread::OnAxisCtrlDlgCtrlMode(WPARAM wParam, LPARAM lParam)
//{
//	/// �_�C�A���O�̐��䃂�[�h�ԍ��ɂ�萧�䃂�[�h������
//	switch ( (int)wParam )
//	{
//		case 0:
//			axisControl.setControlMode( AxisControlPlanner::CONTROL_OFF );
//			break;
//
//		case 1:
//			axisControl.setControlMode( AxisControlPlanner::POSITION );
//			break;
//
//		case 2:
//			axisControl.setControlMode( AxisControlPlanner::VELOCITY );
//			break;
//
//		default:
//			break;
//	}
//}
//
///// �X���C�_�[�l��M
//void MotionPlanThread::OnAxisCtrlDlgRecieveSldrData(WPARAM wParam, LPARAM lParam)
//{
//	/// ���䃂�[�h�̎擾
//	AxisControlPlanner::ControlMode mode = axisControl.getControlMode();
//	/// ����r�̔ԍ��擾 
//	int no = axisControl.getCtrlTargetLegNo();
//	/// �w�ߒl���擾
//	double data = (double)lParam;
//
//	/// ����I�u�W�F�N�g�̌��ݒl���擾
//	Vector joint(LEG_JOINT_NUM);
//	Vector foot(THREE_DIMENSION);
//
//	if ( mode == AxisControlPlanner::POSITION )
//	{
//		joint = asuraXData.getLegJointAngle(no);
//		foot = asuraXData.getLegFootPosition(no);
//
//		/// �X���C�_�[�ԍ��ɂ�苓���ƒl������
//		switch ( (int)wParam )
//		{
//			case 1:
//				axisControl.setKinematicBehavior( AxisControlPlanner::DIRECT );
//				joint(1) = data*DEG2RAD;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 2:
//				axisControl.setKinematicBehavior( AxisControlPlanner::DIRECT );
//				joint(2) = data*DEG2RAD;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 3:
//				axisControl.setKinematicBehavior( AxisControlPlanner::DIRECT );
//				joint(3) = data*DEG2RAD;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 4:
//				axisControl.setKinematicBehavior( AxisControlPlanner::INVERSE );
//				foot(1) = data;
//				axisControl.setFootPosition(no, foot);
//				break;
//			case 5:
//				axisControl.setKinematicBehavior( AxisControlPlanner::INVERSE );
//				foot(2) = data;
//				axisControl.setFootPosition(no, foot);
//				break;
//			case 6:
//				axisControl.setKinematicBehavior( AxisControlPlanner::INVERSE );
//				foot(3) = data;
//				axisControl.setFootPosition(no, foot);
//				break;
//			default:
//				break;
//		}
//	}
//	else if ( mode == AxisControlPlanner::VELOCITY )
//	{
//		/// �X���C�_�[�ԍ��ɂ�苓���ƒl������
//		switch ( (int)wParam )
//		{
//			case 1:
//				joint(1) = data;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 2:
//				joint(2) = data;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 3:
//				joint(3) = data;
//				axisControl.setJointAngle(no, joint);
//				break;
//			case 4:
//				foot(1) = data;
//				axisControl.setFootPosition(no, joint);
//				break;
//			case 5:
//				foot(2) = data;
//				axisControl.setFootPosition(no, joint);
//				break;
//			case 6:
//				foot(3) = data;
//				axisControl.setFootPosition(no, joint);
//				break;
//			default:
//				break;
//		}
//	}
//
//	return;
//
//}
//
///// AsuraX�̃f�[�^��AC�_�C�A���O�ɑ��M
//void MotionPlanThread::OnAxisCtrlDlgSendData(WPARAM wParam, LPARAM lParam)
//{
//	PostMessage( (HWND)wParam, WM_PLAN_SENDING_AC_DATA, (WPARAM)&asuraXData, NULL);
//	
//
//	/// AxisControlPlanner�̎w�ߒl���X�V
//	int legNo = axisControl.getCtrlTargetLegNo();
//	if ( legNo != 0 )
//	{
//		axisControl.setJointAngle( legNo, asuraXData.getLegJointAngle(legNo) );
//		axisControl.setFootPosition( legNo, asuraXData.getLegFootPosition(legNo) );
//	}
//
//	return;
//}
//
////URGThread�A�h���X�Z�b�g
//void MotionPlanThread::OnURGThreadSet(WPARAM wParam, LPARAM lParam)
//{
//	URGThreadID=(DWORD)wParam;
//
//	pTimedMotionProcedure->URGsetThreadID(URGThreadID);	//// m_nThread�͊��N���X��CWinThread�̃����o		URG�p
//
//	return;
//}

/**
 *	------------------------------------------------------------
 *		����֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		������
 */
void MotionPlanThread::initializeMotionPlanThread(void)
{

/**
 *	����
 *		�^�C�}�R�[���o�b�N�쐬
 */
	/// �I�u�W�F�N�g�쐬
	pTimedMotionProcedure = new TimedMotionProcedure();
	/// �^�C�}�R�[���o�b�N�ɖ{�X���b�h��ID��o�^
	pTimedMotionProcedure->setThreadID(m_nThreadID);	//// m_nThread�͊��N���X��CWinThread�̃����o
	

/**
 *	�^�C�}�{�̍쐬
 */
	pMultiMediaTimer = new MultiMediaTimer(*pTimedMotionProcedure);
	pMultiMediaTimer->setTimer(20, 5);

	/// �X���b�h���A�N�e�B�u��
	isAlive = TRUE;
}

/**
 *	����
 *		�I������
 */
void MotionPlanThread::finalizeMotionPlanThread(void)
{
/**
 *	�^�C�}�I������
 */
	/// �^�C�}����~���Ă��Ȃ�������
	if ( pMultiMediaTimer != NULL )
	{
		pMultiMediaTimer->killTimer();
	
		/// �^�C�}�I�u�W�F�N�g�j��
		delete pMultiMediaTimer;
		pMultiMediaTimer = NULL;	
	}

	if ( pTimedMotionProcedure != NULL )
	{
		/// �^�C�}�R�[���o�b�N�j��
		delete pTimedMotionProcedure;
		pTimedMotionProcedure = NULL;
	}

	return;
}

///**
// *	����
// *		���삷��_�C�A���O�ւ̃|�C���^���Z�b�g
// */
///// ������_�C�A���O
//void MotionPlanThread::acquireAxisControlDialog(CAxisControlDialog* pDlg)
//{
//	pAxisControlDialog = pDlg;
//	return;
//}
//
///// �V���A���ʐM�_�C�A���O
//void MotionPlanThread::acquireSerialPortDialog(CSerialPortDialog* pDlg)
//{
//	pSerialPortDialog = pDlg;
//	return;
//}

//UDP�ʐM�_�C�A���O�ւ̃|�C���^���Z�b�g
void MotionPlanThread::acquireAsuraUDPThread(AsuraUDPThread* pDlg)
{
	pUDPThread = pDlg;
	return;
}
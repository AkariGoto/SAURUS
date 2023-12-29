/**
 *  �t�@�C����
 *		MotionPlanThread.h
 *  ����
 *		���e�v����s���X���b�h�����N���X�̐錾�N���X
 *		 UI�X���b�h�Ƃ��č쐬���邽��CWinThread����h��
 *  ���t
 *		�쐬��: 2014/04/28(MON)		�X�V��:
 */

//  20200820  �C���N���[�h����N���X����TrotGait.h����TripodGait.h�ɏC���EURG�EAxisControl�_�C�A���O�֘A�R�����g�A�E�g

#pragma once

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <afxmt.h>

#include "..\..\Kinematics\AsuraX.h"
#include "..\..\Data\AsuraData.h"
#include "..\..\Data\PlanData.h"
#include "..\..\Data\DataHandler.h"

#include "..\..\Plan\PlanParameter.h"
#include "..\..\Plan\PlannerManager.h"
//#include "..\..\Plan\AxisControlPlanner.h"
//
//#include "..\..\Plan\CrawlGaitPlanner.h"
//#include "..\..\Plan\TrotGaitPlanner.h"
#include "..\..\Plan\TripodGaitPlanner.h"  //  20200820
//#include "..\..\Plan\TrackDrivePlanner.h"
//#include "..\..\Plan\ModeChangePlanner.h"
//#include "..\..\Plan\HybridMotionPlanner.h"
//#include "..\..\Plan\URGMotionPlanner.h"		//�ǉ�

#include "..\Timer\MultiMediaTimer.h"
#include "TimedMotionProcedure.h"
#include "..\WinUserMessage.h"
//#include "ASURA2GUI/UDP/AsuraUDPThread.h"
#include "..\..\UDP/AsuraUDPThread.h"


/**
 *	----------------------------------------------------------------------
 *		�ǉ��̃N���X�̑O���錾
 *	----------------------------------------------------------------------
 */
//class CAxisControlDialog;  20200820
//class CSerialPortDialog;
class AsuraUDPThread;

/**
 *	----------------------------------------------------------------------
 *		MotionPlanThread�N���X
 *	----------------------------------------------------------------------
 */
class MotionPlanThread : public CWinThread
{
	DECLARE_DYNCREATE(MotionPlanThread)

/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

/**
 *		Asura�̃L�l�}�e�B�N�X�I�u�W�F�N�g
 */
	static Asura::AsuraX asuraX;

/**
 *		�f�[�^�I�u�W�F�N�g
 */
	/// �L�l�}�e�B�N�X�f�[�^
	static Data::AsuraData asuraXData;
	/// �v�����f�[�^
	static Data::PlanData planData;
	/// �f�[�^�n���h���I�u�W�F�N�g
	static Data::DataHandler dataHandler;

/**
 *		����v��I�u�W�F�N�g
 */
	static Plan::PlannerManager		plannerManager;
	//static Plan::AxisControlPlanner	axisControl;

	//static Plan::CrawlGaitPlanner	crawlGait;
	static Plan::TripodGaitPlanner	tripodGait;  //20200820
	//static Plan::TrackDrivePlanner	trackDrive;
	//static Plan::ModeChangePlanner	modeChange;
	//static Plan::HybridMotionPlanner	hybridMotion;
	//static Plan::URGMotionPlanner	URGMotion;		//�ǉ�

/**
 *		�^�C�}�I�u�W�F�N�g
 */
	System::TimedMotionProcedure*	pTimedMotionProcedure;
	System::MultiMediaTimer*		pMultiMediaTimer;
	static Plan::TimeManager		timeManager;

/**
 *		�_�C�A���O�֘A
 */
	/// �|�C���^
	//CAxisControlDialog* pAxisControlDialog;  20200820
	//CSerialPortDialog*	pSerialPortDialog;
	AsuraUDPThread* pUDPThread;
	/// CAxisControlDialog
	//static double axisCtrlData[Plan::AC_DLG_AXIS_NUM];  20200820

/**
 *		�t���O
 */
	/// �X���b�h���L�����ǂ���
	BOOL isAlive;

	/// �ʐM���L�����ǂ���
	bool isCommAlive;

	//DWORD URGThreadID;  20200820


/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
protected:

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	/// �f�t�H���g�R���X�g���N�^
	MotionPlanThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
	/// �f�X�g���N�^
	virtual ~MotionPlanThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

/**
 *	------------------------------------------------------------
 *		����֐�
 *	------------------------------------------------------------
 */
	/**
	 *	����
	 *		������
	 */
	void initializeMotionPlanThread(void);

	/**
	 *	����
	 *		�I������
	 */
	void finalizeMotionPlanThread(void);

	/**
	 *	����
	 *		�X���b�h���L�����ǂ���
	 */
	BOOL isRunning(void) const{return isAlive;}

	/**
	 *	����
	 *		���삷��_�C�A���O�ւ̃|�C���^���Z�b�g
	 */
	/// ������_�C�A���O
	//void acquireAxisControlDialog(CAxisControlDialog* pDlg);
	/// �V���A���ʐM�_�C�A���O
	//void acquireSerialPortDialog(CSerialPortDialog* pDlg);

	void MotionPlanThread::acquireAsuraUDPThread(AsuraUDPThread* pDlg);

DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *	------------------------------------------------------------
 */
protected:
/**
 *		�J�X�^�����b�Z�[�W�}�b�v�J�n
 */
	/// �X���b�h�I��
	afx_msg void OnEndThread(WPARAM wParam, LPARAM lParam);
	/// �f�[�^�����C���X���b�h�ɑ��M
	afx_msg void OnSendViewData(WPARAM wParam, LPARAM lParam);
	/// ���e�̃Z�b�g�A�b�v
	afx_msg void OnSetupMotion(WPARAM wParam, LPARAM lParam);
	/// ���e�X�^�[�g
	afx_msg void OnStartMotion(WPARAM wParam, LPARAM lParam);
	/// ���e�X�g�b�v
	afx_msg void OnStopMotion(WPARAM wParam, LPARAM lParam);
	/// ���e����
	afx_msg void OnGenerateMotion(WPARAM wParam, LPARAM lParam);

	/// CAxisControlDialog�֘A
	/// �r�ԍ��ݒ�
	//afx_msg void OnAxisCtrlDlgLegNo(WPARAM wParam, LPARAM lParam);
	/// ���䃂�[�h�ݒ�
	//afx_msg void OnAxisCtrlDlgCtrlMode(WPARAM wParam, LPARAM lParam);
	/// �X���C�_�[�l��M
	//afx_msg void OnAxisCtrlDlgRecieveSldrData(WPARAM wParam, LPARAM lParam);
	/// �_�C�A���O�Ƀf�[�^���M
	//afx_msg void OnAxisCtrlDlgSendData(WPARAM wParam, LPARAM lParam);

	/// �ʐM�֌W
	/// �f�[�^���M�J�n
	afx_msg void OnStartDataSending(WPARAM wParam, LPARAM lParam);
	/// �f�[�^���M��~
	afx_msg void OnStopDataSending(WPARAM wParam, LPARAM lParam);

	/// URGThread�A�h���X�Z�b�g
	//afx_msg void OnURGThreadSet(WPARAM wParam, LPARAM lParam);

/**
 *		�J�X�^�����b�Z�[�W�}�b�v�I��
 */
};



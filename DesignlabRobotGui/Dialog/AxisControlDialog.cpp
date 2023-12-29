/**
 *  �t�@�C����
 *		AxisControlDialog.cpp
 *  ����
 *		������̂��߂̃_�C�A���O�N���X
 *  ���t
 *		�쐬��: 2007/06/11(Mon)		�X�V��: 2007/06/18(Mon)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\stdafx.h"

#include "AxisControlDialog.h"
#include "..\Utility\ColorNames.h"
#include <iostream>
#include "..\System\WinUserMessage.h"
#include "..\Plan\PlanParameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;
using namespace Data;


/**
 *	----------------------------------------------------------------------
 *		�萔�錾
 *	----------------------------------------------------------------------
 */
const int SLDR_RANGE_MIN[2] = {-180, -800};
const int SLDR_RANGE_MAX[2] = {180, 800};
const int SLDR_PAGE_SIZE[2] = {25, 50};
const int SLDR_TIC_FREQ[2] = {100, 400};
const int SLDR_SCALE = 10;

/**
 *	----------------------------------------------------------------------
 *		CAxisControlDialog�N���X
 *	----------------------------------------------------------------------
 */

/**
 *		�t���[�����[�N�����s���ɃN���X����N���X�K�w�ł̃N���X�̈ʒu��m�邽�߂�
 *		CObject �N���X�̓��I�h���I�u�W�F�N�g�ɕK�v�� C++ �̃R�[�h�𐶐�����D
 *
 *		�������͔h���N���X�i���̃N���X�FCAxisControlDialog�j�C�������͊�{�N���X�iCDialog�j���Ƃ�D
 */
IMPLEMENT_DYNAMIC(CAxisControlDialog, CDialog)

BEGIN_MESSAGE_MAP(CAxisControlDialog, CDialog)

	/// Windows�\��
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_WM_TIMER()

	/// Windows�Ǘ�
	ON_BN_CLICKED(IDC_CHECK_LEG1, &CAxisControlDialog::OnBnClickedCheckLeg1)
	ON_BN_CLICKED(IDC_CHECK_LEG2, &CAxisControlDialog::OnBnClickedCheckLeg2)
	ON_BN_CLICKED(IDC_CHECK_LEG3, &CAxisControlDialog::OnBnClickedCheckLeg3)
	ON_BN_CLICKED(IDC_CHECK_LEG4, &CAxisControlDialog::OnBnClickedCheckLeg4)
	ON_BN_CLICKED(IDC_CHECK_POSITION, &CAxisControlDialog::OnBnClickedCheckPosition)
	ON_BN_CLICKED(IDC_CHECK_VELOCITY, &CAxisControlDialog::OnBnClickedCheckVelocity)

	/// �J�X�^�����b�Z�[�W
	ON_MESSAGE(WM_PLAN_SENDING_AC_DATA, OnRecieveData)
	ON_MESSAGE(WM_DLG_AC_DISPLAY, OnSwitchDisplayMode)

END_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		CAxisControlDialog�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
 CAxisControlDialog::CAxisControlDialog(CWnd* pParent /*=NULL*/)
{
	/// �����o�ϐ��̏�����
	pParentWindow = pParent;
	isBrushCreated = false;
	motionPlanThreadID = 0;
	selectedCtrlMode = 0;
	timerID = 0;
}

CAxisControlDialog::~CAxisControlDialog()
{
}

void CAxisControlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_JOINT1, jointSlider1);
	DDX_Control(pDX, IDC_SLIDER_JOINT2, jointSlider2);
	DDX_Control(pDX, IDC_SLIDER_JOINT3, jointSlider3);
	DDX_Control(pDX, IDC_SLIDER_FOOTX, footSliderX);
	DDX_Control(pDX, IDC_SLIDER_FOOTY, footSliderY);
	DDX_Control(pDX, IDC_SLIDER_FOOTZ, footSliderZ);
	DDX_Control(pDX, IDC_CHECK_LEG1, m_CheckLeg1);
	DDX_Control(pDX, IDC_CHECK_LEG2, m_CheckLeg2);
	DDX_Control(pDX, IDC_CHECK_LEG3, m_CheckLeg3);
	DDX_Control(pDX, IDC_CHECK_LEG4, m_CheckLeg4);
	DDX_Control(pDX, IDC_CHECK_POSITION, m_CheckPosition);
	DDX_Control(pDX, IDC_CHECK_VELOCITY, m_CheckVelocity);
}

/**
 *	------------------------------------------------------------
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *		CAxisControlDialog ���b�Z�[�W �n���h��
 *	------------------------------------------------------------
 */
/**
 *	Windows�\��
 */
/// WM_CTLCOLOR�ɑΉ�
HBRUSH CAxisControlDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

/// WM_HSCROLL�ɑΉ�
void CAxisControlDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	/// ���[�J���ϐ�
	int jointNo = 0;	/// �֐ߔԍ�... 1, 2, 3
	int footDirection = 0;	/// �^������... 1, 2, 3
	int sldrNum = 0;		/// �X���C�_�ԍ�

	/// �J�E���^
	int i;

	/// �X���C�_�[�̔���
	CSliderCtrlEx* pSliderBar  = (CSliderCtrlEx*)pScrollBar;

	for (i=0; i< SLIDER_NUM; i++)
	{
		if ( pSliderBar == pSliderCtrl[i] )
		{
			sldrNum = i+1; 
			break;
		}
	}

	switch (sldrNum)
	{
		case 1:
			jointNo = 1; break;
		case 2:
			jointNo = 2; break;
		case 3:
			jointNo = 3; break;
		case 4:
			footDirection = 1; break;
		case 5:
			footDirection = 2; break;
		case 6:
			footDirection = 3; break;
		default:
			break;
	}

	if ( 0 < sldrNum )
	{
		/// �X���C�_�[�̐��f�[�^
		sldrPosition[sldrNum-1] = 0;
		/// �X���C�_�̃����W
		int min = 0;
		int max = 0;

		/// �֐ߒl
		double sldrInfo = 0.0;

		/// �X���C�_�̌��ݒl���擾
		sldrPosition[sldrNum-1] = pSliderBar->GetPos();
		
		pSliderBar->GetRange(min, max);

		switch (nSBCode) 
		{
			case SB_PAGELEFT:		/// �X���C�_�̍������N���b�N�����ꍇ�i�܂���PageUp�L�[�j
				if ( sldrNum < 4)
					sldrPosition[sldrNum-1] -= SLDR_PAGE_SIZE[0];
				else
					sldrPosition[sldrNum-1] -= SLDR_PAGE_SIZE[1];

				/// �ŏ��l����
				if( sldrPosition[sldrNum-1] < min )
					sldrPosition[sldrNum-1] = min;
				
				break;
		
			case SB_PAGERIGHT:		/// �X���C�_�̉E�����N���b�N�����ꍇ�i�܂���PageDown�L�[�j
				if ( sldrNum < 4)
					sldrPosition[sldrNum-1] += SLDR_PAGE_SIZE[0];
				else
					sldrPosition[sldrNum-1] += SLDR_PAGE_SIZE[1];

				if( sldrPosition[sldrNum-1] > max )
					sldrPosition[sldrNum-1] = max;
		
				break;
			
			case SB_THUMBPOSITION:	/// �X���C�_���Έړ��������ꍇ
			case SB_THUMBTRACK:		/// �X���C�_�o�[���h���b�O�ړ��������ꍇ
				sldrPosition[sldrNum-1] = pSliderBar->GetPos();

				break;

			default:
				break;
		}

		// ���݈ʒu�ɃX���C�_�o�[�X�V
		pSliderBar->SetPos( sldrPosition[sldrNum-1] );

		/// �X���C�_�l�̏���
		sldrInfo = (double)sldrPosition[sldrNum-1] / (double)SLDR_SCALE;

		/// edit�{�b�N�X�ɕ\��
		CString data;
		data.Format( TEXT("%5.1lf"), sldrInfo );

		/// �X���C�_�ԍ��ɂ�菈��
		/// edit�{�b�N�X�ւ̕\����MotionPlanThread�ւ̒ʒm
		switch (sldrNum)
		{
			case 1:
				SetDlgItemText(IDC_EDIT_JOINT1, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 2:
				SetDlgItemText(IDC_EDIT_JOINT2, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 3:
				SetDlgItemText(IDC_EDIT_JOINT3, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 4:
				SetDlgItemText(IDC_EDIT_FOOTX, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 5:
				SetDlgItemText(IDC_EDIT_FOOTY, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			case 6:
				SetDlgItemText(IDC_EDIT_FOOTZ, data);
				PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_SENDING_SLDR_DATA, 
											(WPARAM)sldrNum, (LPARAM)sldrInfo);
				break;

			default:
				break;
		}
	}	/// end of if ( 0 < sldrNum )

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/// WM_TIMER�ɑΉ�
void CAxisControlDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_REQUEST_DATA, (WPARAM)this->m_hWnd, NULL);

	CDialog::OnTimer(nIDEvent);
}

/**
 *	Windows�Ǘ��C�x���g
 */
void CAxisControlDialog::OnBnClickedCheckLeg1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (selectedLegNo == 1) selectedLegNo = 0;
	else selectedLegNo = 1;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckLeg2()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (selectedLegNo == 2) selectedLegNo = 0;
	else selectedLegNo = 2;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckLeg3()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (selectedLegNo == 3)	selectedLegNo = 0;
	else selectedLegNo = 3;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckLeg4()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (selectedLegNo == 4) selectedLegNo = 0;
	else selectedLegNo = 4;

	if (selectedLegNo != 0)
		PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_LEG_NO, (WPARAM)selectedLegNo, NULL);

	markSelectedLegButton();
}

void CAxisControlDialog::OnBnClickedCheckPosition()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (selectedCtrlMode == 1) selectedCtrlMode = 0;
	else selectedCtrlMode = 1;

	PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_CTRL_MODE, (WPARAM)selectedCtrlMode, NULL);

	markSelectedModeButton();
}

void CAxisControlDialog::OnBnClickedCheckVelocity()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (selectedCtrlMode == 2) selectedCtrlMode = 0;
	else selectedCtrlMode = 2;

	PostThreadMessage(motionPlanThreadID, WM_PLAN_AC_CTRL_MODE, (WPARAM)selectedCtrlMode, NULL);

	markSelectedModeButton();
}

/**
 *	�J�X�^�����b�Z�[�W
 */
/// �f�[�^�̎�M�ƕ\���X�V
LRESULT CAxisControlDialog::OnRecieveData(WPARAM wParam, LPARAM lParam)
{
	/// ���M�f�[�^���L���X�g���ĕێ�
	dlgAsuraData = *( (AsuraData*)wParam );

	/// �\���f�[�^�X�V
	updateDlgItem();

	return 1;
}

/// ON/OFF�̐ؑ�
LRESULT CAxisControlDialog::OnSwitchDisplayMode(WPARAM wParam, LPARAM lParam)
{
	if ( (int)wParam )
	{
		timerID = 1;
		SetTimer(timerID, 80, NULL);

		ShowWindow(SW_SHOW);

		return 1;
	}
	else
	{
		if (timerID == 1)
			KillTimer(timerID);
		
		timerID = 0;

		ShowWindow(SW_HIDE);

		return 0;
	}

	return 2;
}

/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
BOOL CAxisControlDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	SetWindowText( TEXT("Axis Control") );

	/// View�X�^�C���̐ݒ�
	setDialogStyle(GREEN_THEMED);

	/// �{�^���̐ݒ�
	setButtonControlStyle();

/**
 *		�X���C�_�̐ݒ�
 */
	setSliderControlStyle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

BOOL CAxisControlDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	return CDialog::Create(IDD, pParentWindow);

	/*
	return CDialog::Create(lpszTemplateName, pParentWnd);
	*/
}

void CAxisControlDialog::PostNcDestroy()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	DeleteObject(backColorBrush);
	DeleteObject(editBkColorBrush);
	DeleteObject(staticBkColorBrush);

	delete this;

	CDialog::PostNcDestroy();
}

void CAxisControlDialog::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	DestroyWindow();
	/// EndDialog���Ăяo�����߁C�_�C�A���O�{�b�N�X�͔j�����ꂸ��\���ɂȂ邾��
	/*
	CDialog::OnCancel();
	*/
}
/**
 *	------------------------------------------------------------
 *		����֐�
 *	------------------------------------------------------------
 */

/**
 *	�_�C�A���O�̃X�^�C��������
 */
void CAxisControlDialog::setDialogStyle(ViewStyle style)
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
			backColorBrush			= CreateSolidBrush( DLG_BACK_GREEN_COLOR );
			editBkColorBrush		= CreateSolidBrush( EDIT_BACK_GREEN_COLOR );
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
			backColorBrush			= CreateSolidBrush( DLG_BACK_BLUE_COLOR );
			editBkColorBrush		= CreateSolidBrush( EDIT_BACK_BLUE_COLOR );
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
 *		�X���C�_��ݒ肷��
 */
void CAxisControlDialog::setSliderControlStyle(void)
{
	/// �R���g���[���̃����o�ϐ�����
	pSliderCtrl[0] = &jointSlider1;
	pSliderCtrl[1] = &jointSlider2;
	pSliderCtrl[2] = &jointSlider3;
	pSliderCtrl[3] = &footSliderX;
	pSliderCtrl[4] = &footSliderY;
	pSliderCtrl[5] = &footSliderZ;

	/// �����ʒu�Ɣ͈͂̌���
	int i;
	for (i=0; i<3; i++)
	{
		pSliderCtrl[i]->SetRange(SLDR_RANGE_MIN[0]*SLDR_SCALE, SLDR_RANGE_MAX[0]*SLDR_SCALE);
		pSliderCtrl[i]->SetPageSize(SLDR_PAGE_SIZE[0]);
		pSliderCtrl[i]->SetTicFreq(SLDR_TIC_FREQ[0]);
		pSliderCtrl[i]->SetPos(0);
	}

	int j;
	for (j=3; j<6; j++)
	{
		pSliderCtrl[j]->SetRange(SLDR_RANGE_MIN[1]*SLDR_SCALE, SLDR_RANGE_MAX[1]*SLDR_SCALE);
		pSliderCtrl[j]->SetPageSize(SLDR_PAGE_SIZE[1]);
		pSliderCtrl[j]->SetTicFreq(SLDR_TIC_FREQ[1]);
		pSliderCtrl[j]->SetPos(0);
	}

	CString data;
	/// edit�̕\��
	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[0]->GetPos() );
	SetDlgItemText(IDC_EDIT_JOINT1, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[1]->GetPos() );
	SetDlgItemText(IDC_EDIT_JOINT2, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[2]->GetPos() );
	SetDlgItemText(IDC_EDIT_JOINT3, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[3]->GetPos() );
	SetDlgItemText(IDC_EDIT_FOOTX, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[4]->GetPos() );
	SetDlgItemText(IDC_EDIT_FOOTY, data);

	data.Format( TEXT("%5.1lf"), (double)pSliderCtrl[5]->GetPos() );
	SetDlgItemText(IDC_EDIT_FOOTZ, data);


	UpdateData(FALSE);

	/// �X���C�_�̐F����
	int k;
	for (k=0; k<6; k++)
	{
		pSliderCtrl[k]->SetThumbColors(SLDR_THUMB_COLOR, SLDR_THUMB_HILIGHT_COLOR);
		sldrPosition[k] = 0;
	}

	return;
}

/// �{�^���̐ݒ�
void CAxisControlDialog::setButtonControlStyle(void)
{
	int i;
	selectedLegNo = 0;
	selectedCtrlMode = 0;

	/// �A�h���X���
	pCheckLeg[0] = &m_CheckLeg1;
	pCheckLeg[1] = &m_CheckLeg2;
	pCheckLeg[2] = &m_CheckLeg3;
	pCheckLeg[3] = &m_CheckLeg4;

	pCheckMode[0] = &m_CheckPosition;
	pCheckMode[1] = &m_CheckVelocity;

/**
 *	----------------------------------------
 *	�`��̌���
 *	----------------------------------------
 */
	/// �X�^�C���f�[�^
	tButtonStyle tStyle;

	/// ���݂̃{�^���X�^�C���̐ݒ��tStyle�Ɋi�[
	m_DefaultRoundButtonStyle.GetButtonStyle(&tStyle);
	/**
	 *	�X�^�C���ύX
	 */
	tStyle.m_dRadius = 12.0;
	tStyle.m_tColorFace.m_tEnabled = RGB(64, 128, 255);
	tStyle.m_tColorFace.m_tClicked = RGB(255, 223, 128);
	tStyle.m_tColorBorder.m_tEnabled = RGB(255, 223, 128);
	tStyle.m_tColorBorder.m_tClicked = RGB(64, 128, 255);
	tStyle.m_tColorBack.m_tEnabled = DLG_BACK_GREEN_COLOR;
	tStyle.m_tColorBack.m_tClicked = DLG_BACK_GREEN_COLOR;
	/// �ύX��tStyle����ǂݍ���
	m_DefaultRoundButtonStyle.SetButtonStyle(&tStyle);

	/// �ݒ�𔽉f
	for (i=0;i<4;i++)
	{
		pCheckLeg[i]->SetRoundButtonStyle(&m_DefaultRoundButtonStyle);
		pCheckLeg[i]->SetCheck(false);
	}

	for (i=0;i<2;i++)
	{
		pCheckMode[i]->SetRoundButtonStyle(&m_DefaultRoundButtonStyle);
		pCheckMode[i]->SetCheck(false);
	}

/**
 *	----------------------------------------
 *	�t�H���g�̌���
 *	----------------------------------------
 */
	/// �t�H���g�I�u�W�F�N�g
	LOGFONT tFont;
	/// ���݂̐ݒ���i�[
	for (i=0;i<4;i++) pCheckLeg[i]->GetFont(&tFont);
	for (i=0;i<2;i++) pCheckMode[i]->GetFont(&tFont);

	/**
	 *	�ݒ��ύX����
	 */
	strcpy((char*)tFont.lfFaceName,"Verdana");
	tFont.lfHeight = 12;
	/// �ݒ�𔽉f����
	for (i=0;i<4;i++) pCheckLeg[i]->SetFont(&tFont);
	for (i=0;i<2;i++) pCheckMode[i]->SetFont(&tFont);

/**
 *	----------------------------------------
 *	�t�H���g�J���[�̌���
 *	----------------------------------------
 */
	/// �J���[�I�u�W�F�N�g
	tColorScheme tColor;
	/// ���݂̐ݒ���i�[
	for (i=0;i<4;i++) pCheckLeg[i]->GetTextColor(&tColor);
	for (i=0;i<2;i++) pCheckMode[i]->GetTextColor(&tColor);

	/**
	 *	�ݒ��ύX����
	 */
	tColor.m_tEnabled = RGB(255, 255, 128);
	tColor.m_tClicked = RGB(16, 64, 255);
	/// �ݒ�𔽉f����
	for (i=0;i<4;i++) pCheckLeg[i]->SetTextColor(&tColor);
	for (i=0;i<2;i++) pCheckMode[i]->SetTextColor(&tColor);
}

/// ���b�Z�[�W�𑗂�X���b�hID�̐ݒ�
void CAxisControlDialog::setMotionPlanThreadID(DWORD id)
{
	motionPlanThreadID = id;
	return;
}

/**
 *	------------------------------------------------------------
 *		CAxisControlDialog��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
void CAxisControlDialog::markSelectedLegButton(void)
{
	int i;
	for (i=0; i<4; i++)
	{
		if (i == selectedLegNo - 1) pCheckLeg[i]->SetCheck(true);
		else pCheckLeg[i]->SetCheck(false);
	}

	Invalidate(FALSE);
	return;
}

void CAxisControlDialog::markSelectedModeButton(void)
{
	int i;
	for (i=0; i<2; i++)
	{
		if (i == selectedCtrlMode -1) pCheckMode[i]->SetCheck(true);
		else pCheckMode[i]->SetCheck(false);
	}

	Invalidate(FALSE);
	return;
}

void CAxisControlDialog::updateDlgItem(void)
{
	if ( selectedLegNo == 0 )
		return;

	double data[AC_DLG_AXIS_NUM];

	/// �e��f�[�^�̎擾
	data[0] = ( dlgAsuraData.getLegJointAngle(selectedLegNo) )(1)*RAD2DEG;
	data[1] = ( dlgAsuraData.getLegJointAngle(selectedLegNo) )(2)*RAD2DEG;
	data[2] = ( dlgAsuraData.getLegJointAngle(selectedLegNo) )(3)*RAD2DEG;
	data[3] = ( dlgAsuraData.getLegFootPosition(selectedLegNo) )(1);
	data[4] = ( dlgAsuraData.getLegFootPosition(selectedLegNo) )(2);
	data[5] = ( dlgAsuraData.getLegFootPosition(selectedLegNo) )(3);


	/// edit�R���g���[���ւ̕\��
	CString edit;

	edit.Format(TEXT( "%5.1lf"), data[0] ); SetDlgItemText(IDC_EDIT_JOINT1, edit);
	edit.Format(TEXT( "%5.1lf"), data[1] ); SetDlgItemText(IDC_EDIT_JOINT2, edit);
	edit.Format(TEXT( "%5.1lf"), data[2] ); SetDlgItemText(IDC_EDIT_JOINT3, edit);
	edit.Format(TEXT( "%5.1lf"), data[3] ); SetDlgItemText(IDC_EDIT_FOOTX, edit);
	edit.Format(TEXT( "%5.1lf"), data[4] ); SetDlgItemText(IDC_EDIT_FOOTY, edit);
	edit.Format(TEXT( "%5.1lf"), data[5] ); SetDlgItemText(IDC_EDIT_FOOTZ, edit);


	UpdateData(FALSE);

	/// SliderBar�̕ύX
	int i;
	for ( i=0; i<AC_DLG_AXIS_NUM; i++ )
		pSliderCtrl[i]->SetPos( (int)(data[i]*SLDR_SCALE) );

	return;
}


/**
 *  �t�@�C����
 *		AxisControlDialog.h
 *  ����
 *		������̂��߂̃_�C�A���O�N���X
 *  ���t
 *		�쐬��: 2007/06/11(Mon)		�X�V��: 2007/06/18(Mon)
 */

#pragma once

// CAxisControlDialog �_�C�A���O

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\resource.h"
#include "afxcmn.h"

#include "..\Data\AsuraData.h"

#include "..\System\ViewSetting.h"
#include "..\System\Control\RoundButtonStyle.h"
#include "..\System\Control\RoundButton.h"
#include "..\System\Control\SliderCtrlEx.h"


/**
 *	----------------------------------------------------------------------
 *		CAxisControlDialog�N���X
 *	----------------------------------------------------------------------
 */
class CAxisControlDialog : public CDialog
{
	DECLARE_DYNAMIC(CAxisControlDialog)

/**
 *	------------------------------------------------------------
 *		�萔
 *	------------------------------------------------------------
 */
	static const int SLIDER_NUM = 6;

/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	DDX�ϐ�
 *	----------------------------------------
 */
/**
 *	���E���h�{�^���N���X
 */
	CRoundButtonStyle m_DefaultRoundButtonStyle;

	CRoundButton* pCheckLeg[4];
	CRoundButton* pCheckMode[2];

	CRoundButton m_CheckLeg1;
	CRoundButton m_CheckLeg2;
	CRoundButton m_CheckLeg3;
	CRoundButton m_CheckLeg4;

	CRoundButton m_CheckPosition;
	CRoundButton m_CheckVelocity;

/**
 *	�X���C�_�[�R���g���[��
 */
	CSliderCtrlEx jointSlider1;
	CSliderCtrlEx jointSlider2;
	CSliderCtrlEx jointSlider3;
	CSliderCtrlEx footSliderX;
	CSliderCtrlEx footSliderY;
	CSliderCtrlEx footSliderZ;

	CSliderCtrlEx* pSliderCtrl[SLIDER_NUM];	

/**
 *	�e�E�B���h�E
 */
	CWnd* pParentWindow;

/**
 *	���b�Z�[�W�𑗂�X���b�h��ID
 */
	DWORD motionPlanThreadID;

/**
 *	�^�C�}
 */
	/// �^�C�}���ʎq
	UINT_PTR	timerID;

/**
 *	View�̃X�^�C��
 */
	ViewStyle viewStyle;

/**
 *	�F
 */
	/// �G�f�B�b�g�e�L�X�g�F
	COLORREF editColor;
	/// �X�^�e�B�b�N�e�L�X�g�F
	COLORREF staticColor;

	/// �G�f�B�b�g�{�b�N�X�w�i�F
	COLORREF editBkColor;
	/// �X�^�e�B�b�N�e�L�X�g�w�i�F
	COLORREF staticBkColor;

/**
 *	�u���V
 */
	/// �_�C�A���O�w�i
	HBRUSH backColorBrush;
	/// �G�f�B�b�g�w�i
	HBRUSH editBkColorBrush;
	/// �e�L�X�g�w�i
	HBRUSH staticBkColorBrush;

/**
 *	�t���O
 */
	bool isBrushCreated;
	int selectedLegNo;
	int selectedCtrlMode;

/**
 *	�f�[�^
 */
	/// Asura�f�[�^
	Data::AsuraData dlgAsuraData;

	/// �X���C�_�f�[�^
	int sldrPosition[SLIDER_NUM];

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
	CAxisControlDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CAxisControlDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_AXIS_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *	------------------------------------------------------------
 */
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedCheckLeg1();
	afx_msg void OnBnClickedCheckLeg2();
	afx_msg void OnBnClickedCheckLeg3();
	afx_msg void OnBnClickedCheckLeg4();
	afx_msg void OnBnClickedCheckPosition();
	afx_msg void OnBnClickedCheckVelocity();

	afx_msg LRESULT OnRecieveData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSwitchDisplayMode(WPARAM wParam, LPARAM lParam);

/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
public:
	virtual BOOL OnInitDialog();
	virtual BOOL Create(LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);

protected:
	virtual void PostNcDestroy();
	virtual void OnCancel();

/**
 *	------------------------------------------------------------
 *		����֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�_�C�A���O�̃X�^�C���̐ݒ�
 *	----------------------------------------
 */
	/**
	 *	����
	 *		�_�C�A���O�̔w�i�Ȃǂ�ݒ�
	 *	����
	 *		style: �w�i�̃X�^�C��
	 */
	void setDialogStyle(ViewStyle style);
	/**
	 *	����
	 *		�X���C�_�̐ݒ�
	 */
	void setSliderControlStyle(void);
	/**
	 *	����
	 *		�{�^���̐ݒ�
	 */
	void setButtonControlStyle(void);

	/**
	 *	����
	 *		���b�Z�[�W�𑗂�X���b�hID�̐ݒ�
	 *	����
	 *		id: �X���b�hID
	 */
	void setMotionPlanThreadID(DWORD id);
	/**
	 *	����
	 *		���b�Z�[�W�𑗂�X���b�hID�̎擾
	 */
	DWORD getMotionPlanThreadID(void) const{return motionPlanThreadID;}

private:
	/// �{�^���I���̃w���v�֐�
	void markSelectedLegButton(void);
	void markSelectedModeButton(void);

	/// �\���̍X�V
	void updateDlgItem(void);
};


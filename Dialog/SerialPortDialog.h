/**
 *  �t�@�C����
 *		SerialPortDialog.h
 *  ����
 *		�V���A���ʐM�̃��O�\���̂��߂̃_�C�A���O
 *  ���t
 *		�쐬��: 2007/06/11(Mon)		�X�V��: 2007/11/10(Sat)
 */

#pragma once


// CSerialPortDialog �_�C�A���O

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\resource.h"

#include "..\Comm\SerialPort.h"
#include "..\Comm\SciPacket.h"
//�ǉ��w�b�_�[�t�@�C��*//
#include "..\UdpSocket.h"


#include "..\Data\AsuraData.h"
#include "..\Data\PlanData.h"

#include "..\System\ViewSetting.h"
#include "..\File\FileWriter.h"
#include "..\Utility\RingBuffer.h"


/**
 *	----------------------------------------------------------------------
 *		CSerialPortDialog�N���X
 *	----------------------------------------------------------------------
 */
class CSerialPortDialog : public CDialog
{
	DECLARE_DYNAMIC(CSerialPortDialog) //new����������֐��̐錾
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:
	static const int MAX_BUFFER_SIZE = 2048;

private:
/**
 *	�e�E�B���h�E
 */
	CWnd* pParentWindow;

/**
 *	View�̃X�^�C��
 */
	ViewStyle viewStyle;

/**
 *	�ʐM�|�[�g
 */
	Comm::SerialPort* pCommPort;
	
	UDP::UdpSocket* pUdpPort ;

/**
 *	���b�Z�[�W�𑗂�X���b�h��ID
 */
	DWORD motionPlanThreadID;

/**
 *	�ʐM�֌W
 */
	/// �ʐM�p��AsuraData
	Data::AsuraData commAsuraData;
	Data::PlanData commPlanData;

	Data::AsuraData *Asuratest;

	/**
	 *	���M�o�b�t�@
	 */
	char longPacket[Comm::SCI_PACKET_SIZE_LONG];//unsigned char
	unsigned char shortPacket[Comm::SCI_MAX_JOINT_NUM][Comm::SCI_PACKET_SIZE_SHORT];

	/**
	 *	��M�p�o�b�t�@
	 */
	RingBuffer rxBuffer;

	/**
	 *	�p�P�b�g�^�C�v
	 */
	Comm::SciPacketType sciPacketType;

/**
 *	���O�p�̃t�@�C�����C�^�I�u�W�F�N�g
 */
	System::FileWriter* pSendLogWriter;

/**
 *	�^�C�}
 */
	/// �^�C�}���ʎq
	UINT_PTR	timerID;

/**
 *	���O�p������
 */
	/// �V���[�g�p�P�b�g���M�p
	CString shortPacketString[Comm::SCI_MAX_JOINT_NUM];
	/// �����O�p�P�b�g���M�p
	CString longPacketString;

	/// ��M�p
	CString rxDataStr;

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
	/// �u���V
	bool isBrushCreated;
	/// �f�[�^�\��
	bool isSendingDataDisplayed;
	bool isRecievingDataDisplayed;

	/// ���O���s����
	bool isLogging;

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
	CSerialPortDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSerialPortDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SERIAL_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

/**
 *	------------------------------------------------------------
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *	------------------------------------------------------------
 */
public:
	/// Windows�Ǘ�
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	/// �J�X�^�����b�Z�[�W
	/// �_�C�A���O�\��/��\���ؑ�
	afx_msg LRESULT OnSwitchDisplayMode(WPARAM wParam, LPARAM lParam);

	/// �ʐM�J�n
	afx_msg LRESULT OnStartPortListening(WPARAM wParam, LPARAM lParam);
	/// �ʐM��~
	afx_msg LRESULT OnStopPortListening(WPARAM wParam, LPARAM lParam);

	/// �w�ߒl�����{�b�g�֑��M
	afx_msg LRESULT OnSendCommandData(WPARAM wParam, LPARAM lParam);

	/// �V���A���ʐM��M
	afx_msg LRESULT OnReceiveData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHandleReceiveMessage(WPARAM wParam, LPARAM lParam);

	/// ���O�J�n
	afx_msg LRESULT OnStartLogging(WPARAM wParam, LPARAM lParam);
	/// ���O��~
	afx_msg LRESULT OnStopLogging(WPARAM wParam, LPARAM lParam);

	/// ��ʍX�V
	afx_msg LRESULT OnRedrawDialog(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName = NULL, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

protected:
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();

/**
 *	------------------------------------------------------------
 *		����֐�
 *	------------------------------------------------------------
 */
public:
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

	/**
	 *	����
	 *		�_�C�A���O�̔w�i�Ȃǂ�ݒ�
	 *	����
	 *		style: �w�i�̃X�^�C��
	 */
	void setDialogStyle(ViewStyle style);

	/**
	 *	����
	 *		���M�p�P�b�g�^�C�v�̐ݒ�
	 *	����
	 *		type: �p�P�b�g�^�C�v
	 */
	void setSciPacketType(Comm::SciPacketType type){sciPacketType = type; return;}

	/**
	 *	����
	 *		���M�p�P�b�g�^�C�v�̎擾
	 */
	Comm::SciPacketType getSciPacketType(void){return sciPacketType;}

/**
 *	���O�\���̂��߂̊֐�
 */
	void displayLogData(UINT itemID, LPCTSTR lpFmt, ...);

/**
 *	����
 *		�V���A���ʐM�p�p�P�b�g�f�[�^�̍쐬
 *	����
 *		type: �p�P�b�g�^�C�v
 *		packet: ���M�o�b�t�@
 *		adress: ���M��֐ߔԍ��i�P�֐ߖ��߂̏ꍇ�̂݁j
 *		command: �w�ߎ��
 */
	bool buildPacket(Comm::SciPacketType type, char* packet, int address, int command);//unsigned char* packet
};


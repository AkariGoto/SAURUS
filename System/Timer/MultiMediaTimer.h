/**
 *  �t�@�C����
 *		MultiMediaTimer.h
 *  ����
 *		�}���`���f�B�A�^�C�}���g�p�����^�C�}(Windows��p)
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

#ifndef __MultiMediaTimer_h__
#define __MultiMediaTimer_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>

#include "..\CriticalSection.h"

/**
 *		winmm.lib���g�p
 *		�����J�֒ʒm
 */
#pragma comment (lib, "winmm.lib")

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		�萔�A�}�N���錾
 *	----------------------------------------------------------------------
 */

/**
 *		�Ăяo���I�u�W�F�N�g���L�����ǂ����`�F�b�N����}�[�J
 *		0xF5F5F5F5��illegal�ȃA�h���X�̒l
 *		�璷�`�F�b�N�p
 */
const   DWORD   checkMMTimerMarker = 0xF5F5F5F5;


/**
 *	----------------------------------------------------------------------
 *		MultiMediaTimerCallback�N���X
 *			�R�[���o�b�N�֐��̃C���^�[�t�F�C�X
 *			�J��Ԃ�������Timer�N���X���痣����
 *			run()����̌Ăяo�����}���`���f�B�^�C�}
 *			�N���X�̊֐��̓˔��I�ȌĂяo����h��
 *	----------------------------------------------------------------------
 */
/**
 *		�^�C�}�[���Z�b�g����ƈ��̊Ԋu��
 *		���̃R�[���o�b�N�֐������s�����.
 *
 *		�܂�1��O�̃R�[���o�b�N���I���Ȃ��Ă�
 *		���̃R�[�����Ăяo�����̂Œ���
 */
class MultiMediaTimerCallback
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

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
	/// �f�t�H���g�R���X�g���N�^
	MultiMediaTimerCallback(){}
	/// �f�X�g���N�^
	virtual ~MultiMediaTimerCallback(){}

/**
 *	----------------------------------------
 *	�^�C�}�Ŏ��ۂɏ��������
 *	----------------------------------------
 */
	/// ���[�`���J�n���ɌĂяo������
	virtual void	onStart(void){}
	/// ���[�`���I�����ɌĂяo������
	virtual void	onStop(void){}
	/**
	 *		�I�[�o�[���C�h�K�{�̃^�C�}���[�`��
	 *		�I�����ɂ�false��Ԃ�
	 */
	virtual bool	onMultiMediaTimer(void)=0;//false;

};	/// end of class MultiMediaTimerCallback

/**
 *		//////////////////////////////////////////////////
 *				MultiMediaTimer�N���X
 *		//////////////////////////////////////////////////
 */
/**
 *	----------------------------------------------------------------------
 *		MultiMediaTimer�N���X
 *	----------------------------------------------------------------------
 */
/**
 *		�g����
 *			MultiMediaTimerCallback�N���X��h��������, 
 *			onMultiMediaTimer()���I�[�o�[���C�h����
 *
 *			setTimer()���Ăяo�������������, period����
 *			MultiMediaTimerCallback�N���X��	onMultiMediaTimer���Ă΂��
 */
class MultiMediaTimer
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
private:

/**
 *	----------------------------------------
 *	�^�C�}�̂��߂̊e��ϐ�
 *	----------------------------------------
 */
/**
 *	�I�u�W�F�N�g�ϐ�
 *		timerCallback				: �R�[���o�b�N�֐����ŏ��������֐�
 *		criticalSection				: �N���e�B�J���Z�N�V�����I�u�W�F�N�g
 */
	MultiMediaTimerCallback&		timerCallback;
	CriticalSection						criticalSection;

/**
 *	�^�C�}�̐��\�Ɋ֌W����ϐ�
 *		period						: �^�C�}�C�x���g����[msec]
 *		resolution					: �^�C�}�C�x���g�̕���\[msec]
 *		timerID						: �^�C�}�C�x���g�̎��ʎq
 *		frequency					: performanceFrequency�̒P�ʕϊ��l[sec]
 *		performanceFrequency	: �V�X�e���N������̌o�ߎ���[msec]
 */
	UINT			timerPeriod;
	UINT			timerResolution;
	MMRESULT		timerID;
	double			timerFrequency;
	LARGE_INTEGER	performanceFrequency;


/**
 *	�t���O�ϐ�
 *		isTimerEnded				: �^�C�}���I����Ă��邩�ǂ���
 *		isTimerWarned				: �x�����󂯂����ǂ���
 */
	bool	isTimerEnded;
	bool	isTimerWarned;

/**
 *	�}���`���f�B�A�^�C�}�̊J�n����
 */
	double startTime;

/**
 *	�����p�̃}�[�J
 */
	DWORD	marker;

protected:

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
	/// �f�t�H���g�R���X�g���N�^
	MultiMediaTimer(MultiMediaTimerCallback& timerCallback_);
	/// �f�X�g���N�^
	virtual ~MultiMediaTimer();

/**
 *	����
 *		�A�N�Z�X�֐�
 */
	UINT	getPeriod(void)	 const{ return timerPeriod; }
	UINT	getResolution(void) const{ return timerResolution; }
	double	getStartTime(void) const{ return startTime; }

/**
 *	����
 *		�^�C�}�[�������Ă��邩�ǂ���
 */
	bool isActive(void) const{ return (timerID != 0); }

/**
 *	����
 *		�^�C�}�R�[���o�b�N�I�u�W�F�N�g�̃Z�b�g
 *		�^�C�}���J�n����O�ɕK���s��
 *	����
 *		�R�[���o�b�N�֐��ւ̃|�C���^
 */
	bool setMultiMediaTimerCallback(MultiMediaTimerCallback& timerCallback_);

/**
 *	����
 *		�^�C�}�̃Z�b�g
 *	����
 *		period: ����
 *		resolution: �^�C�}�̕���\
 */
	bool setTimer(UINT period, UINT resolution = 1);

/**
 *	����
 *		�^�C�}�̔j��
 */
	bool killTimer(void);

/**
 *	����
 *		�^�C�}�̎����ƕ���\�̃��Z�b�g
 */
	void resetPeriodAndResolution(void);

/**
 *	����
 *		���݂�CPU�����̎擾
 */
	double getPresentTime(void);

private:
/**
 *	����
 *		�^�C�}�v���V�[�W���̃R�[���o�b�N�֐�(static�Ȃ̂̓R�[���o�b�N�֐��̂���)
 *	����
 *		uID			: �^�C�}ID
 *		uMsg		: �s�g�p
 *		dwUser		: ���[�U��`(���this�|�C���^������)
 *		dw1			: �s�g�p
 *		dw2			: �s�g�p
 */
	static void CALLBACK multiMediaTimerProcedure(
									UINT uID, 
									UINT uMsg,
									DWORD_PTR dwUser, 
									DWORD_PTR dw1, 
									DWORD_PTR dw2
									);

/**
 *	����
 *		�^�C�}�̃R�[���o�b�N�֐��ŌĂяo������
 */
	void	run(void);

/**
 *	����
 *		�璷�����p
 */
	bool	checkMarker(void) const{return ( marker == checkMMTimerMarker );}

protected:
};	/// end of class MultiMediaTimer

}

#endif // __MMTimer_h__